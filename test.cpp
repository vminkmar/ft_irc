
#include <vector>              // needed for std::vector
#include <cstdlib>             // needed for MACROS
#include "include/Channel.hpp" // needed for Channel class

void channels_delete(std::vector<Channel*>& c)
{
	for (size_t i = 0; i < c.size(); ++i)
	{
		delete c[i];
	}
}

int main(void)
{
    std::vector<Channel*> channels;
    channels.push_back(new Channel());

	channels_delete(channels);

    channels.clear();

	/* i guess it would be better to have also a Channels class
	 * which contains all channels */

    return (EXIT_SUCCESS);
}
