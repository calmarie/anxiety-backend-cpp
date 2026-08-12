#include "refresh_token_gen.hpp"
#include <openssl/rand.h>
#include <openssl/sha.h> 

#include <sstream>
#include <iomanip>

namespace anxiety_backend {

    std::string RefreshTokenGenerator::GenerateRefreshToken () const{
        std::array<uint8_t ,TOKENLEN> buffer;
        
        if (RAND_bytes(buffer.data(), TOKENLEN) != 1) {
            throw std::runtime_error("Generation of Refresh Token with OpenSSL");
        }

        std::stringstream ss;

        ss << std::hex << std::setfill('0');

        for (int i = 0; i < TOKENLEN; i++) {
            ss << std::hex << std::setw(2) << static_cast<int>(buffer[i]);
        }

        return ss.str();

        
    }
    std::string RefreshTokenGenerator::HashRefreshToken (std::string_view token) const{
        
        std::array<unsigned char, SHA256_DIGEST_LENGTH> hash;

        SHA256(reinterpret_cast<const unsigned char*>(token.data()), TOKENLEN, hash.data());

        std::stringstream ss;

        ss << std::hex << std::setfill('0');

        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
            ss << std::hex << std::setw(2) << static_cast<int>(hash[i]);
        }

        return ss.str();
    }
}