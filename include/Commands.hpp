 enum Reply
{
	RPL_NONE             = 0,
	// Initial
	RPL_WELCOME			= 001,                  // :Welcome to the Internet Relay Network <nickname>
	RPL_YOURHOST		= 002,                  // :Your host is <server>, running version <ver>
	RPL_CREATED			= 003,                  // :This server was created <datetime>
	RPL_MYINFO			= 004,                  // <server> <ver> <usermode> <chanmode>
	RPL_BOUNCE			= 005,                  // :map
	RPL_MAP				= 006,
	RPL_MAPEND			= 007,                  // :End of /MAP
	RPL_MOTDSTART		= 375,                  // :- server Message of the Day
	RPL_MOTD			= 372,                  // :- <info>
	RPL_MOTDALT			= 377,                  // :- <info>                                                                        (some)
	RPL_MOTDALT2		= 378,                  // :- <info>                                                                        (some)
	RPL_MOTDENF			= 376,                  // :End of /MOTD command.
	RPL_UMODEIS			= 221,                  // <mode>

	// IsOn/UserHost
	RPL_USERHOST		= 302,                  // :userhosts
	RPL_ISON			= 303,                  // :nicknames

	// Away
	RPL_AWAY			= 301,                  // <nick> :away
	RPL_UnAway			= 305,                  // :You are no longer marked as being away
	RPL_NowAway			= 306,                  // :You have been marked as being away

	// WHOIS/WHOWAS
	RPL_WhoisHelper      = 310,                  // <nick> :looks very helpful                                                       DALNET
	RPL_WhoIsUser        = 311,                  // <nick> <username> <address> * :<info>
	RPL_WhoIsServer      = 312,                  // <nick> <server> :<info>
	RPL_WhoIsOperator    = 313,                  // <nick> :is an IRC Operator
	RPL_WhoIsIdle        = 317,                  // <nick> <seconds> <signon> :<info>
	RPL_EndOfWhois       = 318,                  // <request> :End of /WHOIS list.
	RPL_WhoIsChannels    = 319,                  // <nick> :<channels>
	RPL_WhoWasUser       = 314,                  // <nick> <username> <address> * :<info>
	RPL_EndOfWhoWas      = 369,                  // <request> :End of WHOWAS
	RPL_WhoReply         = 352,                  // <channel> <username> <address> <server> <nick> <flags> :<hops> <info>
	RPL_EndOfWho         = 315,                  // <request> :End of /WHO list.
	RPL_UserIPs          = 307,                  // :userips                                                                         UNDERNET
	RPL_UserIP           = 340,                  // <nick> :<nickname>=+<user>@<IP.address>                                          UNDERNET

	// List
	RPL_ListStart        = 321,                  // Channel :Users Name
	RPL_List             = 322,                  // <channel> <users> :<topic>
	RPL_ListEnd          = 323,                  // :End of /LIST
	RPL_Links            = 364,                  // <server> <hub> :<hops> <info>
	RPL_EndOfLinks       = 365,                  // <mask> :End of /LINKS list.

	// Post-Channel Join
	RPL_UniqOpIs         = 325,
	RPL_ChannelModeIs    = 324,                  // <channel> <mode>
	RPL_ChannelUrl       = 328,                  // <channel> :url                                                                   DALNET
	RPL_ChannelCreated   = 329,                  // <channel> <time>
	RPL_NoTopic          = 331,                  // <channel> :No topic is set.
	RPL_Topic            = 332,                  // <channel> :<topic>
	RPL_TopicSetBy       = 333,                  // <channel> <nickname> <time>
	RPL_NamReply         = 353,                  // = <channel> :<names>
	RPL_EndOfNames       = 366,                  // <channel> :End of /NAMES list.

	// Invitational
	RPL_Inviting         = 341,                  // <nick> <channel>
	RPL_Summoning        = 342,

	// Channel Lists
	RPL_InviteList       = 346,                  // <channel> <invite> <nick> <time>                                                 IRCNET
	RPL_EndOfInviteList  = 357,                  // <channel> :End of Channel Invite List                                            IRCNET
	RPL_ExceptList       = 348,                  // <channel> <exception> <nick> <time>                                              IRCNET
	RPL_EndOfExceptList  = 349,                  // <channel> :End of Channel Exception List                                         IRCNET
	RPL_BanList          = 367,                  // <channel> <ban> <nick> <time>
	RPL_EndOfBanList     = 368,                  // <channel> :End of Channel Ban List


	// server/misc
	RPL_Version          = 351,                  // <version>.<debug> <server> :<info>
	RPL_Info             = 371,                  // :<info>
	RPL_EndOfInfo        = 374,                  // :End of /INFO list.
	RPL_YoureOper        = 381,                  // :You are now an IRC Operator
	RPL_Rehashing        = 382,                  // <file> :Rehashing
	RPL_YoureService     = 383,
	RPL_Time             = 391,                  // <server> :<time>
	RPL_UsersStart       = 392,
	RPL_Users            = 393,
	RPL_EndOfUsers       = 394,
	RPL_NoUsers          = 395,
	RPL_ServList         = 234,
	RPL_ServListEnd      = 235,
	RPL_AdminMe          = 256,                  // :Administrative info about server
	RPL_AdminLoc1        = 257,                  // :<info>
	RPL_AdminLoc2        = 258,                  // :<info>
	RPL_AdminEMail       = 259,                  // :<info>
	RPL_TryAgain         = 263,                  // :Server load is temporarily too heavy. Please wait a while and try again.

	// tracing
	RPL_TraceLink        = 200,
	RPL_TraceConnecting  = 201,
	RPL_TraceHandshake   = 202,
	RPL_TraceUnknown     = 203,
	RPL_TraceOperator    = 204,
	RPL_TraceUser        = 205,
	RPL_TraceServer      = 206,
	RPL_TraceService     = 207,
	RPL_TraceNewType     = 208,
	RPL_TraceClass       = 209,
	RPL_TraceReconnect   = 210,
	RPL_TraceLog         = 261,
	RPL_TraceEnd         = 262,

	// stats
	RPL_StatsLinkInfo    = 211,                  // <connection> <sendq> <sentmsg> <sentbyte> <recdmsg> <recdbyte> :<open>
	RPL_StatsCommands    = 212,                  // <command> <uses> <bytes>
	RPL_StatsCLine       = 213,                  // C <address> * <server> <port> <class>
	RPL_StatsNLine       = 214,                  // N <address> * <server> <port> <class>
	RPL_StatsILine       = 215,                  // I <ipmask> * <hostmask> <port> <class>
	RPL_StatsKLine       = 216,                  // k <address> * <username> <details>
	RPL_StatsPLine       = 217,                  // P <port> <??> <??>
	RPL_StatsQLine       = 222,                  // <mask> :<comment>
	RPL_StatsELine       = 223,                  // E <hostmask> * <username> <??> <??>
	RPL_StatsDLine       = 224,                  // D <ipmask> * <username> <??> <??>
	RPL_StatsLLine       = 241,                  // L <address> * <server> <??> <??>
	RPL_StatsuLine       = 242,                  // :Server Up <num> days, <time>
	RPL_StatsoLine       = 243,                  // o <mask> <password> <user> <??> <class>
	RPL_StatsHLine       = 244,                  // H <address> * <server> <??> <??>
	RPL_StatsGLine       = 247,                  // G <address> <timestamp> :<reason>
	RPL_StatsULine       = 248,                  // U <host> * <??> <??> <??>
	RPL_StatsZLine       = 249,                  // :info
	RPL_StatsYLine       = 218,                  // Y <class> <ping> <freq> <maxconnect> <sendq>
	RPL_EndOfStats       = 219,                  // <char> :End of /STATS report
	RPL_StatsUptime      = 242,

	// GLINE
	RPL_GLineList        = 280,                  // <address> <timestamp> <reason>                                                   UNDERNET
	RPL_EndOfGLineList   = 281,                  // :End of G-line List                                                              UNDERNET

	// Silence
	RPL_SilenceList      = 271,                  // <nick> <mask>                                                                    UNDERNET/DALNET
	RPL_EndOfSilenceList = 272,                  // <nick> :End of Silence List                                                      UNDERNET/DALNET

	// LUser
	RPL_LUserClient      = 251,                  // :There are <user> users and <invis> invisible on <serv> servers
	RPL_LUserOp          = 252,                  // <num> :operator(s) online
	RPL_LUserUnknown     = 253,                  // <num> :unknown connection(s)
	RPL_LUserChannels    = 254,                  // <num> :channels formed
	RPL_LUserMe          = 255,                  // :I have <user> clients and <serv> servers
	RPL_LUserLocalUser   = 265,                  // :Current local users: <curr> Max: <max>
	RPL_LUserGlobalUser  = 266,                  // :Current global users: <curr> Max: <max>


	// Errors
	ErrNoSuchNick       = 401,                  // <nickname> :No such nick
	ErrNoSuchServer     = 402,                  // <server> :No such server
	ErrNoSuchChannel    = 403,                  // <channel> :No such channel
	ErrCannotSendToChan = 404,                  // <channel> :Cannot send to channel
	ErrTooManyChannels  = 405,                  // <channel> :You have joined too many channels
	ErrWasNoSuchNick    = 406,                  // <nickname> :There was no such nickname
	ErrTooManyTargets   = 407,                  // <target> :Duplicate recipients. No message delivered
	ErrNoColors         = 408,                  // <nickname> #<channel> :You cannot use colors on this channel. Not sent: <text>   DALNET
	ErrNoOrigin         = 409,                  // :No origin specified
	ErrNoRecipient      = 411,                  // :No recipient given (<command>)
	ErrNoTextToSend     = 412,                  // :No text to send
	ErrNoTopLevel       = 413,                  // <mask> :No toplevel domain specified
	ErrWildTopLevel     = 414,                  // <mask> :Wildcard in toplevel Domain
	ErrBadMask          = 415,
	ErrTooMuchInfo      = 416,                  // <command> :Too many lines in the output, restrict your query                     UNDERNET
	ErrUnknownCommand   = 421,                  // <command> :Unknown command
	ErrNoMotd           = 422,                  // :MOTD File is missing
	ErrNoAdminInfo      = 423,                  // <server> :No administrative info available
	ErrFileError        = 424,
	ErrNoNicknameGiven  = 431,                  // :No nickname given
	ErrErroneusNickname = 432,                  // <nickname> :Erroneus Nickname
	ErrNickNameInUse    = 433,                  // <nickname> :Nickname is already in use.
	ErrNickCollision    = 436,                  // <nickname> :Nickname collision KILL
	ErrUnAvailResource  = 437,                  // <channel> :Cannot change nickname while banned on channel
	ErrNickTooFast      = 438,                  // <nick> :Nick change too fast. Please wait <sec> seconds.                         (most)
	ErrTargetTooFast    = 439,                  // <target> :Target change too fast. Please wait <sec> seconds.                     DALNET/UNDERNET
	ErrUserNotInChannel = 441,                  // <nickname> <channel> :They aren't on that channel
	ErrNotOnChannel     = 442,                  // <channel> :You're not on that channel
	ErrUserOnChannel    = 443,                  // <nickname> <channel> :is already on channel
	ErrNoLogin          = 444,
	ErrSummonDisabled   = 445,                  // :SUMMON has been disabled
	ErrUsersDisabled    = 446,                  // :USERS has been disabled
	ErrNotRegistered    = 451,                  // <command> :Register first.
	ErrNeedMoreParams   = 461,                  // <command> :Not enough parameters
	ErrAlreadyRegistered= 462,                  // :You may not reregister
	ErrNoPermForHost    = 463,
	ErrPasswdMistmatch  = 464,
	ErrYoureBannedCreep = 465,
	ErrYouWillBeBanned  = 466,
	ErrKeySet           = 467,                  // <channel> :Channel key already set
	ErrServerCanChange  = 468,                  // <channel> :Only servers can change that mode                                     DALNET
	ErrChannelIsFull    = 471,                  // <channel> :Cannot join channel (+l)
	ErrUnknownMode      = 472,                  // <char> :is unknown mode char to me
	ErrInviteOnlyChan   = 473,                  // <channel> :Cannot join channel (+i)
	ErrBannedFromChan   = 474,                  // <channel> :Cannot join channel (+b)
	ErrBadChannelKey    = 475,                  // <channel> :Cannot join channel (+k)
	ErrBadChanMask      = 476,
	ErrNickNotRegistered= 477,                  // <channel> :You need a registered nick to join that channel.                      DALNET
	ErrBanListFull      = 478,                  // <channel> <ban> :Channel ban/ignore list is full
	ErrNoPrivileges     = 481,                  // :Permission Denied- You're not an IRC operator
	ErrChanOPrivsNeeded = 482,                  // <channel> :You're not channel operator
	ErrCantKillServer   = 483,                  // :You cant kill a server!
	ErrRestricted       = 484,                  // <nick> <channel> :Cannot kill, kick or deop channel service                      UNDERNET
	ErrUniqOPrivsNeeded = 485,                  // <channel> :Cannot join channel (reason)
	ErrNoOperHost       = 491,                  // :No O-lines for your host
	ErrUModeUnknownFlag = 501,                  // :Unknown MODE flag
	ErrUsersDontMatch   = 502,                  // :Cant change mode for other users
	ErrSilenceListFull  = 511                   // <mask> :Your silence list is full                                                UNDERNET/DALNET

};