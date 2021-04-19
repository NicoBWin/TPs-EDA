#ifndef _CONNECT_H_
#define _CONNECT_H_

#include <vector>
#include <string>
#include <curl/curl.h>

#define CLIENT_PATH     "clientData"
#define CACHE_PATH      "/cache"

/* List of hosts to query and all paths on each host. */
struct hostToQuery
{
    std::string protocol;
    std::string host;
    std::vector<std::string> path;
};

struct cmdParams
{
    bool verbose;
    std::vector<struct hostToQuery> hosts;
};

class Client
{
public:
    Client(int argc, char* argv[]);
    ~Client(void);

    int run(void);
    void printHelp(void);

private:
    bool parsed;
    bool verbose;
    CURLcode curl;

    struct cmdParams userData;
};

#endif /* ! _CONNECT_H_ */