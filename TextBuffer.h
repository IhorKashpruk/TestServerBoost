//
// Created by igor on 24.07.16.
//

#ifndef TESTC_POWER_TEXTBUFFER_H
#define TESTC_POWER_TEXTBUFFER_H

#include <sstream>
#include <memory>
#include <vector>
#include "Message.h"

using namespace std;

class TextBuffer {
private:
    string data;
public:
    TextBuffer(){}
    void append(const string& str);
    string getMessage();
    shared_ptr<Message> getMessageStruct(const string& str);
};


#endif //TESTC_POWER_TEXTBUFFER_H
