//
// Created by igor on 24.07.16.
//

#include "TextBuffer.h"

void TextBuffer::append(const string& str){
    data.append(str);
}

string TextBuffer::getMessage(){
    size_t start_p, end_p;
    if((start_p = data.find("\r\r\n")) == string::npos){
        data.clear();
        return "";
    }
    if((end_p = data.find(("\r\r\n"), start_p+1)) == string::npos){
        return "";
    }

    string handler_str = data.substr(start_p+3, end_p-start_p-3);
    data.erase(0, end_p+3);
    return handler_str;
}

shared_ptr<Message> TextBuffer::getMessageStruct(const string& str){
    vector<string> arguments;

    size_t pos1 = 0, pos2 = 0;
    while((pos2 = str.find(';', pos1)) != string::npos){
        arguments.push_back(str.substr(pos1, pos2-pos1));
        pos1 = pos2+1;
    }
    if(arguments.size() != 3)
        return nullptr;
    istringstream ss(arguments.at(2));
    int d;
    ss >> d;

    int n = 0;
    for(auto& it : arguments){
        n += it.size()+1;
    }

    if(str.length()-n > d) {
        return nullptr;
    }

    return make_shared<Message>(arguments[0], arguments[1], arguments[2], str.substr(pos1, str.length()-pos1));
}
