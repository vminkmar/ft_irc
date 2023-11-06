// -------------------------------------------------------------------------- //

#include "../include/class.Server.hpp"         // needed for Server class
#include "../include/class.UserManagement.hpp" // needed for UserManagement class
#include "../include/utils.hpp"                // needed for sumParameters

#include <cstdlib>  // needed for exit() (linux compilation)
#include <cstring>  // needed for memset() (linux compilation)
#include <fcntl.h>  // needed for fcntl()
#include <iostream> // needed for std::cout, std::endl
#include <netdb.h>  // needed for addrinfo
#include <sstream>  // needed for std::stringstream
#include <stdio.h>  // needed for perror() (linux compilation)
#include <unistd.h> // needed for read()

// ------------------ public --------------------------- //

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> constructors */

Server::Server() : m_maxClients(MAX_CLIENTS), m_command(""), m_trail("")
{
}
Server::~Server(){};
bool Server::serverRunning = true;

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> member functions */

void Server::start(int argc, char **argv)
{

    /* get sizeof of struct sockaddr_in address */
    if (inputCheck(argc) == false)
    {
        LOG_ERR("Wrong number of arguments");
        LOG("Usage: ./ircserv <PORT> <PASSWD>");
        return;
    }
    if (getPortAndPasswd(argv) == false)
    {
        return;
    }
    this->m_addrlen = sizeof(this->address);

    /* set socket int to m_server_fd */
    if ((this->m_server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        LOG_ERR("socket");
        return;
    }

    int reuseaddr = 1;
    /* set socket options */
    if (setsockopt(m_server_fd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr)) == -1)
    {
        LOG_ERR("socketopt");
        close(m_server_fd);
        return;
    }
    this->address.sin_family = AF_INET;
    this->address.sin_addr.s_addr = INADDR_ANY;
    /* use of conversion function htons (host byte -> network byte order) */
    this->address.sin_port = htons(PORT);
    memset(this->address.sin_zero, '\0', sizeof(this->address.sin_zero));

    /* bind address to socket */
    if (bind(this->m_server_fd, reinterpret_cast<struct sockaddr *>(&address), sizeof(this->address)) < 0)
    {
        LOG_ERR("bind");
        close(m_server_fd);
        return;
    }

    /* makes socket ready to listen for clients */
    if (listen(this->m_server_fd, 10) < 0)
    {
        LOG_ERR("listen");
        close(m_server_fd);
        return;
    }
    fcntl(m_server_fd, F_SETFL, O_NONBLOCK);
    struct pollfd newServer;
    newServer.fd = m_server_fd;
    newServer.events = POLLIN;
    m_pollfds.push_back(newServer);

    if (createBot() == false)
    {
        return;
    }

    while (Server::serverRunning == true)
    {
        /* waits for event on filedescriptor */
        int ret = poll(this->m_pollfds.data(), m_pollfds.size(), 100);
        if (ret < 0)
        {
            LOG_ERR("poll");
            cleanUpSockets();
            break;
        }

        /* if there is some incoming event */
        if (this->m_pollfds[0].revents & POLLIN)
        {
            acceptClients();
        }
        routine();
    }
    close(m_server_fd);
    close(Marvin.socket);
}

bool Server::getPortAndPasswd(char **argv)
{
    std::string str = argv[1];
    for (size_t i = 0; i < str.size(); i++)
        if (isnumber(str[i]) == false)
        {
            LOG_ERR("Bad input as Port");
            return false;
        }
    this->m_port = atoi(argv[1]);
    if (this->m_port != PORT)
    {
        LOG_ERR("Wrong Port given. Use Port # 6667!");
        return false;
    }
    std::string passwd = argv[2];
    m_passwd = passwd;
    return true;
}

void Server::signal_handler(int sig)
{
    if (sig == SIGINT)
        Server::serverRunning = false;
    return;
}

// ------------------ private --------------------------- //

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> setup and connection */

void Server::acceptClients()
{

    /* accepts client on newSocket */
    int newSocket;
    if ((newSocket = accept(this->m_server_fd, (struct sockaddr *)&address, (socklen_t *)&m_addrlen)) < 0)
    {
        LOG_ERR("accept");
    }
    struct pollfd newClient;
    fcntl(newSocket, F_SETFL, O_NONBLOCK);
    newClient.fd = newSocket;
    newClient.events = POLLIN | POLLOUT;
    newClient.revents = 0;
    this->m_pollfds.push_back(newClient);
    um.addUser(newSocket);
    if (m_pollfds.size() == 2)
    {
        Marvin.socket = newSocket;
        Marvin.self = um.getUser(Marvin.socket);
        Marvin.self->setUsername("Bot");
        Marvin.self->setNickname("Marvin");
        Marvin.self->toggleWelcomedStatus();
    }
    this->m_pollfds[0].revents = 0; /* current event */
}

bool Server::createBot()
{
    int botSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (botSocket < 0)
    {
        LOG_ERR("botSocket");
        return false;
    }

    struct sockaddr_in serverAddress;
    struct addrinfo hints;
    struct addrinfo *servinfo;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(HOST, itostr(PORT).c_str(), &hints, &servinfo) != 0)
    {
        LOG_ERR("getaddrinfo");
        close(botSocket);
        return false;
    }

    serverAddress = *((struct sockaddr_in *)(servinfo->ai_addr));
    if (connect(botSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        LOG_ERR("connect");
        close(botSocket);
        return false;
    }

    freeaddrinfo(servinfo);
    return true;
}

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> server routine */

void Server::routine()
{
    for (t_vec_pollfd_it it = m_pollfds.begin() + 1; it != m_pollfds.end(); ++it)
    {
        if (it->revents == 0)
        {
            continue;
        }
        if (it->revents & (POLLERR | POLLHUP | POLLNVAL))
        {
            um.setOnlineStatus(it->fd, OFFLINE);
        }
        else if (it->revents & POLLIN)
        {
            receiveMessages(it->fd);
        }
        else if (it->revents & POLLOUT)
        {
            sendMessages(it->fd);
        }
        it->revents = 0;
    }
    cleanUpSockets();
}

void Server::receiveMessages(int socket)
{
    char buffer[30000];
    memset(buffer, 0, sizeof(buffer));
    int reading = read(socket, buffer, sizeof(buffer));
    if (reading < 0)
    {
        LOG_ERR("Reading error in receiveMessages()");
    }
    t_str message = buffer;
    parseIncomingMessage(message, socket);
}

void Server::sendMessages(int socket)
{
    while (um.getBuffer(socket, OUTPUT).empty() == false)
    {

        t_str_c &outputBuffer = um.getBuffer(socket, OUTPUT);
        size_t messageEnd = outputBuffer.find("\r\n");
        if (messageEnd == t_str::npos)
        {
            LOG_ERR("no \r\n found in OUTPUT Buffer!");
            break;
        }
        messageEnd += 2;
        t_str_c &message = outputBuffer.substr(0, messageEnd);

        LOG_SEND(socket, message);
        int sending = send(socket, message.data(), message.length(), 0);
        if (sending < 0)
        {
            LOG_ERR("Send return < 0?");
        }
        um.eraseBuffer(socket, 0, messageEnd, OUTPUT);
    }
}

void Server::cleanUpSockets()
{
    for (t_vec_pollfd_it it = m_pollfds.begin() + 2; it != m_pollfds.end();)
    {
        int socket = it->fd;
        t_str_c nickname = um.getNickname(it->fd);

        if (isErasable(socket) == true)
        {
            um.eraseUser(socket);
            it = m_pollfds.erase(it);
            LOG("Socket #" + itostr(socket) + " has been removed (" + nickname + ")");
            close(socket);
        }
        else
        {
            ++it;
        }
    }
}

void Server::cleanEmptyChannels()
{
    t_vec_str_c channelNames = split(um.getChannelNames(), ',');
    for (t_vec_str_cit it = channelNames.begin(); it != channelNames.end(); ++it)
    {
        t_str_c channelName = *it;
        Channel const *channel = um.getChannel(channelName);
        if (channel->getNumberOfUsers() < 1 ||
            (channel->getNumberOfUsers() == 1 && um.getNickname(channel->getFirstUserSocket()) == "Marvin"))
        {
            um.eraseChannel(channelName);
            LOG(channelName + " has been removed (no Users)");
        }
    }
}

void Server::autoPromoteOperator()
{
    t_vec_str_c channelNames = split(um.getChannelNames(), ',');

    for (t_vec_str_cit it = channelNames.begin(); it != channelNames.end(); ++it)
    {
        t_str_c &channelName = *it;
        Channel *channel = um.getChannel(*it);

        if (channel->hasOperator() == false)
        {
            int socketLastUser = channel->getLastUserSocket();
            t_str_c &nicknameLastUser = um.getNickname(socketLastUser);

            channel->addUser(socketLastUser, OPERATOR);

            t_str msg = nicknameLastUser + t_str_c(DEFMSG_PROMOTION);

            broadcast(Marvin.self->getNickname(), channelName, "", msg, "PRIVMSG");
        }
    }
}

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> command parsing */

void Server::Messages(int socket)
{
    if (m_command == "PASS")
    {
        comparePassword(socket);
        return;
    }
    if (um.getRegisteredStatus(socket) == true)
    {
        if (m_command == "CAP")
        {
            CMD_CAP(socket);
        }
        else if (m_command == "NICK")
        {
            CMD_NICK(socket);
        }
        else if (m_command == "USER")
        {
            CMD_USER(socket);
        }
        else if (m_command == "PING")
        {
            CMD_PING(socket);
        }
        else if (m_command == "QUIT")
        {
            CMD_QUIT(socket);
        }
        else if (m_command == "JOIN")
        {
            CMD_JOIN(socket);
        }
        else if (m_command == "PART")
        {
            CMD_PART(socket);
        }
        else if (m_command == "PRIVMSG")
        {
            CMD_PRIVMSG(socket);
        }
        else if (m_command == "TOPIC")
        {
            CMD_TOPIC(socket);
        }
        else if (m_command == "INVITE")
        {
            CMD_INVITE(socket);
        }
        else if (m_command == "KICK")
        {
            CMD_KICK(socket);
        }
        else if (m_command == "MODE")
        {
            CMD_MODE(socket);
        }
        else
        {
            ERR_UNKNOWNCOMMAND(socket);
        }
    }
    cleanEmptyChannels();
    autoPromoteOperator();
}

t_str Server::getParameter(t_str_c &message)
{
    size_t colon = message.find(":");
    if (colon != t_str::npos)
    {
        t_str before = message.substr(0, colon);
        t_str after = message.substr(colon + 1);
        std::stringstream iss(before);
        t_str token;
        while (iss >> token)
        {
            m_parameters.push_back(token);
        }
        return (after);
    }
    else
    {
        std::stringstream iss(message);
        t_str token;
        while (iss >> token)
        {
            this->m_parameters.push_back(token);
        }
        return ("");
    }
}

void Server::getCommand(t_str &message)
{
    size_t end = message.find(" ");
    this->m_command = message.substr(0, end);
    message.erase(message.begin(), message.begin() + end + 1);
}

t_str_c Server::getPartMessage() const
{
    if (m_parameters.size() >= 2)
    {
        return sumParameters(m_parameters.begin() + 1, m_parameters.end());
    }
    return DEFMSG_PART;
}

void Server::comparePassword(int socket)
{
    if (um.getRegisteredStatus(socket) == true)
        ERR_ALREADYREGISTRED(socket);
    else if (m_passwd != m_parameters[0])
        ERR_PASSWDMISMATCH(socket);
    else
    {
        um.setRegisteredStatus(socket, true);
        LOG_ERR(itostr(um.getRegisteredStatus(socket)));
    }
}

void Server::parseIncomingMessage(t_str_c &incomingMessage, int socket)
{
    t_str message = incomingMessage;

    if (!um.getBuffer(socket, INPUT).empty())
    {
        t_str buffer = um.getBuffer(socket, INPUT);
        buffer.append(message);
        message = buffer;
    }
    LOG_INC(socket, incomingMessage);
    size_t pos = message.find("\r\n");
    if (pos != std::string::npos)
    {
        LOG("<inputBuffer> " + um.getBuffer(socket, INPUT));
        while (pos != t_str::npos)
        {
            t_str tmp = message.substr(0, pos);

            getCommand(tmp);

            tmp = getParameter(tmp);

            LOG("<command> " + m_command);
            for (t_vec_str_cit it = m_parameters.begin(); it != m_parameters.end(); ++it)
            {
                LOG("<param>   " + *it);
            }
            m_trail = tmp;
            LOG("<trail>   " + m_trail);

            message.erase(message.begin(), message.begin() + pos + 2);
            if (!(um.getBuffer(socket, INPUT).empty()))
            {
                um.eraseBuffer(socket, 0, pos + 2, INPUT);
            }
            Messages(socket);
            m_parameters.clear();
            pos = message.find("\r\n");
        }
    }
    else
    {
        if (!incomingMessage.empty())
        {
            um.appendToBuffer(socket, incomingMessage, INPUT);
        }
    }
}

bool Server::hasUnallowedChar(t_str_c &stringToCheck, t_str_c &unallowedChars) const
{
    for (size_t i = 0; i < unallowedChars.length(); ++i)
    {
        size_t find = stringToCheck.find(unallowedChars[i]);
        if (find != t_str::npos)
        {
            return true;
        }
    }
    return false;
}

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> user/channel handling */

bool Server::isErasable(int socket) const
{
    if (um.checkForUser(socket) == true)
    {
        if (um.getOnlineStatus(socket) == OFFLINE)
        {
            return true;
        }
    }
    return false;
}

void Server::eraseUserFromAllChannels(int socket)
{
    t_vec_str_c channels = split(um.getChannelNames(), ',');
    t_str_c &partMessage = getPartMessage();
    t_str_c &nickname = um.getNickname(socket);

    for (t_vec_str_cit it = channels.begin(); it != channels.end(); ++it)
    {
        t_str_c &channelName = *it;
        if (um.getChannel(channelName)->isMember(socket) == true)
        {
            um.eraseUserFromChannel(socket, channelName);
            RPL_PART(socket, socket, channelName, partMessage);
            broadcast(nickname, channelName, "", partMessage, "PART");
        }
    }
}

void Server::createChannelBy(int socket, t_str_c &channelName, t_str_c &channelKey)
{
    if (channelName.size() <= 1 || channelName.size() >= 50 || channelName.find_first_of(CHAR_ALLOWED_CHANNEL) != 0 ||
        channelName.find_first_of(" ,\a") != t_str::npos)
    {
        ERR_NOSUCHCHANNEL(socket, channelName);
        return;
    }

    um.addChannel(channelName);
    um.addUserToChannel(socket, OPERATOR, channelName);
    um.addUserToChannel(Marvin.socket, USER, channelName);

    if (channelKey.empty() == false)
    {
        Channel *channel = um.getChannel(channelName);
        channel->setPassword(channelKey);
        LOG("Channel " + channelName + " created by " + um.getNickname(socket) + " (password-protected)");
    }
    else
    {
        LOG("Channel " + channelName + " created by " + um.getNickname(socket));
    }

    RPL_JOIN(socket, socket, channelName);
    RPL_NOTOPIC(socket, channelName);
    RPL_NAMREPLY(socket, channelName, um.getChannelNicknames(channelName));
}

void Server::addUserToChannels(int socket, t_vec_str_c &channelNames, t_vec_str_c &channelKeys)
{
    t_vec_str_cit key = channelKeys.begin();

    for (t_vec_str_cit it = channelNames.begin(); it != channelNames.end(); ++it)
    {
        t_str enteredKey;
        if (key != channelKeys.end())
        {
            enteredKey = *key++;
        }

        t_str_c &channelName = *it;
        if (um.checkForChannel(channelName) == false)
        {
            createChannelBy(socket, channelName, enteredKey);
            continue;
        }

        Channel const *channel = um.getChannel(channelName);
        if (channel->isInviteOnly() == true)
        {

            ERR_INVITEONLYCHAN(socket, channelName);
            continue;
        }

        if (channel->isFull() == true)
        {

            ERR_CHANNELISFULL(socket, channelName);
            continue;
        }

        if (enteredKey.empty() == false && channel->isChannelKey() == false)
        {
            LOG_ERR("Received password for non_pw channel");
        }
        else if (channel->isChannelKey() == true)
        {
            if (channel->getPassword() != enteredKey)
            {
                ERR_BADCHANNELKEY(socket, channel->getName());
                continue;
            }
            um.addUserToChannel(socket, USER, channelName);
        }
        else
        {
            um.addUserToChannel(socket, USER, channelName);
        }
        RPL_JOIN(socket, socket, channel->getName());
        broadcast(um.getNickname(socket), channelName, "", m_trail, "JOIN");
        RPL_IFTOPIC(socket, channel->getName(), channel->getTopic());
        RPL_NAMREPLY(socket, channelName, um.getChannelNicknames(channelName));
    }
}

void Server::broadcast(t_str_c &sender, t_str_c &channelName, t_str_c &nicknameKicked, t_str_c &message,
                       t_str_c &command)
{

    int socketSender = um.getSocket(sender);
    Channel const *channel = um.getChannel(channelName);
    t_channel_users members = channel->getUserMap();

    for (t_channel_users_cit it = members.begin(); it != members.end(); ++it)
    {
        t_str_c &nickname = um.getNickname(it->first);
        int socketTarget = um.getSocket(nickname);
        if (sender.empty() == false && sender == nickname)
        {
            continue;
        }
        if (command == "PRIVMSG")
        {
            RPL_PRIVMSG_CHANNEL(socketSender, socketTarget, channelName, message);
        }
        else if (command == "QUIT")
        {
            RPL_QUIT(socketSender, socketTarget, message);
        }
        else if (command == "JOIN")
        {
            RPL_JOIN(socketSender, socketTarget, channelName);
        }
        else if (command == "PART")
        {
            RPL_PART(socketSender, socketTarget, channelName, message);
        }
        else if (command == "NAMREPLY")
        {
            RPL_NAMREPLY(socketTarget, channelName, um.getChannelNicknames(channelName));
        }
        else if (command == "KICK")
        {
            RPL_KICK(socketSender, socketTarget, channelName, nicknameKicked, message);
        }
        else if (command == "NICK")
        {
            RPL_NICKCHANGE(socketSender, socketTarget, um.getNickname(socketSender), nicknameKicked);
        }
        else if (command == "MODE")
        {
            RPL_CHANNELMODEIS(socketSender, socketTarget, channelName, message, nicknameKicked);
        }
    }
}

// -------------------------------------------------------------------------- //