# **ft_irc**
This is a C++ 98 implementation of an IRC server named ircserv. The server is designed to handle multiple clients concurrently without hanging, utilizing non-blocking I/O operations. The project strictly adheres to certain guidelines and specifications outlined in this README.

##**Project Overview**

### **Features**
- Capable of handling multiple clients simultaneously without hanging.
- Capable of handling multiple clients simultaneously without hanging.
- All I/O operations must be non-blocking.
- Use only 1 poll() (or equivalent) for handling operations.
- Implement features similar to an official IRC server using a reference client.
- Reference client should connect without errors.
- Implement authentication, set a nickname, a username, join a channel, send/receive private messages.
- Forward messages sent to a channel to all clients in that channel.
- Have operators and regular users.
- Implement commands for channel operators: KICK, INVITE, TOPIC, MODE (with subcommands).
  MODE subcommands: i, t, k, o, l.

### **Usage**

You need to install WeeChat. You can run WeeChat multiple times and set different users. If you attempt to use the same user, our server will send back an error message, and WeeChat will automatically change the name, retrying the login with the updated name.

`make`

`./ircserv <port> <password>`
