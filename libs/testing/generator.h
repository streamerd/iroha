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

#ifndef IROHA_GENERATOR_H
#define IROHA_GENERATOR_H

#include <api.pb.h>
#include <crypto/common.h>
#include <crypto/hash.h>
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <functional>
#include <string>

namespace iroha {

/**
 * Current state of random generators.
 */
unsigned int SEED_ = 1337;

const char ALPHABET[] =
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

/**
 * returns a number in a range [min, max)
 */
int64_t random_number(int64_t min, int64_t max) {
  return min + (rand_r(&SEED_) % (max - min));
}

uint8_t random_printable_char() { return (uint8_t)random_number(32, 126 + 1); }


std::string random_string(size_t length, std::string alphabet = ALPHABET) {
  assert(alphabet.size() > 0);
  std::string s;
  std::generate_n(std::back_inserter(s), length, [&alphabet]() {
    size_t i = (size_t)random_number(0, alphabet.size());
    return (char)alphabet[i];
  });
  return s;
}

std::vector<uint8_t> random_blob(size_t length) {
  std::vector<uint8_t> v(length);
  std::generate_n(v.begin(), length, std::bind(random_number, 0, 256));
  return v;
}

iroha::api::Transaction random_transaction() {
  iroha::api::Transaction transaction;

  transaction.set_meta(random_string((size_t)random_number(10, 20)));

  transaction.set_body(random_string((size_t)random_number(100, 200)));

  auto header = transaction.mutable_header();
  // Set created timestamp
  header->set_created_ts((uint64_t)random_number(0, INT64_MAX));
  // Set signature
  auto signature = header->add_sigs();
  signature->set_pubkey(random_string(ed25519::PUBLEN));
  signature->set_signature(random_string(ed25519::SIGNATURELEN));
  // Set hash
  std::string hash(32, '\0');
  sha3_256((unsigned char *)&hash[0],
           (unsigned char *)(transaction.meta() + transaction.body()).c_str(),
           transaction.meta().size() + transaction.body().size());
  header->set_hash(hash);

  return transaction;
}
}

#endif  // IROHA_GENERATOR_H
