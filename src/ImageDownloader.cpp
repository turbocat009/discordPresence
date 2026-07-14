#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <ctime>

#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <ImageDownloader.hpp>
#include "DiscordFunc.hpp"
#include <stdexcept>
#include "ErrorNotifier.hpp"

std::string activity_Name;
std::string activity_Description;
std::string activity_Type;
std::string Id;
std::string IMG;
std::string IMGURL;

static size_t writeStringCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realSize = size * nmemb;
    auto *response = static_cast<std::string *>(userp);
    response->append(static_cast<char *>(contents), realSize);
    return realSize;
}

std::string getJson(const std::string &url, const long long discordID) {
    CURL *curl = curl_easy_init();
    if (!curl)
        throw std::runtime_error("curl_easy_init failed");

    std::string connectionURL = url + "users?discordID=" + std::to_string(discordID);

    std::string response;

    curl_easy_setopt(curl, CURLOPT_URL, connectionURL.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeStringCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 0);
    curl_easy_setopt(curl, CURLOPT_CAINFO, "certs/cacert.pem");

    struct curl_slist *headers = nullptr;
    headers = curl_slist_append(headers, "Accept: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    CURLcode res = curl_easy_perform(curl);

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK)
        throw std::runtime_error(curl_easy_strerror(res));

    if (response == "null") {
        response = createUser(url, discordID);
    }


    nlohmann::json responseJSON = nlohmann::json::parse(response);

    activity_Name = responseJSON["activity_name"];
    activity_Description = responseJSON["activity_desc"];
    activity_Type = responseJSON["activity_type"];
    IMG = responseJSON["activity_img"];
    Id = std::to_string(responseJSON["id"].get<long long>());


    return response;
}

std::string createUser(const std::string &url, const long long discordID) {
    CURL *curl = curl_easy_init();
    if (!curl)
        throw std::runtime_error("curl_easy_init failed");

    std::string connectionURL = url + "addUser";

    nlohmann::json newUser;
    newUser["id"] = std::to_string(discordID);
    newUser["name"] = DiscordFunc::getCurrentUsername();
    newUser["actname"] = "Name";
    newUser["actdesc"] = "Desc";
    newUser["acttype"] = "Type";
    newUser["actimg"] = setIMG(url, discordID, getIMG("https://raw.githubusercontent.com/turbocat009/discordPresence/refs/heads/main/res/gfx/logooo.png"));

    struct curl_slist *headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    std::string body = newUser.dump(4);
    std::string response;


    curl_easy_setopt(curl, CURLOPT_URL, connectionURL.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, body.size());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeStringCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_CAINFO, "certs/cacert.pem");

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        std::string err = std::string("curl error: ") + curl_easy_strerror(res);
        std::cerr << err << "\n";
        ErrorNotifier::notify(err, "Network Error");
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return "Curl Failed";
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return response;
}

std::string modifyUser(const std::string &url, const long long discordID, std::string &actName, std::string &actDesc, std::string &actType)
{
    CURL *curl = curl_easy_init();
    if (!curl)
        throw std::runtime_error("curl_easy_init failed");

    std::string connectionURL = url + "modifyUser";

    nlohmann::json user;
    user["id"] = std::to_string(discordID);
    user["actname"] = actName;
    user["actdesc"] = actDesc;
    user["acttype"] = actType;
    user["actimg"] = IMG;

    struct curl_slist *headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    std::string body = user.dump(4);

    curl_easy_setopt(curl, CURLOPT_URL, connectionURL.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, body.size());
    curl_easy_setopt(curl, CURLOPT_CAINFO, "certs/cacert.pem");

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        std::string err = std::string("curl error: ") + curl_easy_strerror(res);
        std::cerr << err << "\n";
        ErrorNotifier::notify(err, "Network Error");
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return "Curl Failed";
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return "0";
}

std::string getIMG(const std::string IMGURL) {
    CURL *curl = curl_easy_init();
    if (!curl)
        throw std::runtime_error("curl_easy_init failed");

    std::string connectionURL = IMGURL + "?t=" + std::to_string(std::time(nullptr));

    std::filesystem::path temp = std::filesystem::temp_directory_path();
    std::string filename = temp.string() + "image.png";

    FILE *fp = fopen(filename.c_str(), "wb");
    curl_easy_setopt(curl, CURLOPT_URL, connectionURL.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 0);
    curl_easy_setopt(curl, CURLOPT_CAINFO, "certs/cacert.pem");

    CURLcode res = curl_easy_perform(curl);

    curl_easy_cleanup(curl);
    fclose(fp);

    if (res != CURLE_OK)
        throw std::runtime_error(curl_easy_strerror(res));

    return filename;
}

std::string setIMG(const std::string &url, const long long discordID, const std::string &imgLoc) {
    CURL *curl = curl_easy_init();
    if (!curl)
        throw std::runtime_error("curl_easy_init failed");

    std::string connectionURL = url + "addImg";
    std::string response;
    std::string imgURL;

    curl_mime *form = curl_mime_init(curl);
    curl_mimepart *field = nullptr;

    field = curl_mime_addpart(form);
    curl_mime_name(field, "file");
    curl_mime_filedata(field, imgLoc.c_str());

    field = curl_mime_addpart(form);
    curl_mime_name(field, "discordID");
    std::string idStr = std::to_string(discordID);
    curl_mime_data(field, idStr.c_str(), CURL_ZERO_TERMINATED);

    curl_easy_setopt(curl, CURLOPT_URL, connectionURL.c_str());
    curl_easy_setopt(curl, CURLOPT_MIMEPOST, form);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeStringCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_CAINFO, "certs/cacert.pem");

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        std::string err = std::string("curl error: ") + curl_easy_strerror(res);
        std::cerr << err << "\n";
        ErrorNotifier::notify(err, "Network Error");
        curl_mime_free(form);
        curl_easy_cleanup(curl);
        return "Curl Failed";
    }

    try {
        auto responseJSON = nlohmann::json::parse(response);
        imgURL = responseJSON["url"];
    }
    catch (...) {
        curl_mime_free(form);
        curl_easy_cleanup(curl);
        return "JSON Parse Failed";
    }

    curl_mime_free(form);
    curl_easy_cleanup(curl);

    IMG = imgURL;

    std::cout << imgURL << std::endl;

    return imgURL;
}

std::string getActName() {
    return activity_Name;
}

std::string getActDesc() {
    return activity_Description;
}

std::string getActType() {
    return activity_Type;
}

std::string getID() {
    return Id;
}

std::string getActIMG() {
    return IMG + "?t=" + std::to_string(std::time(nullptr));
}
