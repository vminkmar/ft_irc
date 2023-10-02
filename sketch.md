# Coding Convention
m_ before private members
Class Names uppercase

# Questions

# Todos
	
	Adjust Channel/User/UserManagement class design
	 --> Where to check for things like passw, userlimit and so on
	 --> implement user_id in user class  or force a unique username 
	      so one can use that to check for correct adding, erasing,
		   changing permissions
	
	Commands

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
