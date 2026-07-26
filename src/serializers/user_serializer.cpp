#include "user_serializer.hpp"

namespace anxiety_backend {

    userver::formats::json::Value SerilizeUser (const UserModel& user){
        userver::formats::json::ValueBuilder response;
        
        response["id"] = user.uuid;
        response["email"] = user.email;
        response["name"] = user.name;
        response["created_at"] = user.created_at;
        
        return response.ExtractValue();
    }
    userver::formats::json::Value SerilizeError (const char* error){
        userver::formats::json::ValueBuilder response;
        response["error"] = error;

        return response.ExtractValue();
    }
    userver::formats::json::Value SerilizeJwt (const std::string& token){
        userver::formats::json::ValueBuilder response;
        response["token"] = token;

        return response.ExtractValue();
    }


}