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
  tx.creator_account_id = "123";

  tx.tx_counter = 0;
  tx.created_ts = 0;

  tx.signatures.push_back(create_signature());
  tx.signatures.push_back(create_signature());

  //  tx.commands

  // Add peer
  iroha::model::AddPeer add_peer;
  add_peer.address = "localhost";
  std::fill(add_peer.peer_key.begin(), add_peer.peer_key.end(), 0x123);
  tx.commands.push_back(std::make_shared<iroha::model::AddPeer>(add_peer));

  // AddAssetQuantity
  iroha::model::AddAssetQuantity add_asset_qty;
  add_asset_qty.account_id = "123";
  add_asset_qty.asset_id = "123";
  add_asset_qty.amount = std::decimal::make_decimal64(1010LL, -2);
  tx.commands.push_back(std::make_shared<iroha::model::AddAssetQuantity>(add_asset_qty));

  // AddSignatory
  iroha::model::AddSignatory add_signatory;
  add_signatory.account_id = "123";
  std::fill(add_signatory.pubkey.begin(), add_signatory.pubkey.end(), 0x123);
  tx.commands.push_back(std::make_shared<iroha::model::AddSignatory>(add_signatory));

  //AssignMasterKey
  iroha::model::AssignMasterKey assign_master_key;
  std::fill(assign_master_key.pubkey.begin(), assign_master_key.pubkey.end(), 0x123);
  assign_master_key.account_id = "123";
  tx.commands.push_back(std::make_shared<iroha::model::AssignMasterKey>(assign_master_key));

  //CreateAccount
  iroha::model::CreateAccount create_account;
  std::fill(create_account.pubkey.begin(), create_account.pubkey.end(), 0x123);
  create_account.account_name = "123";
  create_account.domain_id = "123";
  tx.commands.push_back(std::make_shared<iroha::model::CreateAccount>(create_account));

  //CreateAsset
  iroha::model::CreateAsset create_asset;
  create_asset.domain_id = "123";
  create_asset.asset_name = "123";
  create_asset.precision = 2;
  tx.commands.push_back(std::make_shared<iroha::model::CreateAsset>(create_asset));

  //CreateDomain
  iroha::model::CreateDomain create_domain;
  create_domain.domain_name = "123";
  tx.commands.push_back(std::make_shared<iroha::model::CreateDomain>(create_domain));

  //RemoveSignatory
  iroha::model::RemoveSignatory remove_signatory;
  remove_signatory.account_id = "123";
  std::fill(remove_signatory.pubkey.begin(), remove_signatory.pubkey.end(), 0x123);
  tx.commands.push_back(std::make_shared<iroha::model::RemoveSignatory>(remove_signatory));

  //SetPermissions
  iroha::model::SetAccountPermissions set_account_permissions;
  set_account_permissions.account_id = "123";
  set_account_permissions.new_permissions.can_transfer = true;
  tx.commands.push_back(std::make_shared<iroha::model::SetAccountPermissions>(set_account_permissions));

  //SetQuorum
  iroha::model::SetQuorum set_quorum;
  set_quorum.account_id = "123";
  set_quorum.new_quorum = 123;
  tx.commands.push_back(std::make_shared<iroha::model::SetQuorum>(set_quorum));

  //TransferAsset
  iroha::model::TransferAsset transfer_asset;
  transfer_asset.src_account_id = "123";
  transfer_asset.dest_account_id = "321";
  transfer_asset.amount = std::decimal::make_decimal64(1010ll, -2);
  transfer_asset.asset_id = "123";
  tx.commands.push_back(std::make_shared<iroha::model::TransferAsset>(transfer_asset));
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
  memset(block.hash.data(), 0x1, iroha::ed25519::pubkey_t::size());
  block.sigs.push_back(create_signature());
  block.created_ts = 0;
  block.height = 0;
  memset(block.prev_hash.data(), 0x5, iroha::ed25519::pubkey_t::size());
  block.txs_number = 0;
  memset(block.merkle_root.data(), 0x123, iroha::ed25519::pubkey_t::size());
  block.transactions.push_back(create_transaction());
  return block;
}


TEST(block_serialize, block_serialize_test){

  auto block = create_block();

  iroha::ametsuchi::BlockSerializer blockSerializer;

  std::cout << unsigned(block.hash[0]) << std::endl;

  auto bytes = blockSerializer.serialize(block);
  std::string str(bytes.begin(), bytes.end());
  std::cout << str << std::endl;
  // deserialize

  auto res = blockSerializer.deserialize(bytes);
  if (res){
    auto deserialized = res.value();
    ASSERT_EQ(block.hash, deserialized.hash);
    ASSERT_EQ(block.created_ts, deserialized.created_ts);

    ASSERT_TRUE(block.sigs.size() > 0);
    for (int i = 0; i < block.sigs.size(); i++){
      ASSERT_EQ(block.sigs[i].signature, deserialized.sigs[i].signature);
      ASSERT_EQ(block.sigs[i].pubkey, deserialized.sigs[i].pubkey);
    }

    ASSERT_EQ(block.prev_hash, deserialized.prev_hash);
    ASSERT_EQ(block.created_ts, deserialized.created_ts);
    ASSERT_EQ(block.merkle_root, deserialized.merkle_root);

    ASSERT_TRUE(block.transactions.size() > 0);
    for (int i = 0; i < block.transactions.size(); i++){
      auto tx = block.transactions[i];
      auto des_tx = deserialized.transactions[i]; // deserialized tx

      ASSERT_TRUE(tx.signatures.size() > 0);
      for (int j = 0; j < tx.signatures.size(); j++){
        ASSERT_EQ(tx.signatures[j].pubkey, des_tx.signatures[j].pubkey);
        ASSERT_EQ(tx.signatures[j].signature, des_tx.signatures[j].signature);
      }

      ASSERT_EQ(tx.created_ts, des_tx.created_ts);
      ASSERT_EQ(tx.creator_account_id, des_tx.creator_account_id);
      ASSERT_EQ(tx.tx_counter, des_tx.tx_counter);

      for (int j = 0; j < tx.commands.size(); j++){

      }

    }

  }
}
