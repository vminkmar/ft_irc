# Coding Convention

    m_ before private members
    Class Names uppercase

# Questions

    connecting --> disconnecting --> reconnecting && quitting

# Todos

    @jakob Channel Map: implement channel signs # & + !
    @jakob JOIN
            --> add passw management

    @jakob CAP Process - what if NICK is not set / already in USE
    @jakob QUIT -- implement erasing/removing and fix fejoining

# Commands
    
    Basics:

        "NICK"  -- works

        "USER"  -- works

        "PASS"
			-need to add it at the end
        
        "QUIT"
		    - User should leave all channels, User gets erased from all 
            - containers and we need to delete user from pollfd struct or vector.

        "PRIVMSG"
			- m_parameters[0] are users as well as channels (msgtarget)
			- m_parameters[1] is the content of the message

        "PING"/"PONG" -- works
        
        "ERROR" ?
        "RESTART"	?
        "NAMES" ?

    Channels:
				
        "JOIN"

        "KICK"   /* operator */
			- remove User from vector
			- m_parameters[0] are the channels that can be divided by ,
			- m_parameters[1] are the userss that can be divided by ,

        "INVITE"
			- m_parameters[0] is the nickname that you want to invite to the 
             channel which is m_parameter[1]
			- only memebers of a channel are allowed to invite other users
			- if invite-only flag is active, just channel operators can invite 
             others

        "TOPIC"  /* specific operators only */
			- is used to change or view the topic of a channel
			- if m_parameters[1] is empty than the topic of the channel in 
             m_parameters[0] is shown else you can change the topic of this
             channel
			- if topic is empty the channel will be removed

        "MODE"   /* i, t, k, o, l */
			- i = set/remove invite-only channel
			- k = change channel key/password
			- t = set/remove the restrictions of the TOPIC command to channel
             operators
			- o = give channel operator priviliges to a user
			- l = set/remove user limit to channel

			- m_parameters[0] is the channel
			- m_parameters[1] is "+" to set the flag or "-" to remove the flag
			- m_parameters[2] is either the target or the nickname 

        "LIST"?
        "PART"?

    Done:
				"PING/PONG"

# Documentation
		
    All incoming Messages are parsed in 3 Parts:
    
    COMMAND PARAMETERS TRAIL
    
    COMMAND is always the first part of the message. In the function
    Server::Messages you always need to look for m_command.

    For PARAMETERS you need to work with the m_parameters(std::vector).

    If there is a TRAIL it is stored in m_trail,
    but mostly you won't need it but you need to look it up in the
   
    RFC2812

    If you want to search for other stuff then you look up RFC2810-RFC2813

# Example Command on Client

    /nick jwill
    --> should log a message on the server

    /huhu hallo
    --> not a valid command, client will give error message

