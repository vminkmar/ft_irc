#ifndef USERPRIVILEGE_HPP
# define USERPRIVILEGE_HPP

/* @note could put this into UserManagement.hpp i guess */

/* @note Possibly need a third Privilege (restriction of TOPIC command) */
	/* @note or could do this with a separate list in each channel */

/* Enumeration for the rights of a user in a channel */
enum UserPrivilege
{
	USER,
	OPERATOR,
};

#endif // USERPRIVILEGE
