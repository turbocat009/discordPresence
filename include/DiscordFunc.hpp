#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "discordpp.h"
#include "Math.hpp"

class DiscordFunc {
    public:
        void initDiscordSDK();
        static void logIn(const uint64_t APPLICATION_ID, discordpp::Client *clientRef);
        static void logOut();
        static void setPresence(std::string &Name, std::string &Desc, std::string &Type, std::string &IMG);
        static std::shared_ptr<discordpp::Client> getClient();
        static std::string getToken();
        static bool isLogIngIn;
        static std::string getCurrentUsername();
        static long getCurrentID();
    private:
        static std::shared_ptr<discordpp::Client> client;
        static std::string token;
    protected:
};