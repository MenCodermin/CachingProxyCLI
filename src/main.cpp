#include <iostream>
#include <string>

#include "include/cache.h"

int main(int argc, char* argv[])
{
    CachingProxy proxy;

    if (argc < 2)
    {
        std::cerr << "Usage: caching-proxy --port <number> --origin <url> | --clear-cache"
                  << std::endl;
        return 1;
    }
    proxy.processCommand(argc, argv);

    return 0;
}
