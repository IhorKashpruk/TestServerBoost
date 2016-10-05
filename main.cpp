#include <iostream>
#include <boost/asio.hpp>
#include <boost/make_shared.hpp>
#include <system_error>
#include <boost/bind.hpp>
#include "Server.h"

//using namespace std;
using namespace boost::asio;
using  namespace boost::asio::posix;
//
///*Properties definition:*/
#define PORT 3333
//typedef shared_ptr<Client> client_ptr;
////typedef shared_ptr<ip::tcp::socket> socket_ptr;
//typedef boost::system::error_code err_code;
//#define MEM_FN_R(func, socket, err, len) boost::bind(&func, socket, err, len)
//#define MEM_FN_ACCEPT(func, socket, err) boost::bind(&func, socket, err)
///*--Properties definition--*/
//
///*Global objects:*/
//
//ip::tcp::endpoint endpoint(ip::tcp::v4(), PORT);
//ip::tcp::acceptor acceptor(ioService, endpoint);
//vector<client_ptr> clients;
////stream_descriptor out(ioService, ::dup(STDOUT_FILENO));
///*--End global objects--*/
io_service ioService;

int main()
{

    Server server(ioService, PORT);
    server.start();
    ioService.run();

    return 0;
}