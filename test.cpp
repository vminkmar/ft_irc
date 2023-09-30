#include "../include/channel.hpp"

#include <vector>

#include <cstdlib> // needed for MACROS

int main(void)
{
    std::vector<Channel*> channels;
    channels.push_back(new Channel());

    for (size_t i = 0; i < channels.size(); ++i)
    {
        delete channels[i];
    }

    channels.clear();

    return (EXIT_SUCCESS);
}