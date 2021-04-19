#include <iostream>
#include <filesystem>
#include <fstream>
#include <cstring>

#include "HTTP_Request.h"

/******************** STATIC PROTOTYPES ********************/
// This are C-Style callbacks for curl.
static size_t getDataFromServer(char* ptr, size_t size, size_t nmemb, void* userData);
static size_t parseHeader(char* buffer, size_t size, size_t nitems, void* userdata);

/******************** CONSTRUCTOR ********************/
HTTP_Request::HTTP_Request(std::string protocol_, std::string host_, unsigned port_, std::string cache_)
{
    this->protocol = "";
    this->host = "";
    this->port = 0;
    this->cache = "";
    this->curl_h = NULL;
    
    if (host_.length() == 0) {
        return;
    }
    else if (port_ == 0) {
        return;
    }

    this->protocol = protocol_;
    this->host = host_;
    this->port = port_;
    this->cache = cache_;
    if (this->cache.back() != '/') {
        this->cache += "/";
    }

    // Init curl with opts
    curl_h = curl_easy_init();
    if (curl_h) {
        curl_easy_setopt(curl_h, CURLOPT_PORT, port);
        curl_easy_setopt(curl_h, CURLOPT_PROTOCOLS, CURLPROTO_HTTP);
        curl_easy_setopt(curl_h, CURLOPT_WRITEFUNCTION, getDataFromServer);
        curl_easy_setopt(curl_h, CURLOPT_WRITEDATA, &receivedData);
        curl_easy_setopt(curl_h, CURLOPT_HEADERFUNCTION, parseHeader);
        curl_easy_setopt(curl_h, CURLOPT_HEADERDATA, &serverFilePath);
    }

    return;
}

HTTP_Request::~HTTP_Request()
{
    if (curl_h != NULL) curl_easy_cleanup(curl_h);
    return;
}

/******************** PUBLIC METHODS ********************/
bool HTTP_Request::getData(std::string path)
{
    if (curl_h == NULL) {
        return false;
    }

    bool success = false;
    if (path == "") {
        path = "/";
    }
    // Clear old data from string
    this->receivedData.clear(); 
    this->serverFilePath.clear();
    
    std::string fullPath;

    fullPath += this->host + path;
    curl_easy_setopt(curl_h, CURLOPT_URL, fullPath.c_str());

    CURLcode error = curl_easy_perform(curl_h);

    if (error == CURLE_OK) {
        // Check against 404 error code
        long response_code = 0;
        curl_easy_getinfo(curl_h, CURLINFO_RESPONSE_CODE, &response_code);
        if (response_code == 200) {
            // Server can answer with its own address as part of the file location
            // remove it
            getRidOfServerAddress();

            // Some websites do not set "Location" as part of their HTTP header.
            if (!this->serverFilePath.compare("") || this->serverFilePath.length() == 0) {
                if (path == "" || path == "/") {
                    this->serverFilePath.assign("/");
                }
                else {
                    this->serverFilePath.assign(path);
                }
            }

            getRidOfParameters();

            writeData(this->serverFilePath);
            success = true;
        }
        else {
            std::cout << "Server returned error code " << response_code << std::endl;
        }
    }
    else if (error == CURLE_COULDNT_RESOLVE_HOST) {
        std::cout << "Unable to resolve host." << std::endl;
    }
    else if (error == CURLE_COULDNT_CONNECT) {
        std::cout << "Unable to connect." << std::endl;
    }
    else {
        std::cout << "An error ocurred while trying to connect." << std::endl;
    }

    return success;
}

void HTTP_Request::setVerbose(void)
{
        curl_easy_setopt(curl_h, CURLOPT_VERBOSE, 1L);
        return;
}

/******************** PRIVATE METHODS ********************/
size_t HTTP_Request::writeData(std::string requestPath)
{
    size_t fileNameSeparator = requestPath.find_last_of('/');

    std::string fullPath = cache + host + "/" + requestPath.substr(0, fileNameSeparator) + "/";
    std::string fileName = requestPath.substr(fileNameSeparator + 1);

    if (fileName.length() == 0 || fileName == "" || fileName == "/") {
        fileName.assign("index.html");
    }

    std::filesystem::create_directories(fullPath);
    std::ofstream file(fullPath + fileName, std::ios::out | std::ios::binary);

    if (file.is_open()) {
        file << this->receivedData;
        file.close();
    } else {
        std::cout << "Unable to open file " << fullPath << fileName << std::endl;
    }

    return 0;
}


void HTTP_Request::getRidOfServerAddress(void)
{
    if (this->serverFilePath.length() == 0 || this->serverFilePath == "") {
        return;
    }

    std::size_t hostInPath = this->serverFilePath.find(this->host);
    if (hostInPath != std::string::npos) {
        this->serverFilePath = this->serverFilePath.substr(hostInPath + this->host.size());
    }

    // Get rid of \r\n in the string path
    while (this->serverFilePath.back() == '\n' || this->serverFilePath.back() == '\r') {
        this->serverFilePath.pop_back();
    }

    return;
}

void HTTP_Request::getRidOfParameters(void)
{
    for (char& c : serverFilePath) {
        switch (c)
        {
        case '?':
        case '&':
        case '=':
        case '"':
            c = '_';
            break;
        default:
            break;
        }
    }

    return;
}

/******************** STATIC FUNCTIONS ********************/
/*
 * Store recieved data from server in receivedData of instance.
 */
static size_t getDataFromServer(char *data, size_t size, size_t nmemb, void *userData)
{
    if (userData == NULL || data == NULL) {
        return 0;
    }

    size_t realsize = size * nmemb;

    std::string * rData = (std::string*)userData;
    rData->append(data, realsize);

    return realsize;
}

/*
 * Get file location as provided by the HTTP header and store it in serverFilePath of this instance.
 */
static size_t parseHeader(char* buffer, size_t size, size_t nitems, void* userData)
{
    if (userData == NULL || buffer == NULL) {
        return 0;
    }

    size_t realsize = size * nitems;

    std::string* rData = (std::string*)userData;
    
    // "Location: " (avoid L to ignore first-letter in caps scenario)
    char* locationHeader = strstr(buffer, "ocation:"); 

    if (locationHeader != NULL) {
        std::string bufferAsString;
        bufferAsString.assign(buffer, realsize);

        // Get and store from the ": " to the end
        std::size_t colonPos = bufferAsString.find(":");
        colonPos += 2;

        rData->append(bufferAsString.substr(colonPos));
    }

    return realsize;
}
