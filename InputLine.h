//
// Created by igor on 29.07.16.
//

#ifndef TESTSERVERBOOST_INPUTLINE_H
#define TESTSERVERBOOST_INPUTLINE_H


#include <string>
#include <set>
#include <vector>
#include <memory>

using namespace std;

enum class CODE_COMMAND
{
    SEND = 0,
    CLOSE = 1,
    EXIT = 2,
    HELP = 3,
    CLIENTS = 4,
    BAD_COMMAND = 5,
};

const vector<string> CommandList {
        "send", "close", "exit", "clients", "help"
};

struct Command
{
    CODE_COMMAND code;
    set<string> arguments;
    string text;
};

struct LineError
{
private:
    string data_;
public:
    LineError() : data_(""){}
    LineError(const string& str) : data_(str) {}

    void setErrorMessage(const string& str){ data_ = str;}
    string message() {return data_;}
    operator bool(){ return !(data_ == ""); }
};

class InputLine {
private:
    string data_;
public:
    InputLine(const string &data_);

    shared_ptr<Command> getCommand(LineError& error);
};


#endif //TESTSERVERBOOST_INPUTLINE_H
