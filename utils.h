#ifndef UTILS_H
#define UTILS_H

#include <cstdlib>
#include <unistd.h>
#include <stdio.h>
#include <sstream>

using namespace std;

#define CONNECT_PORT 5090

#define SERVER "localhost"

#define OUT 0 

#define CONNECT_CODE 1

#define RET_ERRNO(x) (x) == 0 ? 0: -errno

#define MAX_MSG_SIZE 1024

#define SERVER_ROOT "~/SharedFS/"



#endif
