// -------------------------------------------------------------------------- //

#ifndef CLASS_SERVERCONFIG_HPP
#define CLASS_SERVERCONFIG_HPP

/* MAIN SETTINGS */
#define PORT 6667
#define HOST "localhost"
#define SERVERNAME std::string("vmeeeezy")
#define MAX_CLIENTS 512
#define CHANNEL_USERLIMIT 10

/* UN/AllOWED CHARS */
#define CHAR_UNALLOWED_NICK " !@#$%^&*()[]{}<>:;,/"
#define CHAR_ALLOWED_CHANNEL "#&+!"
#define CHAR_ALLOWED_MODS "itkol"

/* DEFAULT MESSAGES */
#define DEFMSG_PART "Goodbye!"
#define DEFMSG_PROMOTION " has been promoted as an Operator (last operator left the channel)!"
#define DEFMSG_KICK "Begone you fool!"

#endif // CLASS_SERVERCONFIG_HPP

// -------------------------------------------------------------------------- //