// -------------------------------------------------------------------------- //

#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

/* MAIN SETTINGS */
#define PORT       6667
#define HOST       "localhost"
#define SERVERNAME std::string("vmeeeezy")
#define MAX_CLIENTS 512

/* UN/AllOWED CHARS */
#define CHAR_UNALLOWED_NICK  " !@#$%^&*()[]{}<>:;,/"
#define CHAR_ALLOWED_CHANNEL "#&+!"
#define CHAR_ALLOWED_MODS    "itkol"

/* DEFAULT MESSAGES */
#define DEFMSG_PART      "Goodbye!"
#define DEFMSG_PROMOTION "I have been promoted to an Operator!"

#endif // SERVERCONFIG_HPP

// -------------------------------------------------------------------------- //
