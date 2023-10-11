# Coding Convention
m_ before private members
Class Names uppercase

# Questions

# Todos

    Update main branch
     --> branch dev/jakob has some important bug fixes
     --> branch buffer/new buffer has some important updates
	
    Consistent Formatting
     --> Look through all classes and implementation and have consistent
         documentation and formatting

    Implement example command
     --> Rigouros testing and error handling so we have a "prime example"

    Implement all commands

# Commands
    transform parameters in vector and at trail after : to a string

    Basics:
        "NICK"
					- need to check if it is the first message and if the nickname is already set, if so then we need to send an message back to client that the nickname got changed, need to check if nick is already used.

        "USER"
					- need to check if user name is already used?

        "PASS"

        "QUIT"
					- User should leave all channels, User gets erased from all containers and we need to delete user from pollfd struct or vector.

        "PRIVMSG"
					- m_parameters[0] are users as well as channels (msgtarget)
					- m_parameters[1] is the content of the message

        "PING"/"PONG"
					works


        "ERROR" ?
        "RESTART"	?
        "NAMES" ?

    Channels:
				
        "JOIN"
					- Channel can be divided by , that means there are multiple channels (m_parameters[0])
					- Channels can have an # or an & as Prefix. # means channel with key and  & means without a key/password
					- m_parameters[1] are the keys that can also be divided by comma
					- if m_parameters[0] = 0 then the user wants to leave all currently joined channels
					- check if a Channel with the same name exists
					- first User that creates a Channel needs to be operator
					- User gets join message



        "KICK"   /* operator */
					- remove User from vector
					- m_parameters[0] are the channels that can be divided by ,
					- m_parameters[1] are the userss that can be divided by ,

        "INVITE"
					- m_parameters[0] is the nickname that you want to invite to the channel which is m_parameter[1]
					- only memebers of a channel are allowed to invite other users
					- if invite-only flag is active, just channel operators can invite others

        "TOPIC"  /* specific operators only */
					- is used to change or view the topic of a channel
					- if m_parameters[1] is empty than the topic of the channel in m_parameters[0] is shown else you can change the topic of this channel
					- if topic is empty the channel will be removed

        "MODE"   /* i, t, k, o, l */
					- i = set/remove invite-only channel
					- k = change channel key/password
					- t = set/remove the restrictions of the TOPIC command to channel operators
					- o = give channel operator priviliges to a user
					- l = set/remove user limit to channel

					- m_parameters[0] is the channel
					- m_parameters[1] is "+" to set the flag or "-" to remove the flag
					- m_parameters[2] is either the target or the nickname 



        "LIST"?
        "PART"?

    Done:
				"PING/PONG"

# Documenation
		All incoming Messages are parsed in 3 Parts, COMMAND, PARAMETERS and TRAIL. Command is always the first part of themessage. In the function Server::Messages you always need to look for m_command and after that you need to work with them_parameters(vector). If there is a trail it is stored in m_trail but mostly you won't need it but you need to look it upin the RFC2812. If you want to search for other stuff then you look up RFC2810-RFC2813. If you have any question write mea message.
