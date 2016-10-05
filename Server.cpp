//
// Created by igor on 24.07.16.
//

#include "Server.h"

typedef boost::system::error_code err_code;

Server::Server(io_service &service, unsigned int port) : noncopyable(), service_(service), port_(port), input(service, ::dup(STDIN_FILENO))  {
    endpoint_ = make_shared<ip::tcp::endpoint>(ip::tcp::v4(), port);
    acceptor_ = make_shared<ip::tcp::acceptor>(service_, *endpoint_);
}

void Server::start() {
    client_ptr client = make_shared<Client>(service_, "0");
    acceptor_->async_accept(*client->getSocket(), boost::bind(&Server::handler_accept, this, client, _1));
    do_read_input();
}

void Server::handler_accept(client_ptr client, const err_code &errCode) {
    static int id = 0;
    if(errCode){
        printf("Error accept: %s.\n", errCode.message().c_str());
    }else{
        client->do_read();
        clients_.insert(pair<string, client_ptr>(to_string(id), client));
        printf("\nClient accepted!. Count of clients:%d.\n", (int)clients_.size());
        id++;
        client->eraseSignal.connect([&](const string str){
            printf("Erase client [id = %s]\n", str.c_str());
            clients_.erase(str);
        });
        client->runQuery.connect(bind(&Server::runQuery, this, _1, _2));
    }
    client_ptr client2 = make_shared<Client>(service_, to_string(id));
    acceptor_->async_accept(*client2->getSocket(), boost::bind(&Server::handler_accept, this, client2, _1));
}

void Server::runQuery(shared_ptr<Message> message_ptr, const string client_id) {

    istringstream ss(message_ptr->getCode());
    int d;
    ss >> d;

    printf("Cliet: %s\n", client_id.c_str());
    printf("Message: %s\n", message_ptr->getString().c_str());

    string str;   // ПЕРЕРОБИТИ!!!
    client_ptr client = clients_[client_id];

    switch(d){
        case (int) TYPE_MESSAGE::AUTH:
            str = message_ptr->getArguments().substr(0, message_ptr->getArguments().find(','));
            printf("Login: %s\n", str.c_str());
            // Перевірка чи залогований
            if(findClient(str)){
                sendMessage(make_shared<Message>(
                        to_string((int) TYPE_MESSAGE::ANSW),
                        message_ptr->getCode(),
                        "2",
                        "bad login"),
                            client_id);
                break;
            }
            renameClientAndMap(client_id, str);
            clients_[str]->setAuthorized(true);
            sendMessage(make_shared<Message>(
                    to_string((int) TYPE_MESSAGE::ANSW),
                    message_ptr->getCode(),
                    "2",
                    "ok"),
                        str);
            break;
        case (int) TYPE_MESSAGE::SEND:
            if(!client->isAuthorized()) {
                sendMessage(make_shared<Message>(
                        to_string((int) TYPE_MESSAGE::ANSW),
                        message_ptr->getCode(),
                        "19",
                        "client no autorized"), client_id);
                break;
            }
            str = message_ptr->getArguments();  // Може бути не один клієнт!!! ПЕРЕРОБИТИ!!!
            message_ptr->setArguments(client_id);
            sendMessage(message_ptr, str);
            break;
        case (int) TYPE_MESSAGE::CLIST:
            if(!client->isAuthorized()) {
                sendMessage(make_shared<Message>(
                        to_string((int) TYPE_MESSAGE::ANSW),
                        message_ptr->getCode(),
                        "19",
                        "client no autorized"), client_id);
                break;
            }
            for_each(clients_.begin(), clients_.end(), [&](auto& pair1){
               str += pair1.first;
                if(clients_.rbegin()->first != pair1.first)
                    str += ",";
            });
            printf("Client list: %s\n", str.c_str());
            ss.str(str);
            ss >> d;
            sendMessage(make_shared<Message>(
                    to_string((int) TYPE_MESSAGE::ANSW),
                    message_ptr->getCode(),
                    to_string(d),
                    str), client_id);
            break;
        case (int) TYPE_MESSAGE::ANSW:
            break;
        default:
            break;
    }

//    printf("Message: [code = %s, args = %s, text_l = %s, text = %s]\n",
//                       message_ptr->getCode().c_str(),
//                       message_ptr->getArguments().c_str(),
//                       message_ptr->getText_length().c_str(),
//                       message_ptr->getText().c_str());
}


void Server::sendMessage(shared_ptr<Message> message_ptr, const string client_id){
    sendMessage(message_ptr->getString(), client_id);
}
void Server::sendMessage(shared_ptr<Message> message_ptr, shared_ptr<Client> client){
    async_write(*client->getSocket(), buffer(message_ptr->getString()), bind(&Server::on_send_message, this, _1, _2));
}
void Server::sendMessage(string message, const string client_id){
    auto it = clients_.find(client_id);

    if(it != clients_.end()){
        async_write(*it->second->getSocket(), buffer(message), bind(&Server::on_send_message, this, _1, _2));
    }
}

void Server::on_send_message(const err_code& error, size_t length){
    if(error){
        printf("Error send message: %s\n", error.message().c_str());
        return;
    }
    printf("Send success!\n");
}

void Server::renameClientAndMap(const string &previous_name, const string &future_name) {
    auto it = clients_.find(previous_name);
    if(it == clients_.end())
        return;
    it->second->setId(future_name);
    clients_.insert(pair<string, client_ptr>(future_name, it->second));
    clients_.erase(previous_name);
}

bool Server::findClient(const string &name) {
    return clients_.find(name) != clients_.end();
}

void Server::do_read_input() {
    async_read_until(input, input_buffer, '\n', boost::bind(&Server::on_read_input, this, _1, _2));
}

void Server::on_read_input(const err_code &error, size_t length) {
    if(!error){
        ostringstream oss;
        oss << &input_buffer;
        string input_message = oss.str();
        input_message.erase(input_message.length()-1);
        runServerQuery(input_message);
        input_buffer.consume(length);
    } else {
        printf("Error read input: %s\n", error.message().c_str());
    }
    async_read_until(input, input_buffer, '\n', boost::bind(&Server::on_read_input, this, _1, _2));
}

void Server::runServerQuery(const string &str) {
    vector<string> words;
    size_t space = -1, start = -1;
    string sd;
    while((space = str.find(' ', space+1)) != string::npos){
        start += 1;
        if((sd = str.substr(start, space-start)) != "")
            words.push_back(sd);
        start = space;
    }
    words.push_back(str.substr(start+1, str.length() - start));

    LineError lineError("awd");
    LineError lineError2;

    if(lineError){
        printf("Error\n");
    }

    if(lineError2){
        printf("Error2\n");
    }

}

void Server::closeClient(const string &id) {
    auto it = clients_.find(id);
    if(it != clients_.end()){
        clients_.erase(id);
    }
}
