// -------------------------------------------------------------------------- //

#include "../include/class.UserManagement.hpp" // needed for UserManagement

#include <sstream>   // needed for std::stringstream
#include <stdexcept> // needed for std::runtime_error

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> user map operations */

void UserManagement::addUser(int socket)
{
    m_users[socket] = User(); /* c11 std::map.emplace */
    setOnlineStatus(socket, ONLINE);
    setRegisteredStatus(socket, false);
}

void UserManagement::eraseUser(int socket)
{
    for (t_um_channels_it it = m_channels.begin(); it != m_channels.end(); ++it)
    {
        eraseUserFromChannel(socket, it->first);
    }
    t_um_users_it user = m_users.find(socket);
    if (user != m_users.end())
    {
        m_users.erase(user);
        return;
    }
    throw std::runtime_error("eraseUser: User not found!");
}

bool UserManagement::checkForUser(int socket) const
{
    return m_users.find(socket) != m_users.end();
}

bool UserManagement::checkForUsername(t_str_c &username) const
{
    for (t_um_users_cit it = m_users.begin(); it != m_users.end(); ++it)
    {
        if (it->second.getUsername() == username)
        {
            return true;
        }
    }
    return false;
}

bool UserManagement::checkForNickname(t_str_c &nickname) const
{
    for (t_um_users_cit it = m_users.begin(); it != m_users.end(); ++it)
    {
        if (it->second.getNickname() == nickname)
        {
            return true;
        }
    }
    return false;
}

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> user map setters */

void UserManagement::setUsername(int socket, t_str_c &newUsername)
{
    t_um_users_it it = m_users.find(socket);
    if (it != m_users.end())
    {
        it->second.setUsername(newUsername);
        return;
    }
    throw std::runtime_error("setUsername: User not found!");
}

void UserManagement::setNickname(int socket, t_str_c &newNickname)
{
    t_um_users_it it = m_users.find(socket);
    if (it != m_users.end())
    {
        it->second.setNickname(newNickname);
        return;
    }
    throw std::runtime_error("setNickname: User not found!");
}

void UserManagement::setOnlineStatus(int socket, bool flag)
{

    t_um_users_it it = m_users.find(socket);
    if (it != m_users.end())
    {
        if (it->second.getOnlineStatus() != flag)
        {
            it->second.toggleOnlineStatus();
        }
        return;
    }
    throw std::runtime_error("setOnlineStatus: User not found!");
}

void UserManagement::setWelcomedStatus(int socket, bool flag)
{
    t_um_users_it it = m_users.find(socket);
    if (it != m_users.end())
    {
        if (it->second.getWelcomedStatus() != flag)
        {
            it->second.toggleWelcomedStatus();
        }
        return;
    }
    throw std::runtime_error("setWelcomedStatus: User not found!");
}

void UserManagement::setRegisteredStatus(int socket, bool flag)
{
    t_um_users_it it = m_users.find(socket);
    if (it != m_users.end())
    {
        it->second.setRegisteredStatus(flag);
        return;
    }
    throw std::runtime_error("setRegisteredStatus: User not found!");
}

void UserManagement::appendToBuffer(int socket, t_str_c &message, int flag)
{
    if (flag == INPUT)
    {
        m_users.find(socket)->second.appendInputBuffer(message);
        return;
    }
    else if (flag == OUTPUT)
    {
        m_users.find(socket)->second.appendOutputBuffer(message);
        return;
    }
    throw std::runtime_error("appendToBuffer: Wrong flagen given!");
}

void UserManagement::eraseBuffer(int socket, int start, int end, int flag)
{
    if (flag == INPUT)
    {
        m_users.find(socket)->second.eraseInputBuffer(start, end);
        return;
    }
    else if (flag == OUTPUT)
    {
        m_users.find(socket)->second.eraseOutputBuffer(start, end);
        return;
    }
    throw std::runtime_error("eraseBuffer: Wrong flag given!");
}

/* <~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~> user map getters */

int UserManagement::getSocket(t_str_c &nickname) const
{
    for (t_um_users_cit it = m_users.begin(); it != m_users.end(); ++it)
    {
        if (it->second.getNickname() == nickname)
        {
            return it->first;
        }
    }
    throw std::runtime_error("getSocket: User not found!");
}

t_str UserManagement::getNickname(int socket) const
{
    t_um_users_cit it = m_users.find(socket);
    if (it != m_users.end())
    {
        return it->second.getNickname();
    }
    throw std::runtime_error("getNickname: User not found!");
}

t_str UserManagement::getNicknames() const
{
    if (m_users.empty())
    {
        return "";
    }
    std::stringstream ss;
    t_um_users_cit last = --m_users.end();
    for (t_um_users_cit it = m_users.begin(); it != m_users.end(); ++it)
    {
        ss << it->second.getNickname();
        if (it != last)
        {
            ss << ", ";
        }
    }
    return ss.str();
}

t_str UserManagement::getUsername(int socket) const
{
    t_um_users_cit it = m_users.find(socket);
    if (it != m_users.end())
    {
        return it->second.getUsername();
    }
    throw std::runtime_error("getUsername: User not found!");
}

t_str UserManagement::getUsernames() const
{
    if (m_users.empty())
    {
        return "";
    }
    std::stringstream ss;
    t_um_users_cit last = --m_users.end();
    for (t_um_users_cit it = m_users.begin(); it != m_users.end(); ++it)
    {
        ss << it->second.getUsername();
        if (it != last)
        {
            ss << ", ";
        }
    }
    return ss.str();
}

t_str UserManagement::getBuffer(int socket, int flag) const
{
    if (flag == INPUT)
    {
        return m_users.find(socket)->second.getInputBuffer();
    }
    else if (flag == OUTPUT)
    {
        return m_users.find(socket)->second.getOutputBuffer();
    }
    throw std::runtime_error("getBuffer: Wrong flag given!");
}

bool UserManagement::getOnlineStatus(int socket) const
{
    t_um_users_cit it = m_users.find(socket);
    if (it != m_users.end())
    {
        return it->second.getOnlineStatus();
    }
    throw std::runtime_error("getOnlineStatus: User not found!");
}

bool UserManagement::getWelcomedStatus(int socket) const
{
    t_um_users_cit it = m_users.find(socket);
    if (it != m_users.end())
    {
        return it->second.getWelcomedStatus();
    }
    throw std::runtime_error("getWelcomedStatus: User not found!");
}

User *UserManagement::getUser(int socket)
{
    t_um_users_it it = m_users.find(socket);
    if (it != m_users.end())
    {
        return &(m_users.find(socket)->second);
    }
    throw std::runtime_error("getUser: User not found!");
}

bool UserManagement::getRegisteredStatus(int socket) const
{
    t_um_users_cit it = m_users.find(socket);
    if (it != m_users.end())
    {
        return it->second.getRegisteredStatus();
    }
    throw std::runtime_error("getUser: User not found!");
}
// -------------------------------------------------------------------------- //
