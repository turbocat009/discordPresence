#pragma once

#include <curl/curl.h>

#include <vector>
#include <string>

struct MemoryBuffer {
    std::vector<unsigned char> data;
};

std::string getJson(const std::string &url, const long discordID);
std::string getIMG(const std::string IMGURL);

std::string createUser(const std::string &url, const long discordID);
std::string modifyUser(const std::string &url, const long discordID, std::string &actName, std::string &actDesc, std::string &actType);
std::string setIMG(const std::string &url, const long discordID, const std::string &imgLoc);

std::string getActName();
std::string getActDesc();
std::string getActType();
std::string getID();
std::string getActIMG();
