#include <iostream>
#include <map>

#include "include/cache.h"

void CachingProxy::processCommand(int argc, char* argv[])
{
    std::string port,origin;

    if(argc < 2) return;

    for(int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if(arg == "--port" && i + 1 < argc)
        {
            port = argv[++i];
        }
        else if(arg == "--origin" && i + 1 < argc)
        {
            origin = argv[++i];
        }
        else if(arg == "--claered-cache")
        {
        }
        else
        {
            std::cerr<<"Invalid command";
        }
    }

    if(!port.empty())
    {
        getResponse(origin);
    }
}

std::string CachingProxy::getResponse(const std::string& origin)
{
    std::string cachedResponse = getFromCache(url);

    if(!cachedResponse.empty())
    {
        std::cout<< "X-cache: HIT"<< std::endl;
        return cachedResponse;
    }

    std::string response = fetchFromOrigin(url);
    addToCache(url, response);

    std::cout << "X-cache: MISS" << std::endl;
    return response;


}

std::string CachingProxy::getFromCache(const std::string& url)
{
    if(cache.find(url) != cache.end())
    {
        return cache[url];
    }
    return "";
}

void CachingProxy::addToCache(const std::string url, const std::string &response)
{
    cache[url] = response;
}