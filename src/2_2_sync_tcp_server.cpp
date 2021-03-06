#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

std::string make_daytime_string()
{
    using namespace std; // for time_t, time and ctime;
    time_t now = time(0);
#if defined _WIN32
    char str[126];
    ctime_s(str, sizeof(str), &now);
    return str;
#else
    return ctime(&now);
#endif
}

int main()
{
    try
    {
        boost::asio::io_context io_context;
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 13));

        for (;;)
        {
            tcp::socket socket(io_context);
            acceptor.accept(socket);
            std::string message = make_daytime_string();

            boost::system::error_code ignored_error;
            boost::asio::write(socket,
                               boost::asio::buffer(message),
                               ignored_error);
        }
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
