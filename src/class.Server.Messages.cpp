// -------------------------------------------------------------------------- //

#include "../include/class.Server.hpp" // needed for Server class
#include "../include/utils.hpp"        // needed for itostr(), split()

#include <stdlib.h> // needed for atoi() (linux)

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> server messages */

void Server::CMD_CAP(int socket)
{
    if (m_parameters[0] == "LS")
    {
        RPL_CAP(socket);
    }
}

void Server::CMD_NICK(int socket)
{

    if (m_parameters.empty() == true)
    {
        ERR_NONICKNAMEGIVEN(socket);
    }
    t_str_c &newNickname = m_parameters[0];
    if (hasUnallowedChar(newNickname, CHAR_UNALLOWED_NICK) == true)
    {
        ERR_ERRONEUSNICKNAME(socket, newNickname);
    }
    else if (um.checkForNickname(newNickname) == true)
    {
        ERR_NICKNAMEINUSE(socket, newNickname);
    }
    else
    {
        t_str_c oldNickname = um.getNickname(socket);
        if (oldNickname.empty() == false)
        {
            RPL_NICKCHANGE(socket, socket, newNickname, oldNickname);
        }
        um.setNickname(socket, newNickname);
        if (um.getUsername(socket).empty() == false && um.getWelcomedStatus(socket) == false)
        {
            RPL_WELCOME(socket, um.getUsername(socket));
            um.setWelcomedStatus(socket, true);
            return;
        }
        t_vec_str_c channels = split(um.getChannelNames(), ',');
        for (t_vec_str_cit it = channels.begin(); it != channels.end(); ++it)
        {
            t_str_c &channelName = *it;
            if (um.getChannel(channelName)->isMember(socket) == true)
            {
                broadcast(um.getNickname(socket), channelName, oldNickname, "", "NICK");
            }
        }
    }
}

void Server::CMD_USER(int socket)
{

    if (m_parameters.empty() == true)
    {
        ERR_NEEDMOREPARAMS(socket, m_command);
    }

    if (um.checkForUser(socket) == true && um.getUsername(socket).empty() == false)
    {
        ERR_ALREADYREGISTRED(socket);
    }
    else
    {
        t_str_c &username = m_parameters[0];
        if (um.getNickname(socket) != "")
        {
            RPL_WELCOME(socket, username);
            um.setWelcomedStatus(socket, true);
        }
        um.setUsername(socket, username);
    }
}

void Server::CMD_PING(int socket)
{
    t_str_c &servername = m_parameters[0];
    RPL_PING(socket, servername);
}

void Server::CMD_QUIT(int socket)
{
    um.setOnlineStatus(socket, OFFLINE);
    LOG("Socket #" + itostr(socket) + " has gone offline (" + um.getNickname(socket) + ")");

    t_vec_str_c channels = split(um.getChannelNames(), ',');
    for (t_vec_str_cit it = channels.begin(); it != channels.end(); ++it)
    {
        t_str_c &channelName = *it;
        if (um.getChannel(channelName)->isMember(socket) == true)
        {
            um.eraseUserFromChannel(socket, channelName);
            broadcast(um.getNickname(socket), channelName, "", m_trail, "QUIT");
        }
    }
    RPL_QUIT(socket, socket, DEFMSG_PART);
}

void Server::CMD_JOIN(int socket)
{

    if (m_parameters.empty() == true)
    {
        ERR_NEEDMOREPARAMS(socket, m_command);
        return;
    }

    if (m_parameters[0] == "0")
    {
        eraseUserFromAllChannels(socket);
        return;
    }

    t_vec_str channelNames = split(m_parameters[0], ',');
    t_vec_str channelKeys;
    if (m_parameters.size() >= 2)
    {
        channelKeys = split(m_parameters[1], ',');
    }

    addUserToChannels(socket, channelNames, channelKeys);
}

void Server::CMD_PART(int socket)
{

    if (m_parameters.empty() == true)
    {
        ERR_NEEDMOREPARAMS(socket, m_command);
        return;
    }

    t_vec_str channels = split(m_parameters[0], ',');

    t_str_c &partMessage = getPartMessage();
    t_str_c &nickname = um.getNickname(socket);

    for (t_vec_str_cit it = channels.begin(); it != channels.end(); ++it)
    {
        t_str_c &channelName = *it;

        if (um.checkForChannel(channelName) == false)
        {
            ERR_NOSUCHCHANNEL(socket, channelName);
            continue;
        }
        Channel const *channel = um.getChannel(channelName);
        if (channel->isMember(socket) == false)
        {
            ERR_NOTONCHANNEL(socket, channelName);
            continue;
        }
        um.eraseUserFromChannel(socket, channelName);
        RPL_PART(socket, socket, channelName, partMessage);
        broadcast(nickname, channelName, "", partMessage, "PART");
    }
}

void Server::CMD_PRIVMSG(int socket)
{

    if (m_parameters.empty() == true)
    {
        ERR_NORECIPIENT(socket, m_command);
        return;
    }

    if (m_trail.empty() == true)
    {
        ERR_NOTEXTTOSEND(socket);
        return;
    }

    t_str_c &target = m_parameters[0];

    if (um.checkForNickname(target) == true)
    {
        if (target == "Marvin")
        {
            RPL_PRIVMSG(socket, target, m_trail);

            if (m_trail == "!server")
            {
                RPL_PRIVMSG(Marvin.socket, um.getNickname(socket),
                            "Number of Users on server: " + itostr(m_pollfds.size() - 2));
            }
            else
            {
                RPL_PRIVMSG(Marvin.socket, um.getNickname(socket), "I am Marvin! Ask me anything...");
            }
        }
        else
        {
            RPL_PRIVMSG(socket, target, m_trail);
        }
        return;
    }
    else if (um.checkForChannel(target) == true)
    {
        Channel const *channel = um.getChannel(target);
        if (channel->isMember(socket) == false)
        {
            ERR_NOTONCHANNEL(socket, target);
        }
        else
        {
            if (m_trail == "!operator")
            {
                t_str_c msg = "Operator: " + um.getChannelOperatorNicknames(target);
                broadcast(Marvin.self->getNickname(), target, "", msg, "PRIVMSG");
            }
            else
            {
                broadcast(um.getNickname(socket), target, "", m_trail, "PRIVMSG");
            }
        }
        return;
    }
    ERR_NOSUCHNICK(socket, target);
}

void Server::CMD_TOPIC(int socket)
{

    if (m_parameters.empty())
    {
        ERR_NEEDMOREPARAMS(socket, m_command);
        return;
    }

    t_str_c channelName = m_parameters[0];
    Channel *channel = um.getChannel(channelName);

    if (channel->isMember(socket) == false)
    {
        ERR_NOTONCHANNEL(socket, channelName);
        return;
    }

    if (m_trail.empty() == true)
    {
        RPL_IFTOPIC(socket, channelName, channel->getTopic());
        return;
    }

    t_str_c topic = m_trail;

    if (channel->isTopicEditable() == false)
    {
        if (channel->isOperator(socket) == false)
        {
            ERR_CHANOPRIVSNEEDED(socket, channelName);
            return;
        }
    }
    channel->setTopic(topic);
    RPL_TOPIC(socket, channelName, topic);
}

void Server::CMD_INVITE(int socket)
{

    if (m_parameters.empty() == true || m_parameters.size() < 2)
    {
        ERR_NEEDMOREPARAMS(socket, m_command);
        return;
    }

    t_str_c nicknameTarget = m_parameters[0];
    if (um.checkForNickname(nicknameTarget) == false)
    {
        ERR_NOSUCHNICK(socket, nicknameTarget);
        return;
    }
    int socketTarget = um.getSocket(nicknameTarget);

    t_str_c channelName = m_parameters[1];

    if (um.checkForChannel(channelName) == false)
    {

        um.addChannel(channelName);
        um.addUserToChannel(socket, OPERATOR, channelName);
        um.addUserToChannel(socketTarget, OPERATOR, channelName);

        RPL_JOIN(socket, socket, channelName);
        RPL_NOTOPIC(socket, channelName);
        RPL_JOIN(socketTarget, socketTarget, channelName);
        RPL_NOTOPIC(socketTarget, channelName);
        RPL_NAMREPLY(socket, channelName, nicknameTarget);
        RPL_NAMREPLY(socketTarget, channelName, um.getNickname(socket));
    }
    else
    {

        Channel *channel = um.getChannel(channelName);

        if (channel->isMember(socket) == false)
        {
            ERR_NOTONCHANNEL(socket, channelName);
            return;
        }
        if (channel->isMember(socketTarget) == true)
        {
            ERR_USERONCHANNEL(socket, nicknameTarget, channelName);
            return;
        }
        if (channel->isInviteOnly() == true)
        {
            if (channel->isOperator(socket) == false)
            {
                ERR_CHANOPRIVSNEEDED(socket, channelName);
                return;
            }
        }
        um.addUserToChannel(socketTarget, USER, channelName);
        RPL_JOIN(socketTarget, socketTarget, channelName);
        RPL_IFTOPIC(socketTarget, channelName, channel->getTopic());

        t_str_c channelMembers = um.getChannelNicknames(channelName);
        RPL_NAMREPLY(socketTarget, channelName, channelMembers);

        broadcast(nicknameTarget, channelName, "", "", "JOIN");
    }

    RPL_INVITING(socket, socketTarget, channelName, nicknameTarget);
    RPL_INVITING(socket, socket, channelName, nicknameTarget);
}

void Server::CMD_KICK(int socketSender)
{

    if (m_parameters.size() < 2)
    {
        ERR_NEEDMOREPARAMS(socketSender, m_command);
    }

    t_vec_str_c channelNames = split(m_parameters[0], ',');
    t_vec_str_c targets = split(m_parameters[1], ',');

    for (t_vec_str_cit it = channelNames.begin(); it != channelNames.end(); ++it)
    {

        t_str_c channelName = *it;

        if (um.checkForChannel(channelName) == false)
        {
            ERR_NOSUCHCHANNEL(socketSender, channelName);
            continue;
        }

        Channel const *channel = um.getChannel(channelName);

        if (channel->isMember(socketSender) == false)
        {
            ERR_NOTONCHANNEL(socketSender, channelName);
            continue;
        }

        if (channel->isOperator(socketSender) == false)
        {
            ERR_CHANOPRIVSNEEDED(socketSender, channelName);
            continue;
        }

        for (t_vec_str_cit itr = targets.begin(); itr != targets.end(); ++itr)
        {

            t_str_c nicknameKicked = *itr;

            if (um.checkForNickname(nicknameKicked) == false)
            {
                ERR_NOSUCHNICK(socketSender, nicknameKicked);
                continue;
            }
            else
            {
                int socketTarget = um.getSocket(nicknameKicked);
                if (channel->isMember(socketTarget) == false)
                {
                    ERR_USERNOTINCHANNEL(socketSender, socketTarget, channelName);
                    continue;
                }

                um.eraseUserFromChannel(socketTarget, channelName);
                if (socketSender != socketTarget)
                {
                    RPL_KICK(socketSender, socketTarget, channelName, nicknameKicked, m_trail);
                }
                RPL_KICK(socketSender, socketSender, channelName, nicknameKicked, m_trail);
                broadcast(um.getNickname(socketSender), channelName, nicknameKicked, m_trail, "KICK");
            }
        }
    }
}

void Server::CMD_MODE(int socket)
{
    if (m_parameters.size() < 2)
    {
        ERR_NEEDMOREPARAMS(socket, m_command);
        return;
    }

    t_str_c &channelName = m_parameters[0];

    if (um.checkForChannel(channelName) == false)
    {
        ERR_NOSUCHCHANNEL(socket, channelName);
        return;
    }

    Channel *channel = um.getChannel(channelName);
    if (channel->isOperator(socket) == false)
    {
        if (channel->isMember(socket) == false)
        {
            ERR_NOTONCHANNEL(socket, channelName);
            return;
        }
        ERR_CHANOPRIVSNEEDED(socket, channelName);
        return;
    }

    for (t_vec_str_cit it = m_parameters.begin() + 1; it != m_parameters.end(); ++it)
    {

        t_str str = *it;

        if (it->find_first_of("+-") != 0)
        {
            ERR_UNKNOWNMODE(socket, str[0], m_parameters[0]);
            return;
        }

        t_str substr = str.substr(1, str.size());
        size_t pos = substr.find_first_not_of(CHAR_ALLOWED_MODS);

        if (pos != std::string::npos)
        {
            ERR_UNKNOWNMODE(socket, substr[pos], m_parameters[0]);
            return;
        }

        char plusorminus = str[0];

        for (t_str_cit sit = str.begin() + 1; sit != str.end(); ++sit)
        {

            char modechar = *sit;

            if (modechar == 'i')
            {
                if ((plusorminus == '-' && channel->isInviteOnly() == true) ||
                    (plusorminus == '+' && channel->isInviteOnly() == false))
                {
                    channel->toggleInviteOnly();
                }
            }
            else if (modechar == 't')
            {
                if ((plusorminus == '-' && channel->isTopicEditable() == true) ||
                    (plusorminus == '+' && channel->isTopicEditable() == false))
                {
                    channel->toggleTopicEditable();
                }
            }
            else if (modechar == 'k')
            {
                if (plusorminus == '+')
                {
                    if ((it + 1) == m_parameters.end())
                    {
                        ERR_NEEDMOREPARAMS(socket, m_command);
                        continue;
                    }
                    channel->setPassword(*(it + 1));
                    ++it;
                }
                else
                {
                    channel->setPassword("");
                }
            }
            else if (modechar == 'o')
            {
                if ((it + 1) == m_parameters.end())
                {
                    ERR_NEEDMOREPARAMS(socket, m_command);
                    continue;
                }

                ++it;
                t_str_c &target = *(it);

                if (um.checkForNickname(target) == false)
                {
                    ERR_NOSUCHNICK(socket, target);
                    continue;
                }
                int socketTarget = um.getSocket(target);
                if (channel->isMember(socketTarget) == false)
                {
                    ERR_USERNOTINCHANNEL(socket, socketTarget, channelName);
                    continue;
                }

                if (plusorminus == '-')
                {
                    um.addUserToChannel(socketTarget, USER, channelName);
                }
                else if (plusorminus == '+')
                {
                    um.addUserToChannel(socketTarget, OPERATOR, channelName);
                }
            }
            else if (modechar == 'l')
            {
                if (plusorminus == '-')
                {
                    channel->setUserLimit(CHANNEL_USERLIMIT);
                }
                else
                {
                    if ((it + 1) == m_parameters.end())
                    {
                        ERR_NEEDMOREPARAMS(socket, m_command);
                        continue;
                    }

                    ++it;
                    t_str_c &target = *it;

                    if (target.find_first_not_of("0123456789") != std::string::npos)
                    {
                        t_str_c errMsg = substr + " " + target;
                        ERR_MODEWRONGPARAM(socket, channelName, substr + " " + target);
                        continue;
                    }

                    int newUserLimit = atoi(target.c_str());

                    channel->setUserLimit(newUserLimit);
                }
            }

            t_str modechange = std::string(1, plusorminus) + modechar;
            t_str param;
            if (it != m_parameters.end())
            {
                param = *(it);
            }
            if (param == modechange)
            {
                param = "";
            }
            RPL_CHANNELMODEIS(socket, socket, channelName, modechange, param);
            t_str msg = "Channel mode changed with: " + modechange + " " + param;
            broadcast(Marvin.self->getNickname(), channelName, "", msg, "PRIVMSG");
        }
    }
}

// -------------------------------------------------------------------------- //