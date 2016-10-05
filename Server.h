//
// Created by igor on 24.07.16.
//

#ifndef TESTSERVERBOOST_SERVER_H
#define TESTSERVERBOOST_SERVER_H

#include "Client.h"
#include "InputLine.h"

using namespace std;
using namespace boost::asio;
using namespace boost::asio::posix;


class Server :public boost::noncopyable{
    enum class TYPE_MESSAGE{
        AUTH = 0,   // authorization    [\r\r\n0;login,password;0;\r\r\n]
        SEND = 1,   // send             [\r\r\n1;Igor;11;Hello world\r\r\n]
        CLIST = 2,  // client list      [\r\r\n2;;;\r\r\n]
        ANSW = 23   // answer           [\r\r\n23;0;2;ok\r\r\n] // 0(AUTH)
    };
    typedef shared_ptr<Client> client_ptr;
    typedef boost::system::error_code err_code;
public:
    Server(io_service &service, unsigned int port);
    void start();
private:
    io_service& service_;
    shared_ptr<ip::tcp::acceptor> acceptor_;
    shared_ptr<ip::tcp::endpoint> endpoint_;
    stream_descriptor input;
    int port_;
    map<string, client_ptr> clients_;
    boost::asio::streambuf input_buffer;


    void sendMessage(shared_ptr<Message> message_ptr, const string client_id);
    void sendMessage(shared_ptr<Message> message_ptr, shared_ptr<Client> client);
    void sendMessage(string message, const string client_id);
    void renameClientAndMap(const string& previous_name, const string& future_name);
    bool findClient(const string& name);
    void do_read_input();
    void runServerQuery(const string& str);
    void closeClient(const string& id);

    // SLOT
    void runQuery(shared_ptr<Message> message_ptr, const string client_id);

    // Handlers
    void on_send_message(const err_code& error, size_t length);
    void handler_accept(client_ptr client, const err_code& errCode);
    void on_read_input(const err_code& error, size_t length);
};


#endif //TESTSERVERBOOST_SERVER_H
