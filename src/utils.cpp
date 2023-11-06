// -------------------------------------------------------------------------- //

#include "../include/utils.hpp" // needed for declaration

#include <iostream> // needed for std::cerr
#include <sstream>  // needed for std::stringstream

t_str_c itostr(int i)
{
    std::stringstream ss;
    ss << i;
    return ss.str();
}

t_str_c sumParameters(t_vec_str_cit start, t_vec_str_cit end)
{
    if (start == end)
    {
        throw std::runtime_error("sumParameters: Wrong iterator given!");
    }
    std::stringstream ss;
    t_vec_str_cit last = end - 1;
    for (t_vec_str_cit it = start; it != end; ++it)
    {
        ss << *it;
        if (it != last)
        {
            ss << " ";
        }
    }
    return ss.str();
}

t_vec_str split(t_str_c &parameter, char delimiter)
{
    t_vec_str split;
    t_str token;
    std::stringstream ss(parameter);

    while (std::getline(ss, token, delimiter))
    {
        split.push_back(token);
    }
    return split;
}

bool inputCheck(int argc)
{
    if (argc != 3)
    {
        return false;
    }
    return true;
}

// -------------------------------------------------------------------------- //