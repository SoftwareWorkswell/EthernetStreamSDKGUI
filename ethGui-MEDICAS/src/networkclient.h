#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H


#include <cstdint>
#include <string>
#include <memory>
#include <chrono>
#include <boost/asio.hpp>

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}




class NetworkClient
{
public:
    NetworkClient(const std::string &host, const std::string &port);
    NetworkClient(const NetworkClient&) = delete;
    NetworkClient& operator=(const NetworkClient&) = delete;
    ~NetworkClient();

    void connect();
    void disconnect();

    int write(const uint8_t *buffer, std::size_t size);
    int write(const std::string &buffer);

    int wait(std::chrono::steady_clock::duration timeout);
    int available();
    int read_some(uint8_t *buffer, std::size_t size);
    int read_some_wait(uint8_t *buffer, std::size_t size);
    int read_exactly(uint8_t *buffer, std::size_t size, std::chrono::steady_clock::duration timeout);
    int read_until(std::string &buffer, char delim, std::chrono::steady_clock::duration timeout);

    operator bool() const { return connected; }
    bool is_connected() const { return connected; }

    std::string get_host() const { return host; }
    std::string get_port() const { return port; }

    void set_debug(bool debug) { this->debug = debug;  }
    bool get_debug() const { return debug;  }

protected:
    bool connected = false;
    std::string host;
    std::string port;
    bool debug = false;

    std::shared_ptr<boost::asio::io_context> io_context;
    std::unique_ptr<boost::asio::ip::tcp::resolver> resolver;
    std::unique_ptr<boost::asio::ip::tcp::socket> socket;

    bool run_for(std::chrono::steady_clock::duration timeout); /// Run operation with timeout
    bool run_until(const std::chrono::steady_clock::time_point &timepoint); /// Run operation until timepoint
};

#endif // NETWORKCLIENT_H

