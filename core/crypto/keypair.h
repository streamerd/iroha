/**
 * Copyright Soramitsu Co., Ltd. 2017 All Rights Reserved.
 * http://soramitsu.co.jp
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef IROHA_KEYPAIR_H
#define IROHA_KEYPAIR_H

#include "ed25519.h"
#include <array>
#include <cstdint>
#include <string>
#include <vector>
#include <option.hpp>

namespace iroha {

enum ed25519 { SIGNATURELEN = 44, PUBLEN = 32, PRIVLEN = 64 };

static inline std::string digest_to_hexdigest(const uint8_t *digest,
                                              size_t size) {
  char code[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                 '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
  std::string res = "";
  uint8_t front, back;
  for (uint32_t i = 0; i < size; i++) {
    front = (uint8_t)(digest[i] & 0xF0) >> 4;
    back = (uint8_t)(digest[i] & 0xF);
    res += code[front];
    res += code[back];
  }
  return res;
}

/**
 * Represents a keypair: public and private key.
 */
class Keypair {
public:
  using signature_t = std::array<uint8_t, SIGNATURELEN>;
  using pubkey_t = std::array<uint8_t, PUBLEN>;
  using privkey_t = std::array<uint8_t, PRIVLEN>;

  explicit Keypair(const pubkey_t &pub, const privkey_t &priv)
      : pubkey(std::move(pub)), privkey(std::move(priv)), has_private(true) {}

  explicit Keypair(const pubkey_t &pub)
      : pubkey(std::move(pub)), has_private(false) {}

  signature_t sign(const std::vector<uint8_t> &message) {
    signature_t sig;
    ed25519_sign(sig.data(), message.data(), message.size(), pubkey.data(),
                 privkey.data());
    return sig;
  }

  bool verify(const std::vector<uint8_t> &msg, const signature_t &sig) {
    return 1 ==
           ed25519_verify(sig.data(), msg.data(), msg.size(), pubkey.data());
  }

  pubkey_t pub_digest() { return pubkey; }
  privkey_t priv_digest() { return privkey; }

  std::string pub_hexdigest() {
    return digest_to_hexdigest(pubkey.data(), PUBLEN);
  }

  std::string priv_hexdigest() {
    return has_private ? digest_to_hexdigest(privkey.data(), PRIVLEN) : "";
  }

  std::string pub_base64() { return base64_encode(pubkey.data(), PUBLEN); }
  std::string priv_base64() {
    return has_private ? base64_encode(privkey.data(), PRIVLEN) : "";
  }

private:
  pubkey_t pubkey;
  privkey_t privkey;

  bool has_private;
};
}

#endif // IROHA_KEYPAIR_H
