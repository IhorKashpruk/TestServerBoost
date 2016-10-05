//
// Created by igor on 22.06.16.
//

#ifndef TESTSERVERBOOST_CLIENT_H
#define TESTSERVERBOOST_CLIENT_H

#include <boost/bind.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio.hpp>
#include <boost/signals2.hpp>
#include <memory>
#include "TextBuffer.h"

using namespace boost::asio;
using namespace std;


class Client :
               public boost::noncopyable {
    typedef shared_ptr<ip::tcp::socket> sock_ptr;
    typedef boost::system::error_code error_code;
public:
    Client(io_service& service, const string& ID)
            : socket(make_shared<ip::tcp::socket>(service)),
              id(ID), authorized(false){}
    ~Client(){
        boost::system::error_code ec;
        socket->shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
        socket->close();
    }

    void connect(ip::tcp::endpoint ep);
    void close();
    sock_ptr getSocket();
    string getID() const;

    void do_read();
    boost::signals2::signal<void(const string)> eraseSignal;
    boost::signals2::signal<void(shared_ptr<Message> message_ptr, const string client_id)> runQuery;
    bool isAuthorized() const;
    void setAuthorized(bool authorized);
    void setId(const string &id);
private:
    void on_connect(const error_code& err);
    void on_read(const error_code& err, size_t bytes);

    sock_ptr socket;

    string id;
    char my_buffer[1024];
    TextBuffer text_buffer;
    bool authorized;
};


#endif //TESTSERVERBOOST_CLIENT_H
