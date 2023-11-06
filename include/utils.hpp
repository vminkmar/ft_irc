// -------------------------------------------------------------------------- //

#ifndef UTILS_HPP
#define UTILS_HPP

#include "typedefs.hpp"

/* converts an integer to a const std::string */
t_str_c itostr(int i);

/* summarizes all elements of a vector (std::string)
 * into one string, divided by a space */
t_str_c sumParameters(t_vec_str_cit start, t_vec_str_cit end);

/* splits a std::string divided by a delimiter
 * and stores it in a vector (std::string) */
t_vec_str split(t_str_c &parameter, char delimiter);

bool inputCheck(int argc);

#endif // UTILS_HPP

// -------------------------------------------------------------------------- //