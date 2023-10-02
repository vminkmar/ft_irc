# Coding Convention
m_ before private members
Class Names uppercase

# Questions

# Todos
	
    Draw Structure 

    Restructure server loop

	UserManagement
    --> adapt overall structure
     --> change m_channels to std::map

    User class
    --> change std::map with int/fd as unique key
     --> implement Buffer class

    Buffer class

	Commands
    --> try to fully implement one or two commands ("full stack")

	Error Handling (Exceptions/Messages)

# Commands
    transform parameters in vector and at trail after : to a string

    Basics:
        "NICK"
        "USER"
        "PASS"
        "QUIT"
        "PRIVMSG"
        "PING"/"PONG"
        "ERROR" ?
        "RESTART"	?
        "NAMES" ?

    Channels:
        "LIST"
        "JOIN"
        "KICK"   /* operator */
        "INVITE"
        "TOPIC"  /* specific operators only */
        "MODE"   /* i, t, k, o, l */
        "PART"?

    Done:
        "USER"
        "NICK"
