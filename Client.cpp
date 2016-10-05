//
// Created by igor on 22.06.16.
//

#include "Client.h"

typedef shared_ptr<ip::tcp::socket> sock_ptr;
typedef boost::system::error_code error_code;

void Client::connect(ip::tcp::endpoint ep) {
    socket->async_connect(ep,
    boost::bind(&Client::on_connect, this, _1));
}

void Client::close() {
    boost::system::error_code ec;
    socket->shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
    socket->close();
    eraseSignal(id);
}

sock_ptr Client::getSocket(){
    return socket;
}

string Client::getID() const {
    return id;
}

void Client::on_connect(const error_code &err) {
    if(!err) do_read();
    else close();
}

void Client::on_read(const error_code &err, size_t bytes) {
    if(!err){

        text_buffer.append(my_buffer);
        memset(my_buffer, 0, 1024);
        string str;
        while((str = text_buffer.getMessage()) != ""){
            shared_ptr<Message> message = text_buffer.getMessageStruct(str);
            if(message != nullptr) {
                runQuery(message, id);
            }
        }
        do_read();
    }
    else {
        close();
    }
}


void Client::do_read() {
    socket->async_read_some(buffer(my_buffer, 1024),
    boost::bind(&Client::on_read, this, _1, _2));
}

void Client::setId(const string &id) {
    Client::id = id;
}

bool Client::isAuthorized() const {
    return authorized;
}

void Client::setAuthorized(bool authorized) {
    Client::authorized = authorized;
}
















