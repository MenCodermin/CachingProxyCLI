#include "include/cache.h"

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <iostream>
#include <map>
#include <mutex>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

// Shared cache and mutex
std::map<std::string, std::string> cache;
std::mutex cacheMutex;
std::string originServer;

void CachingProxy::processCommand(int argc, char* argv[0])
{
    std::string port, origin;
    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg == "--port" && i + 1 < argc)
        {
            port = argv[++i];
        }
        else if (arg == "--origin" && i + 1 < argc)
        {
            origin = argv[++i];
        }
        else if (arg == "--clear-cache")
        {
            clearCache();
        }
        else
        {
            std::cerr << "Invalid command: " << arg << std::endl;
        }
    }

    if (!port.empty() && !origin.empty())
    {
        int portNum = std::stoi(port);
        startServer(portNum, origin);
    }
    else
    {
        std::cerr << "Missing --port or --origin parameters." << std::endl;
    }
}

void CachingProxy::startServer(int port, const std::string& origin)
{
    originServer = origin;

    try
    {
        net::io_context ioc;
        tcp::acceptor acceptor(ioc, tcp::endpoint(tcp::v4(), port));
        std::cout << "Proxy server started on port " << port << " forwarding to " << origin
                  << std::endl;

        while (true)
        {
            tcp::socket socket(ioc);
            acceptor.accept(socket);

            // Process the request
            beast::flat_buffer buffer;
            http::request<http::string_body> req;
            http::read(socket, buffer, req);

            // Fetch or cache response
            std::string target(req.target().data(), req.target().size());
            std::string response = getCachedResponse(target);

            // Send the response
            http::response<http::string_body> res{http::status::ok, req.version()};
            res.set(http::field::server, "CachingProxy");
            res.set(http::field::content_type, "text/plain");
            res.body() = response;
            res.prepare_payload();

            http::write(socket, res);
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void CachingProxy::clearCache()
{
    std::lock_guard<std::mutex> lock(cacheMutex);
    cache.clear();
    std::cout << "Cache cleared successfully!" << std::endl;
}

std::string CachingProxy::getCachedResponse(const std::string& target)
{
    std::lock_guard<std::mutex> lock(cacheMutex);
    if (cache.find(target) != cache.end())
    {
        std::cout << "X-Cache: HIT\n";
        return cache[target];
    }

    std::cout << "X-Cache: MISS\n";
    std::string response = fetchFromOrigin(target);
    cache[target] = response;
    return response;
}

std::string CachingProxy::fetchFromOrigin(const std::string& target)
{
    try
    {
        net::io_context ioc;
        tcp::resolver resolver(ioc);
        auto const results = resolver.resolve(originServer, "80");

        beast::tcp_stream stream(ioc);
        stream.connect(results);

        http::request<http::string_body> req{http::verb::get, target, 11};
        req.set(http::field::host, originServer);
        req.set(http::field::user_agent, "CachingProxy/1.0");

        http::write(stream, req);

        beast::flat_buffer buffer;
        http::response<http::string_body> res;
        http::read(stream, buffer, res);

        return res.body();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error fetching from origin: " << e.what() << std::endl;
        return "Error: Unable to fetch response.";
    }
}
