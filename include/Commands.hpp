#ifndef COMMANDS_HPP
# define COMMANDS_HPP

#include <sstream>
#include <iostream>

template<typename T>
std::string to_string(const T & value) {
	std::ostringstream oss;
	oss << value;
	std::string to_return = oss.str();
	while (to_return.size() < 3)
		to_return = "0" + to_return;
	return to_return;
}

enum Reply
{
	RPL_NONE				= 0,
	// Initial
	RPL_WELCOME				= 001,				  // :Welcome to the Internet Relay Network <nickname>
	RPL_YOURHOST			= 002,				  // :Your host is <server>, running version <ver>
	RPL_CREATED				= 003,				  // :This server was created <datetime>
	RPL_MYINFO				= 004,				  // <server> <ver> <usermode> <chanmode>
	RPL_BOUNCE				= 005,				  // :map
	RPL_MAP					= 006,
	RPL_MAPEND				= 007,				  // :End of /MAP
	RPL_MOTDSTART			= 375,				  // :- server Message of the Day
	RPL_MOTD				= 372,				  // :- <info>
	RPL_MOTDALT				= 377,				  // :- <info>																		(some)
	RPL_MOTDALT2			= 378,				  // :- <info>																		(some)
	RPL_ENDOFMOTD			= 376,				  // :End of /MOTD command.
	RPL_UMODEIS				= 221,				  // <mode>

	// IsOn/UserHost
	RPL_USERHOST			= 302,				  // :userhosts
	RPL_ISON				= 303,				  // :nicknames

	// Away
	RPL_AWAY				= 301,				  // <nick> :away
	RPL_UNAWAY				= 305,				  // :You are no longer marked as being away
	RPL_NOWAWAY				= 306,				  // :You have been marked as being away

	// WHOIS/WHOWAS
	RPL_WHOISHELPER			= 310,				  // <nick> :looks very helpful													   DALNET
	RPL_WHOISUSER			= 311,				  // <nick> <username> <address> * :<info>
	RPL_WHOISSERVER			= 312,				  // <nick> <server> :<info>
	RPL_WHOISOPERATOR		= 313,				  // <nick> :is an IRC Operator
	RPL_WHOISIDLE			= 317,				  // <nick> <seconds> <signon> :<info>
	RPL_ENDOFWHOIS			= 318,				  // <request> :End of /WHOIS list.
	RPL_WHOISCHANNELS		= 319,				  // <nick> :<channels>
	RPL_WHOWASUSER			= 314,				  // <nick> <username> <address> * :<info>
	RPL_ENDOFWHOWAS			= 369,				  // <request> :End of WHOWAS
	RPL_WHOREPLY			= 352,				  // <channel> <username> <address> <server> <nick> <flags> :<hops> <info>
	RPL_ENDOFWHO			= 315,				  // <request> :End of /WHO list.
	RPL_USERIPS				= 307,				  // :userips																		 UNDERNET
	RPL_USERIP				= 340,				  // <nick> :<nickname>=+<user>@<IP.address>										  UNDERNET

	// List
	RPL_LISTSTART			= 321,				  // Channel :Users Name
	RPL_LIST				= 322,				  // <channel> <users> :<topic>
	RPL_LISTEND				= 323,				  // :End of /LIST
	RPL_LINKS				= 364,				  // <server> <hub> :<hops> <info>
	RPL_ENDOFLINKS			= 365,				  // <mask> :End of /LINKS list.

	// Post-Channel Join
	RPL_UNIQOPIS			= 325,
	RPL_CHANNELMODEIS		= 324,				  // <channel> <mode>
	RPL_CHANNELURL			= 328,				  // <channel> :url																   DALNET
	RPL_CHANNELCREATED		= 329,				  // <channel> <time>
	RPL_NOTOPIC				= 331,				  // <channel> :No topic is set.
	RPL_TOPIC				= 332,				  // <channel> :<topic>
	RPL_TOPICSETBY			= 333,				  // <channel> <nickname> <time>
	RPL_NAMREPLY			= 353,				  // = <channel> :<names>
	RPL_ENDOFNAMES			= 366,				  // <channel> :End of /NAMES list.

	// Invitational
	RPL_INVITING			= 341,				  // <nick> <channel>
	RPL_Summoning			= 342,

	// Channel Lists
	RPL_INVITELIST			= 346,				  // <channel> <invite> <nick> <time>												 IRCNET
	RPL_ENDOFINVITLIST		= 357,				  // <channel> :End of Channel Invite List											IRCNET
	RPL_EXCEPTLIST			= 348,				  // <channel> <exception> <nick> <time>											  IRCNET
	RPL_ENDOFEXCEPTLIST		= 349,				  // <channel> :End of Channel Exception List										 IRCNET
	RPL_BANLIST				= 367,				  // <channel> <ban> <nick> <time>
	RPL_ENDOFBANLIST		= 368,				  // <channel> :End of Channel Ban List


	// server/misc
	RPL_VERSION				= 351,				  // <version>.<debug> <server> :<info>
	RPL_INFO				= 371,				  // :<info>
	RPL_ENDOFINFO			= 374,				  // :End of /INFO list.
	RPL_YOUREOPER			= 381,				  // :You are now an IRC Operator
	RPL_REHASHING			= 382,				  // <file> :Rehashing
	RPL_YOURESERVICE		= 383,
	RPL_TIME				= 391,				  // <server> :<time>
	RPL_USERSSTART			= 392,
	RPL_USERS				= 393,
	RPL_ENDOFUSERS			= 394,
	RPL_NOUSERS				= 395,
	RPL_SERVLIST			= 234,
	RPL_SERVLISTEND			= 235,
	RPL_ADMINME				= 256,				  // :Administrative info about server
	RPL_ADMINLOC1			= 257,				  // :<info>
	RPL_ADMINLOC2			= 258,				  // :<info>
	RPL_ADMINEMAIL			= 259,				  // :<info>
	RPL_TRYAGAIN			= 263,				  // :Server load is temporarily too heavy. Please wait a while and try again.
	RPL_KILLDONE			= 361,

	// tracing
	RPL_TRACELINK			= 200,
	RPL_TRACECONNECTING		= 201,
	RPL_TRACEHANDSHAKE		= 202,
	RPL_TRACEUNKNOWN		= 203,
	RPL_TRACEOPERATOR		= 204,
	RPL_TRACEUSER			= 205,
	RPL_TRACESERVER			= 206,
	RPL_TRACESERVICE		= 207,
	RPL_TRACENEWTYPE		= 208,
	RPL_TRACECLASS			= 209,
	RPL_TRACERECONNECT		= 210,
	RPL_TRACELOG			= 261,
	RPL_TRACEEND			= 262,

	// stats
	RPL_STATSLINKINFO		= 211,				  // <connection> <sendq> <sentmsg> <sentbyte> <recdmsg> <recdbyte> :<open>
	RPL_STATSCOMMANDS		= 212,				  // <command> <uses> <bytes>
	RPL_STATSCLINE			= 213,				  // C <address> * <server> <port> <class>
	RPL_STATSNLINE			= 214,				  // N <address> * <server> <port> <class>
	RPL_STATSILINE			= 215,				  // I <ipmask> * <hostmask> <port> <class>
	RPL_STATSKLINE			= 216,				  // k <address> * <username> <details>
	RPL_STATSPLINE			= 217,				  // P <port> <??> <??>
	RPL_STATSQLINE			= 222,				  // <mask> :<comment>
	RPL_STATSELINE			= 223,				  // E <hostmask> * <username> <??> <??>
	RPL_STATSDLINE			= 224,				  // D <ipmask> * <username> <??> <??>
	RPL_STATSLLINE			= 241,				  // L <address> * <server> <??> <??>
	RPL_STATSuLINE			= 242,				  // :Server Up <num> days, <time>
	RPL_STATSoLINE			= 243,				  // o <mask> <password> <user> <??> <class>
	RPL_STATSHLINE			= 244,				  // H <address> * <server> <??> <??>
	RPL_STATSGLINE			= 247,				  // G <address> <timestamp> :<reason>
	RPL_STATSULINE			= 248,				  // U <host> * <??> <??> <??>
	RPL_STATSZLINE			= 249,				  // :info
	RPL_STATSYLINE			= 218,				  // Y <class> <ping> <freq> <maxconnect> <sendq>
	RPL_ENDOFSTATS			= 219,				  // <char> :End of /STATS report
	RPL_STATSUPTIME			= 242,

	// GLINE
	RPL_GLINELIST			= 280,				  // <address> <timestamp> <reason>												   UNDERNET
	RPL_ENDOFGLINELIST		= 281,				  // :End of G-line List															  UNDERNET

	// Silence
	RPL_SILENCELIST			= 271,				  // <nick> <mask>																	UNDERNET/DALNET
	RPL_ENDOFSILENCELIST 	= 272,				  // <nick> :End of Silence List													  UNDERNET/DALNET

	// LUser
	RPL_LUSERCLIENT			= 251,				  // :There are <user> users and <invis> invisible on <serv> servers
	RPL_LUSEROP				= 252,				  // <num> :operator(s) online
	RPL_LUSERUNKNOWN		= 253,				  // <num> :unknown connection(s)
	RPL_LUSERCHANNELS		= 254,				  // <num> :channels formed
	RPL_LUSERME				= 255,				  // :I have <user> clients and <serv> servers
	RPL_LUSERLOCALUSER		= 265,				  // :Current local users: <curr> Max: <max>
	RPL_LUUSERGLOBALUSER	= 266,				  // :Current global users: <curr> Max: <max>


	// Errors
	ERRNOSUCHNICK			= 401,				  // <nickname> :No such nick
	ERRNOSUCHSERVER			= 402,				  // <server> :No such server
	ERRNOSUCHCHANNEL		= 403,				  // <channel> :No such channel
	ERRCANNOTSENDTOCHAN		= 404,				  // <channel> :Cannot send to channel
	ERRTOOMANYCHANNELS		= 405,				  // <channel> :You have joined too many channels
	ERRWASNOSUCHNICK		= 406,				  // <nickname> :There was no such nickname
	ERRTOOMANYTARGETS		= 407,				  // <target> :Duplicate recipients. No message delivered
	ERRNOCOLORS				= 408,				  // <nickname> #<channel> :You cannot use colors on this channel. Not sent: <text>   DALNET
	ERRNOORIGIN				= 409,				  // :No origin specified
	ERRNORECIPIENT			= 411,				  // :No recipient given (<command>)
	ERRNOTEXTTOSEND			= 412,				  // :No text to send
	ERRNOTOPLEVEL			= 413,				  // <mask> :No toplevel domain specified
	ERRWILDTOPLEVEL			= 414,				  // <mask> :Wildcard in toplevel Domain
	ERRBADMASK				= 415,
	ERRTOOMUCHINFO			= 416,				  // <command> :Too many lines in the output, restrict your query					 UNDERNET
	ERRUNKNOWNCOMMAND		= 421,				  // <command> :Unknown command
	ERRNOMOTD				= 422,				  // :MOTD File is missing
	ERRNOADMININFO			= 423,				  // <server> :No administrative info available
	ERRFILEERROR			= 424,
	ERRNONICKNAMEGIVEN		= 431,				  // :No nickname given
	ERRERRONEUSNICKNAME		= 432,				  // <nickname> :Erroneus Nickname
	ERRNICKNAMEINUSE		= 433,				  // <nickname> :Nickname is already in use.
	ERRNICKCOLLISION		= 436,				  // <nickname> :Nickname collision KILL
	ERRUNAVAILRESSOURCE		= 437,				  // <channel> :Cannot change nickname while banned on channel
	ERRNICKTOOFAST			= 438,				  // <nick> :Nick change too fast. Please wait <sec> seconds.						 (most)
	ERRTARGETTOOFAST		= 439,				  // <target> :Target change too fast. Please wait <sec> seconds.					 DALNET/UNDERNET
	ERRUSERNOTINCHANNEL		= 441,				  // <nickname> <channel> :They aren't on that channel
	ERRNOTONCHANNEL			= 442,				  // <channel> :You're not on that channel
	ERRUSERONCHANNEL		= 443,				  // <nickname> <channel> :is already on channel
	ERRNOLOGIN				= 444,
	ERRSUMMONDISABLED		= 445,				  // :SUMMON has been disabled
	ERRUSERSDISABLED		= 446,				  // :USERS has been disabled
	ERRNOTREGISTERED		= 451,				  // <command> :Register first.
	ERRNEEDMOREPARAMS		= 461,				  // <command> :Not enough parameters
	ERRALREADYREGISTERED	= 462,				  // :You may not reregister
	ERRNOPERMFORHOST		= 463,
	ERRPASSWDMISTMATCH		= 464,
	ERRYOUREBANNEDCREEP		= 465,
	ERRYOUWILLBEBANNED		= 466,
	ERRKEYSET				= 467,				  // <channel> :Channel key already set
	ERRSERVERCANCHANGE		= 468,				  // <channel> :Only servers can change that mode									 DALNET
	ERRCHANNELISFULL		= 471,				  // <channel> :Cannot join channel (+l)
	ERRUNKNOWNMODE			= 472,				  // <char> :is unknown mode char to me
	ERRINVITEONLYCHAN		= 473,				  // <channel> :Cannot join channel (+i)
	ERRBANNEDFROMCHAN		= 474,				  // <channel> :Cannot join channel (+b)
	ERRBADCHANNELKEY		= 475,				  // <channel> :Cannot join channel (+k)
	ERRBADCHANMASK			= 476,
	ERRNICKNOREGISTERED		= 477,				  // <channel> :You need a registered nick to join that channel.					  DALNET
	ERRBANLISTFULL			= 478,				  // <channel> <ban> :Channel ban/ignore list is full
	ERRNOPRIVILEGES			= 481,				  // :Permission Denied- You're not an IRC operator
	ERRCHANOPRIVSNEED		= 482,				  // <channel> :You're not channel operator
	ERRCANTKILLSERVER		= 483,				  // :You cant kill a server!
	ERRRESTRICTED			= 484,				  // <nick> <channel> :Cannot kill, kick or deop channel service					  UNDERNET
	ERRUNIQOPRIVSNEED		= 485,				  // <channel> :Cannot join channel (reason)
	ERRNOOPERHOST			= 491,				  // :No O-lines for your host
	ERRUMODEUNKNOWNFLAG		= 501,				  // :Unknown MODE flag
	ERRUSERSDONTMATCH		= 502,				  // :Cant change mode for other users
	ERRSILENCELISTFULL		= 511,				   // <mask> :Your silence list is full												UNDERNET/DALNET
	ERRNOCHANMODES			= 512
};

#endif