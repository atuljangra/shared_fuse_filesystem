#ifndef UTILS_H
#define UTILS_H

#define CONNECT_PORT 5090

#define SERVER "localhost"

#define CONNECT_CODE 1

#define RET_ERRNO(x) (x) == 0 ? 0: -errno

#define log(...) \
                do { if (!OUT) fprintf(stdout, ##__VA_ARGS__); \
                    else fprintf(f, ##__VA_ARGS__); } while (0)

#endif
