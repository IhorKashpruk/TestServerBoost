//
// Created by igor on 24.07.16.
//

#ifndef TESTC_POWER_MESSAGE_H
#define TESTC_POWER_MESSAGE_H

#include <string>

using namespace std;

class Message
{
public:
    const string &getCode() const {
        return code;
    }

    const string &getArguments() const {
        return arguments;
    }

    void setCode(const string &code) {
        Message::code = code;
    }

    void setArguments(const string &arguments) {
        Message::arguments = arguments;
    }

    void setText_length(const string &text_length) {
        Message::text_length = text_length;
    }

    void setText(const string &text) {
        Message::text = text;
    }

    const string &getText_length() const {
        return text_length;
    }

    const string &getText() const {
        return text;
    }

private:
    string code;
    string arguments;
    string text_length;
    string text;
public:
    Message(const string& c, const string& args, const string& tex_l, const string& tex);
    string getString();
};


#endif //TESTC_POWER_MESSAGE_H
