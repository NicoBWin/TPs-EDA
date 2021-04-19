#ifndef _HTTP_REQUEST_
#define _HTTP_REQUEST_

#include <curl/curl.h>
#include <string>

class HTTP_Request
{
public:
    HTTP_Request(std::string protocol, std::string host, unsigned port = 80, std::string cache = "cache");
    ~HTTP_Request();

    bool getData(std::string path = "/");
    void setVerbose(void);

private:
    CURL* curl_h;

    std::string protocol; // HTTPS, GIT, FTP, etc
    std::string host;
    unsigned port;

    std::string cache;

    std::string receivedData;
    std::string serverFilePath;       // Content of "Location" in HTTP header

    size_t writeData(std::string requestPath);

    void getRidOfServerAddress(void);
    void getRidOfParameters(void);
};

#endif /* ! _HTTP_REQUEST_ */