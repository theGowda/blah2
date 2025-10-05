#include "Socket.h"
#include <iostream>

asio::io_context Socket::io_context;
const uint32_t Socket::MTU = 1024;

Socket::Socket(const std::string& ip, uint16_t port)
    : endpoint(asio::ip::make_address(ip), port), socket(io_context) {
    try {
        socket.connect(endpoint);
    } catch (const std::exception& e) {
        std::cerr << "Warning: Could not connect to " << ip << ":" << port
                  << " - " << e.what() << std::endl;
        std::cerr << "Socket will attempt to reconnect on each send." << std::endl;
    }
}

Socket::~Socket()
{
}

void Socket::sendData(const std::string& data) {
    asio::error_code err;

    for (std::size_t i = 0; i < (data.size() + MTU - 1) / MTU; ++i) {
        std::string subdata = data.substr(i * MTU, MTU);
        socket.write_some(asio::buffer(subdata, subdata.size()), err);

        if (err) {
            std::cerr << "Error sending data: " << err.message() << std::endl;
        }
    }
}
