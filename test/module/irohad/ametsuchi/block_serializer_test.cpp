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

#include <gtest/gtest.h>
#include <ametsuchi/block_serializer.hpp>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
#include <model/commands/add_peer.hpp>

iroha::model::Signature create_signature();
iroha::model::Transaction create_transaction();
iroha::model::Proposal create_proposal();
iroha::model::Block create_block();

iroha::model::Signature create_signature() {
  iroha::model::Signature signature{};
  memset(signature.signature.data(), 0x123, iroha::ed25519::sig_t::size());
  memset(signature.pubkey.data(), 0x123, iroha::ed25519::pubkey_t::size());
  return signature;
}

iroha::model::Transaction create_transaction() {
  iroha::model::Transaction tx{};
  memset(tx.creator.data(), 0x123, 32);

  tx.tx_counter = 0;
  tx.created_ts = 0;

  tx.signatures.push_back(create_signature());
  tx.signatures.push_back(create_signature());

  //  tx.commands
  iroha::model::AddPeer add_peer;
  add_peer.address = "Innopolis";
  add_peer.account_id = "123";

  tx.commands.push_back(std::make_shared<iroha::model::AddPeer>(add_peer));

  return tx;
}

iroha::model::Proposal create_proposal() {
  std::vector<iroha::model::Transaction> txs;
  txs.push_back(create_transaction());
  txs.push_back(create_transaction());

  iroha::model::Proposal proposal(txs);
  return proposal;
}

iroha::model::Block create_block() {
  iroha::model::Block block{};
  memset(block.hash.data(), 0x123, iroha::ed25519::pubkey_t::size());
  block.sigs.push_back(create_signature());
  block.created_ts = 0;
  block.height = 0;
  memset(block.prev_hash.data(), 0x123, iroha::ed25519::pubkey_t::size());
  block.txs_number = 0;
  memset(block.merkle_root.data(), 0x123, iroha::ed25519::pubkey_t::size());
  block.transactions.push_back(create_transaction());
  return block;
}


TEST(block_serialize, block_serialize_test){

  auto block = create_block();

  iroha::ametsuchi::BlockSerializer blockSerializer;

  rapidjson::StringBuffer sb;
  rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
  blockSerializer.serialize(writer, block);

  std::cout << sb.GetString();
}
