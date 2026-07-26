#pragma once
#include <array>
#include <string>
#include <cstdint>

#define HASHLEN 32
#define SALTLEN 16

namespace anxiety_backend{

class PasswordHasher{

public:
    std::string Hash(std::string_view password) const;

    bool Verify(std::string_view password_hash, std::string_view password) const;


private:
    std::array<uint8_t ,SALTLEN> GenerateSalt() const;
};
}
