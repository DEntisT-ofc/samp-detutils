/*
* 
* DETUtils
*
* Since 2019., DEntisT
*
*
*
* - Library which provides
*   the new ASCII char defines.
*
*
*/

#define CHARACTER_SPACE 32
#define CHARACTER_EXCLAMATION_MARK 33
#define CHARACTER_DOUBLE_QUOTES 34
#define CHARACTER_NUMBER 35
#define CHARACTER_DOLLAR 36
#define CHARACTER_PERCENT_SIGN 37
#define CHARACTER_AMPERSAND 38
#define CHARACTER_SINGLE_QUOTE 39
#define CHARACTER_PARENTHESIS_OPEN 40
#define CHARACTER_PARENTHESIS_CLOSED 41
#define CHARACTER_ASTERISK 42
#define CHARACTER_PLUS 43
#define CHARACTER_COMMA 44
#define CHARACTER_HYPHEN 45
#define CHARACTER_FULLSTOP 46
#define CHARACTER_DOT CHARACTER_FULLSTOP
#define CHARACTER_PERIOD CHARACTER_FULLSTOP
#define CHARACTER_SLASH 47
#define CHARACTER_ZERO 48
#define CHARACTER_ONE 49
#define CHARACTER_TWO 50
#define CHARACTER_THREE 51
#define CHARACTER_FOUR 52
#define CHARACTER_FIVE 53
#define CHARACTER_SIX 54
#define CHARACTER_SEVEN 55
#define CHARACTER_EIGHT 56
#define CHARACTER_NINE 57
#define CHARACTER_COLON 58
#define CHARACTER_SEMICOLON 59
#define CHARACTER_OPEN_ANGLED_BRACKET 60
#define CHARACTER_EQUALS 61
#define CHARACTER_CLOSED_ANGLED_BRACKET 62
#define CHARACTER_QUESTION_MARK 63
#define CHARACTER_AT_SYMBOL 64
#define CHARACTER_UPPERCASE_A 65
#define CHARACTER_UPPERCASE_B 66
#define CHARACTER_UPPERCASE_C 67
#define CHARACTER_UPPERCASE_D 68
#define CHARACTER_UPPERCASE_E 69
#define CHARACTER_UPPERCASE_F 70
#define CHARACTER_UPPERCASE_G 71
#define CHARACTER_UPPERCASE_H 72
#define CHARACTER_UPPERCASE_I 73
#define CHARACTER_UPPERCASE_J 74
#define CHARACTER_UPPERCASE_K 75
#define CHARACTER_UPPERCASE_L 76
#define CHARACTER_UPPERCASE_M 77
#define CHARACTER_UPPERCASE_N 78
#define CHARACTER_UPPERCASE_O 79
#define CHARACTER_UPPERCASE_P 80
#define CHARACTER_UPPERCoASE_Q 81
#define CHARACTER_UPPERCASE_R 82
#define CHARACTER_UPPERCASE_S 83
#define CHARACTER_UPPERCASE_T 84
#define CHARACTER_UPPERCASE_U 85
#define CHARACTER_UPPERCASE_V 86
#define CHARACTER_UPPERCASE_W 87
#define CHARACTER_UPPERCASE_X 88
#define CHARACTER_UPPERCASE_Y 89
#define CHARACTER_UPPERCASE_Z 90
#define CHARACTER_OPENING_BRACKET 91
#define CHARACTER_BACKSLASH 92
#define CHARACTER_CLOSING_BRACKET 93
#define CHARACTER_CARET 94
#define CHARACTER_CIRCUMFLEX CHARACTER_CARET
#define CHARACTER_UNDERSCORE 95
#define CHARACTER_GRAVE_ACCENT 96
#define CHARACTER_LOWERCASE_A 97
#define CHARACTER_LOWERCASE_B 98
#define CHARACTER_LOWERCASE_C 99
#define CHARACTER_LOWERCASE_D 100
#define CHARACTER_LOWERCASE_E 101
#define CHARACTER_LOWERCASE_F 102
#define CHARACTER_LOWERCASE_G 103
#define CHARACTER_LOWERCASE_H 104
#define CHARACTER_LOWERCASE_I 105
#define CHARACTER_LOWERCASE_J 106
#define CHARACTER_LOWERCASE_K 107
#define CHARACTER_LOWERCASE_L 108
#define CHARACTER_LOWERCASE_M 109
#define CHARACTER_LOWERCASE_N 110
#define CHARACTER_LOWERCASE_O 111
#define CHARACTER_LOWERCASE_P 112
#define CHARACTER_LOWERCASE_Q 113
#define CHARACTER_LOWERCASE_R 114
#define CHARACTER_LOWERCASE_S 115
#define CHARACTER_LOWERCASE_T 116
#define CHARACTER_LOWERCASE_U 117
#define CHARACTER_LOWERCASE_V 118
#define CHARACTER_LOWERCASE_W 119
#define CHARACTER_LOWERCASE_X 120
#define CHARACTER_LOWERCASE_Y 121
#define CHARACTER_LOWERCASE_Z 122
#define CHARACTER_OPENING_BRACE 123
#define CHARACTER_VERTICAL_BAR 124
#define CHARACTER_CLOSING_BRACE 125
#define CHARACTER_EQUIVALENCY_SIGN 126
#define CHARACTER_TILDE CHARACTER_EQUIVALENCY_SIGN
#define CHARACTER_DELETE 127

// API:

#define CHARACTER_TYPE_UNKNOWN -1
#define CHARACTER_TYPE_CONTROL 0
#define CHARACTER_TYPE_PRINTABLE 1
#define CHARACTER_TYPE_EXTENDED 2

stock GetCharacterType(charid)
{
	switch(charid)
	{
		case 0 .. 31: return CHARACTER_TYPE_CONTROL;
		case 32 .. 127: return CHARACTER_TYPE_PRINTABLE;
		case 128 .. 255: return CHARACTER_TYPE_EXTENDED;
	}
	return CHARACTER_TYPE_UNKNOWN;
}