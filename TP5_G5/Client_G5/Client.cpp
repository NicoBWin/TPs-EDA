#include <iostream>
#include <filesystem>
#include <cstring>  /* For libparse */

#include "lib/libparse/libparse.hpp"
#include "HTTP_Request.h"
#include "Client.h"

/******************** DEFINITIONS ********************/
/******************** MACROS ********************/

/******************** STATIC PROTOTYPES ********************/
// This are needed to handle C-style callbacks (used in libcurl and libparse)

static int parseCmdCallback(char*, char*, void*);
static struct hostToQuery* hostFromVector(std::string host, std::vector<struct hostToQuery>& allHosts);
static bool pathExists(std::vector<std::string> host_paths, const char* pathToVerify);


/******************** CONSTRUCTOR ********************/
Client::Client(int argc, char* argv[])
{
    this->parsed = false;
    this->verbose = false;
    this->curl = CURLE_FAILED_INIT;

    this->userData.verbose = false;

    // Parce CMD
    if (parseCmdLine(argc, argv, parseCmdCallback, &userData) > 0) {
        this->parsed = true;
        this->verbose = userData.verbose;
    }
    else {
        printHelp();
        return;
    }

    std::string cachePath;
    cachePath.assign(CLIENT_PATH);
    cachePath += CACHE_PATH;

    std::filesystem::create_directories(cachePath);

    // Curl
    this->curl = curl_global_init(CURL_GLOBAL_ALL);
}

Client::~Client()
{
    curl_global_cleanup();
}

/******************** PUBLIC METHODS ********************/
int Client::run()
{
    if (this->curl != CURLE_OK || !this->parsed) {
        return -1;
    }

    std::string cachePath;
    cachePath.assign(CLIENT_PATH);
    cachePath += CACHE_PATH;
    
    for (struct hostToQuery queryMe : userData.hosts) {
        HTTP_Request myConnection(queryMe.protocol, queryMe.host, 80, cachePath);

        if (userData.verbose) {
            myConnection.setVerbose();
        }

        for (std::string path : queryMe.path) {
            std::cout << "Asking " << queryMe.host << " for file " << path << std::endl;
            if (myConnection.getData(path)) {
                std::cout << "Success!" << std::endl;
            }

        }
        std::cout << "Finished talking with " << queryMe.host << std::endl;
    }

    return 0;
}

void Client::printHelp(void)
{
    std::cout << "Usage:\n"
        << "Client_G5.exe [key] server[/path/to/file]\n"
        << "Client_G5.exe [key] server[/path/to/file] server[/path/to/file] ... \n"
        << "Client_G5.exe [key] server[/path/to/file] another_server[/path/to/file]\n\n"
        << "One or multiple servers can be recieved, and each one can request one or multiple files from the server.\n"
        << "When no file is specified, it's left to the server to handle it (in HTTP this is usually the same as /index.html)\n\n"
        << "Valid keys are:"
        << "-v | --verbose\t\tShow verbose output"
        << std::endl;

    return;
}

/******************** PRIVATE METHODS ********************/

/******************** STATIC FUNCTIONS ********************/
/*
 * libparse callback.
 */
static int parseCmdCallback(char* key, char* value, void* userData)
{
    
	if (userData == NULL) return PARAMS_INVALID;

	struct cmdParams* data = (struct cmdParams*) userData;

    if (key != NULL) {
        if (!strcmp(key, "v") || !strcmp(key, "-verbose")) {
            data->verbose = true;
        }
        else {
            return PARAMS_INVALID;
        }
    }

	// Parameter
    if (value != NULL) {
        /* Find the first "://". It should be after a protocol name (such as http, ftp, etc)
         * in any given URL. When it exists, we just skip everything before this substring.
         */
        char* doubleSlash = strstr(value, "://");
        std::string proto;

        /* Where does the path starts in the string */
        char* urlPath = NULL;

        /* Size of the host string. Used to ignore the path when converting to std::string */
        size_t hostSize = 0;


        if (doubleSlash != NULL) {
            proto.assign(value, doubleSlash);
            value = doubleSlash + 3;
        }

        urlPath = strchr(value, '/');

        if (urlPath != NULL) {
            while ((value + hostSize) != urlPath) {
                hostSize++;
            }
        }
        else {
            /* Host-only URL */
            hostSize = strlen(value);
        }

        std::string newHost;
        newHost.assign(value, hostSize);

        struct hostToQuery* host_to_path = hostFromVector(newHost, data->hosts);

        if (urlPath == NULL && !pathExists(host_to_path->path, "/")) {
            host_to_path->path.push_back(std::string("/"));
        }
        else if (urlPath != NULL && !pathExists(host_to_path->path, urlPath)) {
            host_to_path->path.push_back(std::string(urlPath));
        }

        // Curl cannot handle '&'
        std::string& newlyPath = host_to_path->path.back();
        for (size_t i = 0; i < newlyPath.length(); i++) {
            if (newlyPath[i] == '&') {
                newlyPath.insert(i, "\"");
                newlyPath.insert(i+2, "\"");
                i += 2;
            }
        }

        if (proto.length() > 0
            && (host_to_path->protocol.length() == 0 || host_to_path->protocol==""))
        {
            host_to_path->protocol.assign(proto);
        }
    }
	return PARAMS_VALID;
}

/*
 * Get _host_ from _allHosts_ vector. Add it if does not exists. 
 */
static struct hostToQuery* hostFromVector(std::string host, std::vector<struct hostToQuery>& allHosts)
{
    size_t index = 0;
    for (index = 0; index < allHosts.size(); index++) {
        if (allHosts[index].host == host) {
            break;
        }
    }

    if (index >= allHosts.size()) {
        struct hostToQuery newHost;
        newHost.host = host;
        allHosts.push_back(newHost);
        index = allHosts.size() - 1;
    }
    return &allHosts[index];
}

/*
 * Check if _pathToVerify_ has already been loaded into _host_paths_ list.
 */
static bool pathExists(std::vector<std::string> host_paths, const char* pathToVerify)
{
    if (pathToVerify == NULL) {
        return false;
    }

    bool exists = false;

    for (std::string currentPath : host_paths) {
        if ( !strcmp(currentPath.c_str(), pathToVerify) ) {
            exists = true;
            break;
        }
    }

    return exists;
}
