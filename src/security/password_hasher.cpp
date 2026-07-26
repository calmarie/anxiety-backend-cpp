#include "password_hasher.hpp"

#include <openssl/rand.h>
#include <stdexcept>
#include <argon2.h>

namespace anxiety_backend {

std::string PasswordHasher::Hash(std::string_view password) const{

    std::array<uint8_t, SALTLEN> salt = GenerateSalt();

    const uint8_t *pwd = reinterpret_cast<const uint8_t*>(password.data());
    uint32_t pwdlen = password.size();

    
    uint32_t t_cost = 2;            // count of passes
    uint32_t m_cost = (1<<16);      // memory cost
    uint32_t parallelism = 1;       // number of threads and lanes
    std::string encoded;

    auto encoded_len = argon2_encodedlen(
    t_cost,
    m_cost,
    parallelism,
    SALTLEN,
    HASHLEN,
    Argon2_id
    );

    encoded.resize(encoded_len);

    argon2id_hash_encoded(
        t_cost,
        m_cost,parallelism,
        pwd,
        pwdlen,
        salt.data(),
        SALTLEN,
        HASHLEN, 
        encoded.data(),
        encoded_len
    );
    

    return encoded;

}

bool PasswordHasher::Verify(std::string_view password_hash, std::string_view password) const{

    bool flag = argon2_verify(password_hash.data(), password.data(), password.size(), Argon2_id);

    return flag;
}

std::array<uint8_t ,SALTLEN> PasswordHasher::GenerateSalt() const {
    std::array<uint8_t ,SALTLEN> buffer;
    if (RAND_bytes(buffer.data(), 16) != 1) {
        throw std::runtime_error("Generation of salt with OpenSSL");
    }
    return buffer;
}

}