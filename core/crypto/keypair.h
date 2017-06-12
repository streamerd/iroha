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

#include <array>
#include <cstdint>
#include <nonstd/any.hpp>
#include <nonstd/optional.hpp>
#include <string>
#include <vector>
#include "common.h"
#include "ed25519.h"


namespace iroha {

/**
 * Represents a keypair: public and private key.
 */
class Keypair {
 public:
  using signature_t = std::array<uint8_t, SIGNATURELEN>;
  using pubkey_t = std::array<uint8_t, PUBLEN>;
  using privkey_t = std::array<uint8_t, PRIVLEN>;

  /**
   * Build a keypair with public and private key in binary format
   * @param pub
   * @param priv
   */
  explicit Keypair(const pubkey_t &pub, const privkey_t &priv)
      : pubkey(std::move(pub)), privkey(std::move(priv)), has_private(true) {}

  /**
   * Build a keypair with only public key in binary format
   * Useful for signature verification.
   * @param pub
   */
  explicit Keypair(const pubkey_t &pub)
      : pubkey(std::move(pub)), has_private(false) {}


  /**
   * Sign the message
   * @param message - arbitrary blob
   * @return nonstd::nullopt if current keypair has no private key,
   * otherwise returns signature
   */
  nonstd::optional<signature_t> sign(const std::vector<uint8_t> &message) {
    // if keypair has no private key, it is not possible to sign
    if (!has_private) return nonstd::nullopt;

    signature_t sig;
    ed25519_sign(sig.data(), message.data(), message.size(), pubkey.data(),
                 privkey.data());
    return nonstd::optional<signature_t>(sig);
  }

  /**
   * Verify the signature against given message.
   * @param msg
   * @param sig
   * @return true if signature is ok, false otherwise
   */
  bool verify(const std::vector<uint8_t> &msg, const signature_t &sig) {
    return 1 ==
           ed25519_verify(sig.data(), msg.data(), msg.size(), pubkey.data());
  }

  /**
   * Getters for public and private keys in "digest" (binary) format
   * @return
   */
  pubkey_t pub_digest() { return pubkey; }
  nonstd::optional<privkey_t> priv_digest() {
    return has_private ? nonstd::optional<privkey_t>(privkey) : nonstd::nullopt;
  }

  /**
   * Getters for public and private keys in "hexdigest" (hex string) format
   * @return
   */
  std::string pub_hexdigest() {
    return digest_to_hexdigest(pubkey.data(), PUBLEN);
  }
  nonstd::optional<std::string> priv_hexdigest() {
    auto r = digest_to_hexdigest(privkey.data(), PRIVLEN);
    return has_private ? nonstd::optional<std::string>(r) : nonstd::nullopt;
  }

  /**
   * Getters for public and private keys in "base64" (string) format
   * @return
   */
  std::string pub_base64() { return base64_encode(pubkey.data(), PUBLEN); }
  nonstd::optional<std::string> priv_base64() {
    auto r = base64_encode(privkey.data(), PRIVLEN);
    return has_private ? nonstd::optional<std::string>(r) : nonstd::nullopt;
  }


 private:
  pubkey_t pubkey;
  privkey_t privkey;

  bool has_private;
};
}

#endif  // IROHA_KEYPAIR_H
