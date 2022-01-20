/*
 * 
 * SA:MP DEntisT's Utilities
 *
 * Since 2019., DEntisT
 *
 *
 *
 * - Library which provides creating
 *   of property entrances and exits,
 *   and interiors.
 *
 *
 */

#if defined DEV_MODE

#include <a_samp>

#endif

#tryinclude "d_extra\d_libtags.inc"

#if !defined d_extra_included
    #error [samp-detutils]: (error) - You are missing "d_extra\d_libtags.inc" in your "include\DETUTILS" folder.
#endif

#if defined d_properties_included

    #endinput

#endif

#define d_properties_included

#define DisableDefaultProperties DisableInteriorEnterExits

#if !defined MAX_PROPERTIES

#define MAX_PROPERTIES 300

#endif

#if !defined IsValidString
        #define IsValidString(%1) ((!(%1[0])) || (((%1[0]) == '\1') && (!(%1[1]))))
#endif

enum enum_PropertyData
{
    p_PropertyName[32],

    Float:p_PropertyEntrance_X,
    Float:p_PropertyEntrance_Y,
    Float:p_PropertyEntrance_Z,
    Float:p_PropertyEntrance_A,

    Float:p_PropertyExit_X,
    Float:p_PropertyExit_Y,
    Float:p_PropertyExit_Z,
    Float:p_PropertyExit_A,

    p_PropertyIntid,
    p_PropertyIntid_OUT,
    p_PropertyVwid,
    p_PropertyVwid_OUT,

    p_PropertyPickup,
    p_PropertyPickupID,
    p_PropertyLabel,
    p_PropertyLabelID
};

forward OnPropertyCreated(propertyid);
forward OnPlayerEnterProperty(playerid);
forward OnPlayerExitProperty(playerid);

new g_PlayerProperty[MAX_PLAYERS];

new g_PropertyData[MAX_PROPERTIES][enum_PropertyData];

new g_PropertyID;

new _EntranceTitleShown[MAX_PLAYERS];

// Interior IDs:

#define DETUTILS_PROP_PARAMS__(%0,%1,%2,%3); \
    g_PropertyData[g_PropertyID][p_PropertyIntid] = %3;g_PropertyData[g_PropertyID][p_PropertyVwid] = %3;_eInteriorid = %3; _eVirtualWorldid = %3;g_PropertyData[g_PropertyID][p_PropertyExit_X] = %0;g_PropertyData[g_PropertyID][p_PropertyExit_Y] = %1;g_PropertyData[g_PropertyID][p_PropertyExit_Z] = %3;

#define INTERIOR_CUSTOM         -1
#define INTERIOR_MARKET_247_1    0
#define INTERIOR_MARKET_247_2    1
#define INTERIOR_MARKET_247_3    2
#define INTERIOR_MARKET_247_4    3
#define INTERIOR_MARKET_247_5    4
#define INTERIOR_MARKET_247_6    5

/*

                                                              
88        88                            88                    
88        88                            88                    
88        88                            88                    
88aaaaaaaa88   ,adPPYba,    ,adPPYba,   88   ,d8   ,adPPYba,  
88""""""""88  a8"     "8a  a8"     "8a  88 ,a8"    I8[    ""  
88        88  8b       d8  8b       d8  8888[       `"Y8ba,   
88        88  "8a,   ,a8"  "8a,   ,a8"  88`"Yba,   aa    ]8I  
88        88   `"YbbdP"'    `"YbbdP"'   88   `Y8a  `"YbbdP"'  
                                                              
                                                              

*/

#define int_%0\32; int_

#define PROPERTY_ACTION_ENTER 0
#define PROPERTY_ACTION_EXIT 1

void:_PropertyLoadingTime(const playerid)
{
    #if defined detutils_debug
    printf("[samp-detutils]: (debug) - tpcll :: Player ( id : %i ) is now controlable.", playerid);
    #endif
    rem:<To prevent symbol-never-used error:>;
    new _c = playerid;
    _c++;
    SetTimer("_ToggleControllable", 1000, false);
}

static g_HideAnnouncement[MAX_PLAYERS];

static _Announcement(playerid, const _Message[])
{
    new c_Message[300];

    KillTimer(g_HideAnnouncement[playerid]);

    format(c_Message,300,"%s",_Message);

    TextDrawShowForPlayer(playerid, g_AnnouncementUsebox);

    PlayerTextDrawSetString( playerid, g_AnnouncementString[ playerid ], c_Message );

    PlayerTextDrawShow(playerid,g_AnnouncementString[ playerid ]);

    g_HideAnnouncement[playerid] = _SetTimer("hideannc", 4000, false, "i", playerid);
    return 1;
}

DETUTILS_DECL__ func:hideannc(playerid)
{
    TextDrawHideForPlayer(playerid, g_AnnouncementUsebox);
    PlayerTextDrawHide(playerid, g_AnnouncementString[playerid]);
    KillTimer(g_HideAnnouncement[playerid]);
    return 1;
}

static _CreateAnncTextDraws()
{
    g_AnnouncementUsebox = TextDrawCreate(366.444427, 363.882232, "usebox");
    TextDrawLetterSize(g_AnnouncementUsebox, 0.000000, 1.840366);
    TextDrawTextSize(g_AnnouncementUsebox, 267.333312, 0.000000);
    TextDrawAlignment(g_AnnouncementUsebox, 1);
    TextDrawColor(g_AnnouncementUsebox, 0);
    TextDrawUseBox(g_AnnouncementUsebox, true);

    TextDrawBoxColor(g_AnnouncementUsebox, 102);
    TextDrawSetShadow(g_AnnouncementUsebox, 0);
    TextDrawSetOutline(g_AnnouncementUsebox, 0);
    TextDrawFont(g_AnnouncementUsebox, 0);
    return 1;
}

//#define STRING "string" // for some reasons this exists

static _CreatePlayerTextDraws(playerid)
{
    g_AnnouncementString[playerid] = CreatePlayerTextDraw(playerid, 315.111145, 364.373321, "string");
    PlayerTextDrawLetterSize(playerid, g_AnnouncementString[playerid], 0.201111, 1.346133);

    PlayerTextDrawAlignment(playerid, g_AnnouncementString[playerid], 2);
    PlayerTextDrawColor(playerid, g_AnnouncementString[playerid], -1);
    PlayerTextDrawSetShadow(playerid, g_AnnouncementString[playerid], 0);

    PlayerTextDrawSetOutline(playerid, g_AnnouncementString[playerid], 1);
    PlayerTextDrawBackgroundColor(playerid, g_AnnouncementString[playerid], 51);
    PlayerTextDrawFont(playerid, g_AnnouncementString[playerid], 2);
    PlayerTextDrawSetProportional(playerid, g_AnnouncementString[playerid], 1);
    return 1;
}

forward _ToggleControllable();

public _ToggleControllable()
{
    for(new i; i < MAX_PLAYERS; i++)
    {
        TogglePlayerControllable(i, true);
    }
    rem:<This is used to prevent player being frozen for several minutes>;
    return 1;
}

#if !defined FILTERSCRIPT

public OnGameModeInit()
{
    _CreateAnncTextDraws();
    
    if(funcidx("a_props_OnGameModeInit") != -1)
    {
        CallLocalFunction("a_props_OnGameModeInit", "");
    }
    return 1;
}

#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit
#else
    #define _ALS_OnGameModeInit
#endif

#define OnGameModeInit a_props_OnGameModeInit

forward a_props_OnGameModeInit();

#endif

public OnPlayerConnect(playerid)
{
    _CreatePlayerTextDraws(playerid);
    if(funcidx("a_props_OnPlayerConnect") != -1)
    {
        CallLocalFunction("a_props_OnPlayerConnect", "i", playerid);
    }
    return 1;
}

#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif

#define OnPlayerConnect a_props_OnPlayerConnect

forward a_props_OnPlayerConnect(playerid);

public OnPlayerSpawn(playerid)
{
    TextDrawHideForPlayer(playerid, g_AnnouncementUsebox);
    PlayerTextDrawHide(playerid, g_AnnouncementString[playerid]);
    if(funcidx("a_props_OnplayerSpawn") != -1)
    {
        CallLocalFunction("a_props_OnplayerSpawn", "i", playerid);
    }
    return 1;
}

#if defined _ALS_OnPlayerSpawn
    #undef OnPlayerSpawn
#else
    #define _ALS_OnPlayerSpawn
#endif

#define OnPlayerSpawn a_props_OnplayerSpawn

forward a_props_OnplayerSpawn(playerid);

_OnPlayerNearEntrance(playerid);

public _OnPlayerNearEntrance(playerid)
{
    //new PropertyFuncName[100];
    new PropertyActionID;
    
    for(new i; i < MAX_PROPERTIES; i++)
    {
        if( IsPlayerInRangeOfPoint( playerid, 1.5, g_PropertyData[ i ][p_PropertyEntrance_X], \
        g_PropertyData[ i ][p_PropertyEntrance_Y], g_PropertyData[ i ][p_PropertyEntrance_Z]) \
        && GetPlayerVirtualWorld( playerid ) == g_PropertyData[ i ][p_PropertyVwid_OUT] \
        && !IsPlayerInAnyVehicle( playerid ))
        {
            PropertyActionID = 0;
            /*format(PropertyFuncName,sizeof(PropertyFuncName),
                "OnPropertyActionPerformed",g_PropertyData[ i ][p_PropertyName]);*/

            if(funcidx("OnPropertyActionPerformed") != -1) CallLocalFunction("OnPropertyActionPerformed",
                                                                "iii",playerid,i,PropertyActionID);

            SetPlayerInterior( playerid, g_PropertyData[ i ][p_PropertyIntid] );
            SetPlayerVirtualWorld( playerid, g_PropertyData[ i ][p_PropertyVwid] );
            
            d_ac_SetPlayerPos( playerid, g_PropertyData[ i ][p_PropertyExit_X], g_PropertyData[ i ][p_PropertyExit_Y], \
            g_PropertyData[ i ][p_PropertyExit_Z] );
            
            SetPlayerFacingAngle( playerid, g_PropertyData[ i ][p_PropertyExit_A] );
            SetCameraBehindPlayer( playerid );
            
            #if defined detutils_debug
            printf("[samp-detutils]: (debug) - nearintcallback :: Player entered the interior with id : %i.", i);
            #endif

            TogglePlayerControllable(playerid,false);

            SetPlayerTime( playerid, 12, 0 );
            _PropertyLoadingTime(playerid);
            
            g_PlayerProperty[ playerid ] = i;
            
            if(funcidx("OnPlayerEnterProperty") != -1)
                CallLocalFunction("OnPlayerEnterProperty", "i", playerid);
            
        }
        else if( IsPlayerInRangeOfPoint( playerid, 1.5, g_PropertyData[ i ][p_PropertyExit_X], \
        g_PropertyData[ i ][p_PropertyExit_Y], g_PropertyData[ i ][p_PropertyExit_Z]) && \
        GetPlayerVirtualWorld( playerid ) == g_PropertyData[ i ][p_PropertyVwid])
        {
            SetPlayerVirtualWorld( playerid, g_PropertyData[ i ][p_PropertyVwid_OUT] );
            SetPlayerInterior( playerid, g_PropertyData[ i ][p_PropertyIntid_OUT] );
            
            d_ac_SetPlayerPos( playerid, g_PropertyData[ i ][p_PropertyEntrance_X], g_PropertyData[ i ][p_PropertyEntrance_Y], \
            g_PropertyData[ i ][p_PropertyEntrance_Z] );
            
            SetPlayerFacingAngle( playerid, g_PropertyData[ i ][p_PropertyEntrance_A] );
            SetCameraBehindPlayer( playerid );

            TogglePlayerControllable(playerid,false);

            _PropertyLoadingTime(playerid);

            #if defined detutils_debug
            printf("[samp-detutils]: (debug) - nearintcallback :: Player exited the interior with id : %i.", i);
            #endif

            PropertyActionID = 1;
            /*format(PropertyFuncName,sizeof(PropertyFuncName),
                "OnPropertyActionPerformed",g_PropertyData[ i ][p_PropertyName]);*/

            if(funcidx("OnPropertyActionPerformed") != -1) CallLocalFunction("OnPropertyActionPerformed","iii",playerid,i,PropertyActionID);

            g_PlayerProperty[ playerid ] = -1;
            
            if(funcidx("OnPlayerExitProperty") != -1)
                CallLocalFunction("OnPlayerExitProperty", "i", playerid);
        }
    }
    return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
    ClearPlayerPropertyData(playerid);
    if(funcidx("aint_ALS_OnPlayerDisconnect") != -1)
    {
        CallLocalFunction("aint_ALS_OnPlayerDisconnect", "ii", playerid, reason);
    }
    #if defined detutils_debug
    printf("[samp-detutils]: (debug) - playerdiscc :: Player interior data cleared.");
    #endif
    return 1;
}

#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif

#define OnPlayerDisconnect aint_ALS_OnPlayerDisconnect

forward aint_ALS_OnPlayerDisconnect(playerid, reason);

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if( newkeys == KEY_SECONDARY_ATTACK)
    {
        if(GetPlayerState(playerid) == 1)
        {
            _OnPlayerNearEntrance(playerid);
            #if defined detutils_debug
            printf("[samp-detutils]: (debug) - keystate :: OnPlayerNearEntrance called successfully.");
            #endif
        }

        if( GetPlayerSpecialAction(playerid ) != SPECIAL_ACTION_NONE && \
        GetPlayerSpecialAction(playerid ) != SPECIAL_ACTION_CUFFED)
        {
            SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
        }
    }
    if(funcidx("aint_ALS_OnPlayerKeyStateChange") != -1)
    {
        CallLocalFunction("aint_ALS_OnPlayerKeyStateChange", "iii", playerid, newkeys, oldkeys);
    }
    return 1;
}

#if defined _ALS_OnPlayerKeyStateChange
    #undef OnPlayerKeyStateChange
#else
    #define _ALS_OnPlayerKeyStateChange
#endif

#define OnPlayerKeyStateChange aint_ALS_OnPlayerKeyStateChange

forward aint_ALS_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);

DETUTILS_DECL__ func:ResetUpdateBool(playerid)
{
    #if defined detutils_debug
    printf("[samp-detutils]: (debug) - keystate :: util function 'ResetUpdateBool' called successfully.");
    #endif

    _EntranceTitleShown[playerid] = 0;
    return 1;
}

public OnPlayerUpdate(playerid)
{
    for(new i; i < MAX_PROPERTIES; i++)
    {
        if( IsPlayerInRangeOfPoint( playerid, 1.5, g_PropertyData[ i ][p_PropertyEntrance_X], \
        g_PropertyData[ i ][p_PropertyEntrance_Y], g_PropertyData[ i ][p_PropertyEntrance_Z]) \
        && GetPlayerVirtualWorld( playerid ) == g_PropertyData[ i ][p_PropertyVwid_OUT] \
        && !IsPlayerInAnyVehicle( playerid ) && _EntranceTitleShown[playerid] != 1)
        {
            new PropertyName[100];
            format(PropertyName,sizeof(PropertyName),DETUTILS_TEXT_PROPERTY_NAME,g_PropertyData[ i ][ p_PropertyName ]);
            
            _Announcement(playerid, PropertyName);
            #if defined detutils_debug
            printf("[samp-detutils]: (debug) - playerupdate :: Gametext shown.");
            #endif
            _EntranceTitleShown[playerid] = 1;
            _SetTimer("ResetUpdateBool", 10000, false, "i", playerid);
        }
    }
    if(funcidx("a_props_playerupdate") != -1)
    {
        CallLocalFunction("a_props_playerupdate", "i", playerid);
    }
    return 1;
}

#if defined _ALS_OnPlayerUpdate
    #undef OnPlayerUpdate
#else
    #define _ALS_OnPlayerUpdate
#endif

#define OnPlayerUpdate a_props_playerupdate

forward a_props_playerupdate(playerid);

/*

                                                                                      
88b           d88              88                         db         88888888ba   88  
888b         d888              ""                        d88b        88      "8b  88  
88`8b       d8'88                                       d8'`8b       88      ,8P  88  
88 `8b     d8' 88  ,adPPYYba,  88  8b,dPPYba,          d8'  `8b      88aaaaaa8P'  88  
88  `8b   d8'  88  ""     `Y8  88  88P'   `"8a        d8YaaaaY8b     88""""""'    88  
88   `8b d8'   88  ,adPPPPP88  88  88       88       d8""""""""8b    88           88  
88    `888'    88  88,    ,88  88  88       88      d8'        `8b   88           88  
88     `8'     88  `"8bbdP"Y8  88  88       88     d8'          `8b  88           88  
                                                                                      
                                                                                      

*/

stock Create3DLabelAndPickup( 
        const _LabelString[], 
        Float:_xPosition, 
        Float:_yPosition, 
        Float:_zPosition,  
        _VirtualWorld, 
        _Pickupid, 
        Float:_LabelRadius ) 
{
    if(IsValidString(_LabelString))
        return 0;

    #if defined detutils_debug
    printf("[samp-detutils]: (debug) - c3dladp :: String checking finished.");
    #endif

    if(_VirtualWorld < 0)
        return 0;

    #if defined detutils_debug
    printf("[samp-detutils]: (debug) - c3dladp :: Virtual world validity checked.");
    #endif

    if(_LabelRadius <= 0)
        return 0;

    #if defined detutils_debug
    printf("[samp-detutils]: (debug) - c3dladp :: Label visibility radius check finished.");
    #endif

    if(_Pickupid < 0)
        return 0;

    #if defined detutils_debug
    printf("[samp-detutils]: (debug) - c3dladp :: Pickup id validity checked.");
    #endif

    Create3DTextLabel(
                            _LabelString, 
                            0xffffffff, 
                            _xPosition, 
                            _yPosition, 
                            _zPosition, 
                            _LabelRadius, 
                            _VirtualWorld);

    #if defined detutils_debug
    printf("[samp-detutils]: (debug) - c3dladp :: Created 3d label successfully.");
    #endif

    CreatePickup(
                    _Pickupid, 
                    1, 
                    _xPosition, 
                    _yPosition, 
                    _zPosition, 
                    _VirtualWorld);

    #if defined detutils_debug
    printf("[samp-detutils]: (debug) - c3dladp :: Created dynamic pickup successfully.");
    #endif

    #if defined detutils_debug
    printf("[samp-detutils]: (debug) - Operation completed successfully. Error code: ERRNULL");
    #endif

    return 1;
}

stock CreatePropertyEntrance(
        const PropertyName[], 
        Float:_xEntrance, 
        Float:_yEntrance, 
        Float:_zEntrance,  
        _xInteriorid,  
        _xVirtualWorldid,
        bool:Labels = false,
        Interior = INTERIOR_CUSTOM,
        Float:_xExit = 0.000, 
        Float:_yExit = 0.000, 
        Float:_zExit = 0.000, 
        _eInteriorid = 0,
        _eVirtualWorldid = 0)
{
    if(g_PropertyID == MAX_PROPERTIES)
    {
        #if defined detutils_debug
        for(new i = 0; i < MAX_PLAYERS; i++)
        {
            if(i != INVALID_PLAYER_ID)
            {
                d_SendWarningMessage(i, DETUTILS_TEXT_MAX_PROPS_REACH);
            }
        }
        #endif
        return 0;
    }
    printf("[samp-detutils]: (debug) - Property '%s' successfully created, entrance is set to: %f, %f, %f.",
            PropertyName,
            Float:_xEntrance, 
            Float:_yEntrance, 
            Float:_zEntrance);
    #if defined detutils_debug
    printf("[samp-detutils]: (debug) - Property created.");
    #endif

    g_PropertyID++;

    #if defined detutils_debug
    printf("[samp-detutils]: (debug) - Property id assigned.");
    #endif

    strmid(g_PropertyData[g_PropertyID][p_PropertyName], 
            PropertyName, 
            0, 
            strlen(PropertyName), 
            255);

    #if defined detutils_debug
    printf("[samp-detutils]: (debug) - Saved property name.");
    #endif

    printf("[samp-detutils]: (debug) - Property '%s' has an id %d.",PropertyName,g_PropertyID);

    g_PropertyData[g_PropertyID][p_PropertyEntrance_X] = _xEntrance;
    g_PropertyData[g_PropertyID][p_PropertyEntrance_Y] = _yEntrance;
    g_PropertyData[g_PropertyID][p_PropertyEntrance_Z] = _zEntrance;

    #if defined detutils_debug
    printf("[samp-detutils]: (debug) - Entrance coordinates assigned to property data.");
    #endif

    g_PropertyData[g_PropertyID][p_PropertyExit_X] = _xExit;
    g_PropertyData[g_PropertyID][p_PropertyExit_Y] = _yExit;
    g_PropertyData[g_PropertyID][p_PropertyExit_Z] = _zExit;

    #if defined detutils_debug
    printf("[samp-detutils]: (debug) - Exit coordinates assigned to property data.");
    #endif

    g_PropertyData[g_PropertyID][p_PropertyIntid] = _eInteriorid;
    g_PropertyData[g_PropertyID][p_PropertyIntid_OUT] = _xInteriorid;
    g_PropertyData[g_PropertyID][p_PropertyVwid] = _eVirtualWorldid;
    g_PropertyData[g_PropertyID][p_PropertyVwid_OUT] = _xVirtualWorldid;

    #if defined detutils_debug
    printf("[samp-detutils]: (debug) - Virtual world IDs and interior IDs assigned to property data.");
    #endif

    if(Interior != INTERIOR_CUSTOM)
    {
        #if defined detutils_debug
        printf("[samp-detutils]: (debug) - Assigned interior map id to property id : %i", Interior);
        #endif

        if(Interior == INTERIOR_MARKET_247_1)
        {
            g_PropertyData[g_PropertyID][p_PropertyIntid] = 17;
            g_PropertyData[g_PropertyID][p_PropertyVwid] = 17;
            _eInteriorid = 17;
            _eVirtualWorldid = 17;

            g_PropertyData[g_PropertyID][p_PropertyExit_X] = -25.884498;
            g_PropertyData[g_PropertyID][p_PropertyExit_Y] = -185.868988;
            g_PropertyData[g_PropertyID][p_PropertyExit_Z] = 1003.546875;
        }
        if(Interior == INTERIOR_MARKET_247_2)
        {
            DETUTILS_PROP_PARAMS__(6.091179,-29.271898,1003.549438,10);
        }
        if(Interior == INTERIOR_MARKET_247_3)
        {
            DETUTILS_PROP_PARAMS__(-30.946699,-89.609596,1003.546875,18);
        }
        if(Interior == INTERIOR_MARKET_247_4)
        {
            DETUTILS_PROP_PARAMS__(-25.132598,-139.066986,1003.546875,16);
        }
        if(Interior == INTERIOR_MARKET_247_5)
        {
            DETUTILS_PROP_PARAMS__(-27.312299,-29.277599,1003.557250,4);
        }
        if(Interior == INTERIOR_MARKET_247_6)
        {
            DETUTILS_PROP_PARAMS__(-26.691598,-55.714897,1003.546875,6);
        }
    }

    if(Labels == true)
    {
        new _EntranceLabelString[300];
        #if defined detutils_debug
        printf("[samp-detutils]: (debug) - Empty string declared.");
        #endif
        format(_EntranceLabelString,sizeof(_EntranceLabelString),"{ffffff}[ %s ]\n{ffffff}Enter here!",PropertyName);
        #if defined detutils_debug
        printf("[samp-detutils]: (debug) - Formatted the 3D label string successfully.");
        #endif
        Create3DLabelAndPickup(_EntranceLabelString, _xEntrance,_yEntrance,_zEntrance, _xVirtualWorldid, 19133, 3.0);
        #if defined detutils_debug
        printf("[samp-detutils]: (debug) - Streamer :: Created 3D label and pickup on entrance.");
        #endif
        Create3DLabelAndPickup("Exit here!", g_PropertyData[g_PropertyID][p_PropertyExit_X],
            g_PropertyData[g_PropertyID][p_PropertyExit_Y], g_PropertyData[g_PropertyID][p_PropertyExit_Z], _eVirtualWorldid, 19133, 3.0);
        #if defined detutils_debug
        printf("[samp-detutils]: (debug) - Streamer :: Created 3D label and pickup on exit.");
        #endif
    }

    #if defined detutils_debug
    printf("[samp-detutils]: (debug) - Internal property data assigned.");
    #endif

    if(funcidx("OnPropertyCreated") != -1)
    {
        CallLocalFunction("OnPropertyCreated", "i", g_PropertyID);
    }

    #if defined detutils_debug
    printf("[samp-detutils]: (debug) - OnPropertyCreated(propertyid) called.");
    #endif

    return 1;
}

// DELETED FUNCTION, MAKES NO SENSE.

#if defined detutils_complete_error_boom

stock CreateCustomInterior(
        const PropertyName[], 
        Float:_xEntrance, 
        Float:_yEntrance, 
        Float:_zEntrance, 
        Float:_xExit, 
        Float:_yExit, 
        Float:_zExit, 
        _eInteriorid, 
        _xInteriorid, 
        _eVirtualWorldid, 
        _xVirtualWorldid)
{
    if(g_PropertyID == MAX_PROPERTIES)
    {
        #if defined detutils_debug
        for(new i = 0; i < MAX_PLAYERS; i++)
        {
            if(i != INVALID_PLAYER_ID)
            {
                d_SendWarningMessage(i, "Maximum number of created interiors reached.");
            }
        }
        #endif
        return 0;
    }
    printf("[samp-detutils]: (debug) - Interior '%s' successfully created, entrance is set to: %f, %f, %f.",
            PropertyName,
            Float:_xEntrance, 
            Float:_yEntrance, 
            Float:_zEntrance);

    #if defined detutils_debug
    printf("[samp-detutils]: (debug) - Interior created.");
    #endif

    g_PropertyID++;

    strmid(g_PropertyData[g_PropertyID][p_PropertyName], 
            PropertyName, 
            0, 
            strlen(PropertyName), 
            255);

    #if defined detutils_debug
    printf("[samp-detutils]: (debug) - Saved interior name.");
    #endif

    printf("[samp-detutils]: (debug) - Interior '%s' has an id %d.",PropertyName,g_PropertyID);
    #if defined detutils_debug
    printf("[samp-detutils]: (debug) - Interior id assigned.");
    #endif
    g_PropertyData[g_PropertyID][p_PropertyEntrance_X] = _xEntrance;
    g_PropertyData[g_PropertyID][p_PropertyEntrance_Y] = _yEntrance;
    g_PropertyData[g_PropertyID][p_PropertyEntrance_Z] = _zEntrance;

    #if defined detutils_debug
    printf("[samp-detutils]: (debug) - Entrance coordinates assigned to interior data.");
    #endif

    g_PropertyData[g_PropertyID][p_PropertyExit_X] = _xExit;
    g_PropertyData[g_PropertyID][p_PropertyExit_Y] = _yExit;
    g_PropertyData[g_PropertyID][p_PropertyExit_Z] = _zExit;

    #if defined detutils_debug
    printf("[samp-detutils]: (debug) - Exit coordinates assigned to interior data.");
    #endif

    g_PropertyData[g_PropertyID][p_PropertyIntid] = _eInteriorid;
    g_PropertyData[g_PropertyID][p_PropertyIntid_OUT] = _xInteriorid;
    g_PropertyData[g_PropertyID][p_PropertyVwid] = _eVirtualWorldid;
    g_PropertyData[g_PropertyID][p_PropertyVwid_OUT] = _xVirtualWorldid;

    #if defined detutils_debug
    printf("[samp-detutils]: (debug) - Virtual world and interiorids assigned to custom interior.");
    #endif

    #if defined detutils_debug
    printf("[samp-detutils]: (debug) - Internal interior data assigned.");
    #endif

    if(funcidx("OnPropertyCreated") != -1)
    {
        CallLocalFunction("OnPropertyCreated", "i", g_PropertyID);
    }

    #if defined detutils_debug
    printf("[samp-detutils]: (debug) - OnPropertyCreated(customintid) called.");
    #endif

    return 1;
}

#endif

// NORMAL CODE, MAKES SENSE:

stock bool:IsPlayerInProperty(playerid)
{
    if(g_PlayerProperty[playerid] == -1) return false;
    else return true;
}

stock ClearPlayerPropertyData(playerid)
{
    #if defined detutils_debug
    printf("[samp-detutils]: (debug) - ClearPlayerPropertyData called successfully.");
    #endif
    g_PlayerProperty[playerid] = -1;
    #if defined detutils_debug
    printf("[samp-detutils]: (debug) - ClearPlayerPropertyData action successfully performed.");
    #endif
    return 1;
}

stock GetLastPropertyIDUsed()
{
    #if defined detutils_debug
    printf("[samp-detutils]: (debug) - GetLastPropertyIDUsed successfully called.");
    #endif
    return g_PropertyID;
}

stock SetPropertyEntranceCustomAngles(
        const _PropertyID,
        Float:_aAngle,
        Float:_bAngle)
{
    #if defined detutils_debug
    printf("[samp-detutils]: (debug) - Successfully set interior (intid : %i) angles to %f-%f.", 
        _PropertyID, _aAngle, _bAngle);
    #endif
    g_PropertyData[_PropertyID][p_PropertyEntrance_A] = _aAngle;
    g_PropertyData[_PropertyID][p_PropertyExit_A] = _bAngle;
    return 1;
}

/*

                                                                                                 
88888888888                                                          db         88888888ba   88  
88                          ,d                                      d88b        88      "8b  88  
88                          88                                     d8'`8b       88      ,8P  88  
88aaaaa      8b,     ,d8  MM88MMM  8b,dPPYba,  ,adPPYYba,         d8'  `8b      88aaaaaa8P'  88  
88"""""       `Y8, ,8P'     88     88P'   "Y8  ""     `Y8        d8YaaaaY8b     88""""""'    88  
88              )888(       88     88          ,adPPPPP88       d8""""""""8b    88           88  
88            ,d8" "8b,     88,    88          88,    ,88      d8'        `8b   88           88  
88888888888  8P'     `Y8    "Y888  88          `"8bbdP"Y8     d8'          `8b  88           88  
                                                                                                 
                                                                                                 
*/

// Bug fix:

public OnPlayerSpawn(playerid)
{
    TogglePlayerControllable(playerid, 1);
    if(funcidx("a_props_playerspawn") != -1)
    {
        CallLocalFunction("a_props_playerspawn", "i", playerid);
    }
    return 1;
}

#if defined _ALS_OnPlayerSpawn
    #undef OnPlayerSpawn
#else
    #define _ALS_OnPlayerSpawn
#endif

#define OnPlayerSpawn a_props_playerspawn

forward a_props_playerspawn(playerid);

//////////////////////////////////////////////////

//#if !defined DETUTILS_COMPATIBILITY_MODE

forward OnPropertyActionPerformed(playerid, propertyid, actionid);

stock GetPropertyName(PropertyID)
{
    //#if defined detutils_complete_error_boom
    new f_PropertyName[256];

    strcat(f_PropertyName, g_PropertyData[PropertyID][p_PropertyName]);

    return f_PropertyName;
    //#endif
    //return g_PropertyData[PropertyID][p_PropertyName];
}

stock GetPropertyIDByName(const PropertyName[])
{
    for(new PropertyCount = 1; 
        PropertyCount < MAX_PROPERTIES; 
        PropertyCount++)
    {
        if(!strcmp(g_PropertyData[PropertyCount][p_PropertyName], PropertyName))
        {
            return PropertyCount;
        }
    }
    return 0;
}

#if defined detutils_complete_error_boom

#define public%3OnPropertyActionPerformed%2(%0,%1) \
    forward int_%0(%1); \
    public int_%0(%1)


#endif

#if defined detutils_complete_error_boom

#define INTERIOR__%0(%1) \
    forward int_%0(%1,_Actionid); \
    public int_%0(%1,_Actionid)

#endif

stock SetPlayerProperty(_p_Playerid, pg_PropertyID)
{
    if(pg_PropertyID < -1)
        return 0;

    #if defined detutils_debug
    printf("[samp-detutils]: (debug) - Successfully set player's (id : %i) property id to %i.", _p_Playerid, pg_PropertyID);
    #endif

    g_PlayerProperty[_p_Playerid] = pg_PropertyID;
    return 1;
}

#if defined detutils_complete_error_boom

#define IsInteriorActionPerformed(%0) \
    (_Actionid == %0) >>> data * plus / Check;

#endif

static void:AnnounceLibraryLoaded_int()
{
    LoadDETUtilsScript(DETUTILS_SCRIPT_PROPERTIES);
}

#if !defined FILTERSCRIPT

main()
{
    #if defined detutils_debug
    SaveLogIntoFile("detutils_properties.log", "[samp-detutils]: (debug) - Loading...");
    SaveLogIntoFile("detutils_properties.log", "[samp-detutils]: (debug) - Loaded.");
    #endif
    AnnounceLibraryLoaded_int();
    a_int_main();
}

#if defined _ALS_main
    #undef main
#else
    #define _ALS_main
#endif

#define main a_int_main

#else

public OnFilterScriptInit()
{
    _CreateAnncTextDraws();
    #if defined detutils_debug
    SaveLogIntoFile("detutils_properties.log", "[samp-detutils]: (debug) - Loading...");
    SaveLogIntoFile("detutils_properties.log", "[samp-detutils]: (debug) - Loaded.");
    #endif
    AnnounceLibraryLoaded_int();

    if(funcidx("a_props_filterscriptinit") != -1)
    {
        CallLocalFunction("a_props_filterscriptinit", "");
    }
    return 1;
}

#if defined _ALS_OnFilterScriptInit
    #undef OnFilterScriptInit
#else
    #define _ALS_OnFilterScriptInit
#endif

#define OnFilterScriptInit a_props_filterscriptinit

forward a_props_filterscriptinit();

#endif