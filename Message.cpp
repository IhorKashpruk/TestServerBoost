//
// Created by igor on 24.07.16.
//

#include "Message.h"

Message::Message(const string& c, const string& args, const string& tex_l, const string& tex)
        : code(c), arguments(args), text_length(tex_l), text(tex){}

string Message::getString(){
    return code+";"+arguments+";"+text_length+";"+text;
}