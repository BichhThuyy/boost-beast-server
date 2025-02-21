// boost_beast_server.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
namespace asio = boost::asio;
using tcp = asio::ip::tcp;

void handle_request(tcp::socket& socket) {
    try {
        beast::flat_buffer buffer;

        // Read the HTTP request
        http::request<http::string_body> req;
        http::read(socket, buffer, req);

        // Prepare a simple HTTP response
        http::response<http::string_body> res;
        http::status::ok, req.version();
        std::cout << "Request: " << req << std::endl;

        if (req.target() == "/json") {
            res.result(http::status::ok);
            res.set(http::field::content_type, "application/json");
            res.body() = R"({"message": "Hello, JSON!"})";
        }
        else {
            res.set(http::field::content_type, "text/plain");
            res.body() = "Hello, World!";
        }

        // Send the response
        res.prepare_payload();
        http::write(socket, res);
    } catch (std::exception& e) {
        std::cerr << "Request handling error: " << e.what() << std::endl;
    }
};

// Function to start the HTTP server
void run_server(asio::io_context& ioc, unsigned short port) {
    tcp::acceptor acceptor{ ioc, tcp::endpoint(tcp::v4(), port) };

    std::cout << "Server is running on port " << port << "...\n";

    while (true) {
        tcp::socket socket{ ioc };
        acceptor.accept(socket);
        handle_request(socket);
    }
};

int main()
{
    asio::io_context ioc;
    run_server(ioc, 8080);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
