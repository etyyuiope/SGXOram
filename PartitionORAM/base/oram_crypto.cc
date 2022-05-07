/*
 Copyright (c) 2022 Haobin Chen

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "oram_crypto.h"

#include <spdlog/spdlog.h>

#include "oram_utils.h"

extern std::shared_ptr<spdlog::logger> logger;

namespace oram_crypto {
Cryptor::Cryptor() {
  if (sodium_init() == -1) {
    logger->error("Failed to initialize sodium.");
    abort();
  }
  // Initialize the random number.
  randombytes_buf(random_val_, sizeof(ORAM_CRYPTO_RANDOM_SIZE));
  is_initialized = true;
  logger->info("Cryptor initialized.");
}

Cryptor::~Cryptor() {
  // Do nothing.
}

std::shared_ptr<Cryptor> Cryptor::get_instance(void) {
  std::shared_ptr<Cryptor> instance = std::shared_ptr<Cryptor>(new Cryptor());
  return instance;
}

void Cryptor::crypto_prelogue(void) {
  PANIC_IF(!is_initialized, "Cryptor is not initialized.");

  if (crypto_aead_aes256gcm_is_available() == 0) {
    logger->error("AES-256-GCM is not available on this CPU.");
    abort();
  }
}

// The encryption algorithm also uses AES-GCM mode to encrypt the message.
partition_oram::Status Cryptor::encrypt(const uint8_t* message, size_t length,
                                        uint8_t* const iv,
                                        std::string* const out) {
  crypto_prelogue();
  PANIC_IF(!is_negotiated, "Cryptor is not negotiated.");

  // Use the key to encrypte the data.

  // Fill in the IV.
  out->resize(crypto_aead_aes256gcm_ABYTES + length);
  unsigned long long ciphertext_len;
  randombytes_buf(iv, ORAM_CRYPTO_RANDOM_SIZE);
  int ret = crypto_aead_aes256gcm_encrypt(
      (uint8_t*)out->data(), &ciphertext_len, message, length, nullptr, 0, NULL,
      iv, session_key_rx_);

  return ret == 0 ? partition_oram::Status::OK
                  : partition_oram::Status::INVALID_OPERATION;
}

partition_oram::Status Cryptor::decrypt(const uint8_t* message, size_t length,
                                        const uint8_t* iv,
                                        std::string* const out) {
  crypto_prelogue();
  PANIC_IF(!is_negotiated, "Cryptor is not negotiated.");

  if (length < crypto_aead_aes256gcm_ABYTES) {
    logger->error("The length of the message is too short.");
    return partition_oram::Status::INVALID_ARGUMENT;
  }

  // The message consists of the GCM MAC tag, the nonce, ant the
  // ciphertext itself, so it is easily for us to dertemine the length of the
  // plaintext because length of ciphertext = length of plaintext.
  size_t message_len = length - crypto_aead_aes256gcm_ABYTES;
  uint8_t* const decrypted = (uint8_t*)malloc(message_len);

  unsigned long long decrypted_len;
  int ret = crypto_aead_aes256gcm_decrypt(decrypted, (ull*)&message_len,
                                          nullptr, message, length, nullptr, 0,
                                          iv, session_key_tx_);
  *out = std::string((char*)decrypted, message_len);

  // Free the memory.
  oram_utils::safe_free(decrypted);
  return ret == 0 ? partition_oram::Status::OK
                  : partition_oram::Status::INVALID_OPERATION;
}

partition_oram::Status Cryptor::digest(const uint8_t* message, size_t length,
                                       std::string* const out) {
  uint8_t* const digest = (uint8_t*)malloc(crypto_hash_sha256_BYTES);
  uint8_t* const message_with_nonce =
      (uint8_t*)malloc(length + ORAM_CRYPTO_RANDOM_SIZE);
  // Concatenate the raw message with the random number.
  memcpy(message_with_nonce, message, length);
  memcpy(message_with_nonce + length, random_val_, ORAM_CRYPTO_RANDOM_SIZE);
  // Digest the message using SHA-256.
  int ret = crypto_hash_sha256(digest, message_with_nonce,
                               length + ORAM_CRYPTO_RANDOM_SIZE);
  *out = std::string((char*)digest, crypto_hash_sha256_BYTES);

  // Free the memory.
  oram_utils::safe_free_all(2, digest, message_with_nonce);
  return ret == 0 ? partition_oram::Status::OK
                  : partition_oram::Status::INVALID_OPERATION;
}

partition_oram::Status Cryptor::sample_key_pair(void) {
  crypto_prelogue();

  // Generate a key pair.
  int ret = crypto_kx_keypair(public_key_, secret_key_);
  return ret == 0 ? partition_oram::Status::OK
                  : partition_oram::Status::INVALID_OPERATION;
}

partition_oram::Status Cryptor::sample_session_key(const std::string& peer_pk,
                                                   bool type) {
  crypto_prelogue();

  // Check the length of the peer's public key.
  if (peer_pk.length() != crypto_kx_PUBLICKEYBYTES) {
    logger->error("The length of the peer's public key is not correct.");
    return partition_oram::Status::INVALID_ARGUMENT;
  }

  // Generate a session key. Prerequisite after this point: the peer's public
  // key must be known. Compute two shared keys using the peer's public key
  // and the cryptor's secret key. session_key_rx_ will be used by the client to
  // receive data from the server, session_key_tx will be used by the client to
  // send data to the server.
  int ret = -1;
  if (type == 0) {
    ret = crypto_kx_client_session_keys(session_key_rx_, session_key_tx_,
                                        public_key_, secret_key_,
                                        (uint8_t*)peer_pk.c_str());
  } else {
    ret = crypto_kx_server_session_keys(session_key_rx_, session_key_tx_,
                                        public_key_, secret_key_,
                                        (uint8_t*)peer_pk.c_str());
  }

  if (ret == partition_oram::Status::OK) {
    is_negotiated = true;
    return partition_oram::Status::OK;
  } else {
    return partition_oram::Status::INVALID_OPERATION;
  }
}

std::pair<std::string, std::string> Cryptor::get_key_pair(void) {
  PANIC_IF(!is_initialized, "Cryptor is not initialized.");

  std::pair<std::string, std::string> key_pair;
  key_pair.first = std::string((char*)public_key_, crypto_kx_PUBLICKEYBYTES);
  key_pair.second = std::string((char*)secret_key_, crypto_kx_SECRETKEYBYTES);
  return key_pair;
}

std::pair<std::string, std::string> Cryptor::get_session_key_pair(void) {
  std::pair<std::string, std::string> key_pair;
  key_pair.first =
      std::string((char*)session_key_rx_, crypto_kx_SESSIONKEYBYTES);
  key_pair.second =
      std::string((char*)session_key_tx_, crypto_kx_SESSIONKEYBYTES);
  return key_pair;
}

uint32_t Cryptor::uniform_random(uint32_t min, uint32_t max) {
  PANIC_IF((min > max), "The minimum value is greater than the maximum value.");

  // @ref Chromium's base/rand_util.cc for the implementation.
  uint32_t range = max - min + 1;
  uint32_t max_acceptable_value =
      (std::numeric_limits<uint32_t>::max() / range) * range - 1;
  // We sample a random number and them map it to the range [min, max]
  // (inclusive) in a uniform way by scaling.
  uint32_t value;

  do {
    // Use a strong RNG to generate a random number.
    // This is important because we want this function to be pseudorandom
    // and cannot be predicted by any adversary.
    value = randombytes_random();
  } while (value > max_acceptable_value);

  value = value % range + min;
  return value;
}
}  // namespace oram_crypto