// -------------------------------------------------------------------------- //

#ifndef TYPEDEFS_HPP
# define TYPEDEFS_HPP

#include <string> // needed for std::string
#include <vector> // needed for std::vector
#include <poll.h> // needed for poll_fd

    /* used as default string */
    typedef std::string               t_str;
    typedef t_str const               t_str_c;
    typedef t_str::const_iterator     t_str_cit;

    /* used to store parsed parameters */
    typedef std::vector<t_str>        t_vec_str;
    typedef t_vec_str const           t_vec_str_c;
    typedef t_vec_str::iterator       t_vec_str_it;
    typedef t_vec_str::const_iterator t_vec_str_cit;

    /* used to store all accepted socket fd */
    typedef std::vector<pollfd>       t_vec_pollfd;
    typedef t_vec_pollfd::iterator    t_vec_pollfd_it;

#endif // TYPEDEFS_HPP

// -------------------------------------------------------------------------- //
