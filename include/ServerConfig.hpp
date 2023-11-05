// -------------------------------------------------------------------------- //

#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

#define PORT       6667
#define HOST       "localhost"
#define SERVERNAME std::string("vmeeeezy")

/* LOG COLOURING */
#define YELLOW             "\033[33m"
#define PINK               "\033[95m"
#define CYAN               "\033[36m"
#define RED                "\033[31m"
#define WHITE              "\033[37m"
#define RESET              "\033[0m"
#define COLOUR_LOG         WHITE
#define COLOUR_INTERACTION WHITE
#define COLOUR_IN          CYAN
#define COLOUR_OUT         PINK
#define COLOUR_ERR         RED

#define CHAR_UNALLOWED_NICK  " !@#$%^&*()[]{}<>:;,/"
#define CHAR_ALLOWED_CHANNEL "#&+!"
#define CHAR_ALLOWED_MODS    "itkol"

/* DEFAULT MESSAGES */
#define DEFMSG_PART      "Goodbye!"
#define DEFMSG_PROMOTION "I have been promoted to an Operator!"

#define MAX_CLIENTS 512

#endif // SERVERCONFIG_HPP

// -------------------------------------------------------------------------- //
