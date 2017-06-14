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

#ifndef IROHA_TX_GENERATOR_H
#define IROHA_TX_GENERATOR_H

#include <api.pb.h>
#include "generator.h"

namespace iroha {

iroha::api::Transaction random_transaction() {
  iroha::api::Transaction transaction;

  transaction.set_meta(random_string((size_t)random_number(30, 50)));

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

#endif  // IROHA_TX_GENERATOR_H
