#include <iostream>
#include <thread>
#include <atomic>
#include <string>
#include <functional>
#include <csignal>

#include "DiscordFunc.hpp"

std::shared_ptr<discordpp::Client> DiscordFunc::client = nullptr;
std::string DiscordFunc::token = "nullptr";
bool DiscordFunc::isLogIngIn = false;

void DiscordFunc::initDiscordSDK() {
  std::cout << "Initializing Discord SDK...\n";

  //Create Discord Client
  client = std::make_shared<discordpp::Client>();

  // Set up logging callback
  client->AddLogCallback([](auto message, auto severity) { 
    std::cout << "[" << EnumToString(severity) << "] " << message << std::endl; 
  }, discordpp::LoggingSeverity::Info);

  // Set up status callback to monitor client connection
  client->SetStatusChangedCallback([](discordpp::Client::Status status, discordpp::Client::Error error, int32_t errorDetail) {
      std::cout << "Status changed: " << discordpp::Client::StatusToString(status) << std::endl;

      if (status == discordpp::Client::Status::Ready) {
        std::cout << "Client is ready! You can now call SDK functions.\n";
      } else if (error != discordpp::Client::Error::None) {
        std::cerr << "Connection Error: " << discordpp::Client::ErrorToString(error) << " - Details: " << errorDetail << std::endl;
      } 
    }
  );
}

void DiscordFunc::logIn(uint64_t APPLICATION_ID, discordpp::Client *clientRef) {
  if (!isLogIngIn) {
    // Set a flag to indicate Log In
    isLogIngIn = true;

    // Generate OAuth2 code verifier for authentication
    auto codeVerifier = clientRef->CreateAuthorizationCodeVerifier();
  
    // Set up authentication arguments
    discordpp::AuthorizationArgs args{};
    args.SetClientId(APPLICATION_ID);
    args.SetScopes(discordpp::Client::GetDefaultPresenceScopes());
    args.SetCodeChallenge(codeVerifier.Challenge());
  
    // Begin authentication process
    clientRef->Authorize(args, [clientRef, codeVerifier, APPLICATION_ID](discordpp::ClientResult result, std::string code, std::string redirectUri) {
    if (!result.Successful()) {
      std::cerr << "Authentication Error: " << result.Error() << std::endl;
      return;
    } else {
      std::cout << "Authorization successful! Getting access token...\n";
  
      // Exchange auth code for access token
      clientRef->GetToken(APPLICATION_ID, code, codeVerifier.Verifier(), redirectUri,
        [clientRef](discordpp::ClientResult result,
        std::string accessToken,
        std::string refreshToken,
        discordpp::AuthorizationTokenType tokenType,
        int32_t expiresIn,
        std::string scope) {
          std::cout << "Access token received! Establishing connection...\n";
          // Next Step: Update the token and connect
          clientRef->UpdateToken(discordpp::AuthorizationTokenType::Bearer, accessToken, [accessToken](discordpp::ClientResult result) {
            if(result.Successful()) {
              std::cout << "Token updated, connecting to Discord...\n";
              client->Connect();
              token = accessToken;
              return;
            } else {
              isLogIngIn = false;
              std::cout << "Failed to update token\n";
              return;
            }});
        });
        return;
    } });
    return;
  }
}

void DiscordFunc::logOut() {
  client->ClearRichPresence();
  client->Disconnect();
  client->UpdateToken(discordpp::AuthorizationTokenType::Bearer, "", [](discordpp::ClientResult result) {});
  isLogIngIn = false;
  token = "nullptr";
}

void DiscordFunc::setPresence(std::string &Name, std::string &Desc, std::string &Type, std::string &IMG) {
  discordpp::Activity activity;
  activity.SetType(discordpp::ActivityTypes::Playing);
  activity.SetName(Name);
  activity.SetDetails(Desc);
  activity.SetState(Type);

  discordpp::ActivityAssets assets;
  assets.SetLargeImage(IMG);
  activity.SetAssets(assets);

  client->UpdateRichPresence(activity, [](discordpp::ClientResult result) {});
}

std::shared_ptr<discordpp::Client> DiscordFunc::getClient() {
  return client;
}

std::string DiscordFunc::getToken() {
  return token;
}

std::string DiscordFunc::getCurrentUsername() {
  auto user = client->GetCurrentUser();
  return user.Username();
}

long long DiscordFunc::getCurrentID() {
  auto user = client->GetCurrentUser();
  return user.Id();
}