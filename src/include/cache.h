#pragma once

#include <map>

class CachingProxy
{
public:
    void processCommand(int argc, char* argv[]);
    std::string getResponse(const std::string& url);
    std::string getFromCache(const std::string& url);
    void CachingProxy::addToCache(const std::string url, const std::string &response);
private:
    std::map<std::string, std::string> cache; 
};