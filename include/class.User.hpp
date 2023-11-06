// -------------------------------------------------------------------------- //

#ifndef CLASS_USER_HPP
# define CLASS_USER_HPP

#include "typedefs.hpp" // needed for Types, typedefs

class User{

    private:

        t_str m_nickname;
        t_str m_username;
        t_str m_inputBuffer;
        t_str m_outputBuffer;
        bool  m_onlineStatus;
        bool  m_welcomed;
		bool  m_registeredStatus;

    public:

        /* <------ constructors -----> */
        User();
        User& operator=(const User& src);
        User(const User &copy);
        ~User();
        
        /* <------ setters -----> */
        void setNickname         (t_str_c& nickname);
        void setUsername         (t_str_c& username);
		void setRegisteredStatus (bool flag);
        void appendOutputBuffer  (t_str_c& message);
        void appendInputBuffer   (t_str_c& message);
        void eraseOutputBuffer   (int start, int end);
        void eraseInputBuffer    (int start, int end);
        void toggleOnlineStatus  ();
        void toggleWelcomedStatus();

        /* <------ getters -----> */
        t_str_c& getNickname()        const;
        t_str_c& getUsername()        const;
        t_str_c& getInputBuffer()     const;
        t_str_c& getOutputBuffer()    const;
        bool     getOnlineStatus()    const;
        bool     getWelcomedStatus()  const;
		bool	 getRegisteredStatus() const;
};

std::ostream& operator<<(std::ostream &os, const User &user);

#endif // CLASS_USER_HPP

// -------------------------------------------------------------------------- //
