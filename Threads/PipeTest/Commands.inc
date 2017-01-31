// ----------------------------------------------------------------------
//
// include -  Commands.inc
//
// contents - Commands between the Chessmaster Application and the engines
//
// ----------------------------------------------------------------------

#ifndef		COMMANDS_INC
#define		COMMANDS_INC

#ifdef __cplusplus
 extern "C" {
#endif

#define		MAX_COMMAND_LEN			24


typedef enum
{
    // messages from the app to the engine
    FIRST_CMD_TO_ENGINE = 0,

    CMDE_STATUS = FIRST_CMD_TO_ENGINE,
    CMDE_REQUEST_VERSION,
    CMDE_TERMINATE,
    CMDE_SET_TRANSPOSITION_SIZE,
    CMDE_INITIALIZE_BOARD,
    CMDE_ADD_MOVES,
    CMDE_YOUR_TURN,
    CMDE_BEST_LINE,
    CMDE_NEXT_BEST_LINE,
    CMDE_CURRENT_LINE,
    CMDE_QUICK_HINT,
    CMDE_LAST_VALUATION,
    CMDE_POSITIONS_SEEN,
    CMDE_FORCE_MOVE,
    CMDE_ABORT_SEARCH,
    CMDE_SET_PERSONALITY,
    CMDE_SOLVE_FOR_MATE,
    CMDE_SEARCH_BEST_LINE,
    CMDE_SEARCH_FIXED_PLY,
    CMDE_SEARCH_PERSONALITY_LINE,

    LAST_CMD_TO_ENGINE = CMDE_SEARCH_PERSONALITY_LINE,

    // messages from the engine to the app
    FIRST_CMD_TO_APP,

	// winboard commands
    CMDA_VERIFY_STATUS = FIRST_CMD_TO_APP,
	CMDA_ILLEGAL_MOVE,
	CMDA_ERROR,
	CMDA_MOVE,
	CMDA_DRAW,
	CMDA_RESIGN,
	CMDA_RESULT,
	CMDA_TELLUSER,
	CMDA_TELLUSERERROR,
	CMDA_ASKUSER,
	CMDA_TELLICS,
	
    CMDA_VERSION_INFO,
    CMDA_ENGINE_MOVE,
    CMDA_MOVE_LINE,
    CMDA_THINKING,    
    CMDA_SEARCH_MOVE_RESULTS,
    CMDA_SEARCH_MATE_RESULTS,
    CMDA_REQUEST_PLAYER,
    CMDA_REQUEST_TIME,
    CMDA_REQUEST_TIME_CONTROL,
    CMDA_REQUEST_COLOR_TO_MOVE,
    CMDA_REQUEST_PERSONALITY,

    LAST_CMD_TO_APP = CMDA_REQUEST_PERSONALITY,

    NUM_APP_ENGINE_COMMANDS,

	CMD_NONE

} eENGINE_COMMAND;

#define     COMMENT_BEGIN_CHAR      '{'
#define     COMMENT_END_CHAR        '}'
#define     OPTIONAL_BEGIN_CHAR     '('
#define     OPTIONAL_END_CHAR       ')'

#if 0				
typedef struct
{
	eENGINE_COMMAND	command;
    char		    szCommand[MAX_COMMAND_LEN];
	char			szParam1;
	char			szParam2;

} _COMMAND_STRUCT;

_COMMAND_STRUCT	CommandsFromEngine[] = 
{
	{	CMDA_ILLEGAL_MOVE,  "Illegal move",			MOVE            "",         },
	{	CMDA_ERROR,         "Error",				"(ERRORTYPE)",	COMMAND,    },
	{	CMDA_MOVE,          "move",					MOVE,           "",         },
	{	CMDA_RESIGN,        "resign",               "",             "",         },
	{	CMDA_DRAW,          "offer draw",           "",             "",         },
	{	CMDA_RESULT,        "0-1",					"",             "",         },
    {	CMDA_RESULT,        "1-0",					"",             "",         },
    {	CMDA_RESULT,        "1/2-1/2"				"",             "",         },
	{	CMDA_TELLUSER,      "telluser",				"",             "",         },
	{	CMDA_TELLUSERERROR, "tellusererror",		"",             "",         },
	{	CMDA_ASKUSER,       "askuser",				REPTAG,			"",         },
	{	CMDA_TELLICS,       "tellics",				"",				"",         },
    {	CMDA_THINKING,      "",				        "",				"",         },
	                                                                            
	{	NO_COMMAND,		    "",                     "",             "",         },
};

_COMMAND_STRUCT	ImpreciseCommandsFromEngine[] = 
{
	{	CMDA_MOVE,          "",         			"...",			MOVE,       },
    {   CMDA_THINKING,      "",                     "",             "",         },
};

_COMMAND_STRUCT CommandsFromApplication[] = 
{
    {	CMDE_STATUS,                    "status",   "",     "",     },
    {	CMDE_REQUEST_VERSION,           "version",  "",     "",     },
    {	CMDE_TERMINATE,                 "quit",		"",     "",     },
    {	CMDE_SET_TRANSPOSITION_SIZE,    "ttable",   "",     "",     },
    {	CMDE_INITIALIZE_BOARD,          "board",    "",     "",     },
    {	CMDE_ADD_MOVES,                 "moves",    "",     "",     },
    {	CMDE_YOUR_TURN,                 "",    "",     "",     },
    {	CMDE_BEST_LINE,                 "",    "",     "",     },
    {	CMDE_NEXT_BEST_LINE,            "",    "",     "",     },
    {	CMDE_CURRENT_LINE,              "",    "",     "",     },
    {	CMDE_QUICK_HINT,                "",    "",     "",     },
    {	CMDE_LAST_VALUATION,            "",    "",     "",     },
    {	CMDE_POSITIONS_SEEN,            "",    "",     "",     },
    {	CMDE_FORCE_MOVE,                "",    "",     "",     },
    {	CMDE_ABORT_SEARCH,              "",    "",     "",     },
    {	CMDE_SET_PERSONALITY,           "",    "",     "",     },
    {	CMDE_SOLVE_FOR_MATE,            "",    "",     "",     },
    {	CMDE_SEARCH_BEST_LINE,          "",    "",     "",     },
    {	CMDE_SEARCH_FIXED_PLY,          "",    "",     "",     },
    {	CMDE_SEARCH_PERSONALITY_LINE,   "",    "",     "",     },
                                     
};
#endif


#ifdef __cplusplus
} 
#endif

#endif	// COMMANDS_INC