#ifndef CACHING_PROXY_H
#define CACHING_PROXY_H

#include <string>

class CachingProxy
{
   public:
    void startServer(int port, const std::string& origin);
    void clearCache();
    void processCommand(int argc, char* argv[0]);

   private:
    std::string fetchFromOrigin(const std::string& target);
    std::string getCachedResponse(const std::string& target);
    void cacheResponse(const std::string& target, const std::string& response);
};

#endif  // CACHING_PROXY_H
