//
// Created by igor on 29.07.16.
//

#include "InputLine.h"

InputLine::InputLine(const string &data_) : data_(data_) {}

shared_ptr<Command> InputLine::getCommand(LineError& error) {
    return shared_ptr<Command>();
}
