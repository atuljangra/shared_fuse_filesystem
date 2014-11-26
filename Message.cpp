#include <sstream>

#include "Message.h"

using namespace std;

const char *Message::serialize() {
    stringstream ss; 
    ss << _code << " " << _ret << " " << _msg;

    return ss.str().c_str();    
}


void Message::create_getAttr(const char *path) {
    stringstream ss;
    ss << path;
    _msg = ss.str();
    cout << _msg << endl;
    _code = GETATTR;
}


