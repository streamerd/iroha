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

#include <ametsuchi/block_serializer.hpp>

namespace iroha {
  namespace ametsuchi {

    using namespace rapidjson;

    /* Serialize */

    std::vector<uint8_t> BlockSerializer::serialize(model::Block block) {
      rapidjson::StringBuffer sb;
      rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
      serialize(writer, std::move(block));
      auto str = sb.GetString();
      std::vector<uint8_t> bytes{str, str + sb.GetLength()};
      return bytes;
    }

    void BlockSerializer::serialize(PrettyWriter<StringBuffer>& writer,
                                    model::Block block) {
      writer.StartObject();
      writer.String("hash");
      writer.String(block.hash.to_string().c_str());

      writer.String("signatures");
      writer.StartArray();
      for (auto sig : block.sigs) {
        serialize(writer, sig);
      }
      writer.EndArray();

      writer.String("created_ts");
      writer.Uint64(block.created_ts);

      writer.String("height");
      writer.Uint64(block.height);

      writer.String("prev_hash");
      writer.String(block.prev_hash.to_string().c_str());

      writer.String("txs_number");
      writer.Uint(block.txs_number);

      writer.String("merkle_root");
      writer.String(block.merkle_root.to_string().c_str());

      writer.String("transactions");
      writer.StartArray();
      for (auto tx : block.transactions) {
        serialize(writer, tx);
      }
      writer.EndArray();

      writer.EndObject();
    }

    void BlockSerializer::serialize(PrettyWriter<StringBuffer>& writer,
                                    model::Signature signature) {
      writer.StartObject();

      writer.String("pubkey");
      writer.String(signature.pubkey.to_string().c_str());

      writer.String("signature");
      writer.String(signature.signature.to_string().c_str());

      writer.EndObject();
    }

    void BlockSerializer::serialize(PrettyWriter<StringBuffer>& writer,
                                    model::Transaction transaction) {
      writer.StartObject();

      writer.String("signatures");
      writer.StartArray();
      for (auto sig : transaction.signatures) {
        serialize(writer, sig);
      }
      writer.EndArray();

      writer.String("created_ts");
      writer.Uint64(transaction.created_ts);

      writer.String("creator_account_id");
      writer.String(transaction.creator_account_id.c_str());

      writer.String("tx_counter");
      writer.Uint64(transaction.tx_counter);

      writer.String("commands");
      writer.StartArray();
      for (auto command : transaction.commands) {
        serialize(writer, *command);
      }
      writer.EndArray();

      writer.EndObject();
    }

    void BlockSerializer::serialize(PrettyWriter<StringBuffer>& writer,
                                    model::Command& command) {
      if (instanceof <model::AddPeer>(&command)) {
        auto add_peer = static_cast<model::AddPeer&>(command);
        serialize(writer, add_peer);
      }
      if (instanceof <model::AddAssetQuantity>(&command)) {
        auto add_asset_quantity =
            static_cast<model::AddAssetQuantity&>(command);
        serialize(writer, add_asset_quantity);
      }
      if (instanceof <model::AddSignatory>(&command)) {
        auto add_signatory = static_cast<model::AddSignatory&>(command);
        serialize(writer, add_signatory);
      }
      if (instanceof <model::AssignMasterKey>(&command)) {
        auto assign_master_key = static_cast<model::AssignMasterKey&>(command);
        serialize(writer, assign_master_key);
      }
      if (instanceof <model::CreateAccount>(&command)) {
        auto create_account = static_cast<model::CreateAccount&>(command);
        serialize(writer, create_account);
      }
      if (instanceof <model::CreateAsset>(&command)) {
        auto create_asset = static_cast<model::CreateAsset&>(command);
        serialize(writer, create_asset);
      }
      if (instanceof <model::CreateDomain>(&command)) {
        auto create_domain = static_cast<model::CreateDomain&>(command);
        serialize(writer, create_domain);
      }
      if (instanceof <model::RemoveSignatory>(&command)) {
        auto remove_signatory = static_cast<model::RemoveSignatory&>(command);
        serialize(writer, remove_signatory);
      }
      if (instanceof <model::SetAccountPermissions>(&command)) {
        auto set_account_permissions =
            static_cast<model::SetAccountPermissions&>(command);
        serialize(writer, set_account_permissions);
      }
      if (instanceof <model::SetQuorum>(&command)) {
        auto set_quorum = static_cast<model::SetQuorum&>(command);
        serialize(writer, set_quorum);
      }
      if (instanceof <model::TransferAsset>(&command)) {
        auto transfer_asset = static_cast<model::TransferAsset&>(command);
        serialize(writer, transfer_asset);
      }
    }

    void BlockSerializer::serialize(PrettyWriter<StringBuffer>& writer,
                                    model::AddPeer& add_peer) {
      writer.StartObject();

      writer.String("command_type");
      writer.String("AddPeer");

      writer.String("address");
      writer.String(add_peer.address.c_str());

      writer.String("peer_key");
      writer.String(add_peer.peer_key.to_string().c_str());

      writer.EndObject();
    }

    void BlockSerializer::serialize(
        PrettyWriter<StringBuffer>& writer,
        model::AddAssetQuantity& add_asset_quantity) {
      writer.StartObject();

      writer.String("command_type");
      writer.String("AddAssetQuantity");

      writer.String("account_id");
      writer.String(add_asset_quantity.account_id.c_str());

      writer.String("asset_id");
      writer.String(add_asset_quantity.asset_id.c_str());

      writer.String("amount");

      writer.Double(
          std::decimal::decimal64_to_double(add_asset_quantity.amount));

      writer.EndObject();
    }

    void BlockSerializer::serialize(PrettyWriter<StringBuffer>& writer,
                                    model::AddSignatory& add_signatory) {
      writer.StartObject();

      writer.String("command_type");
      writer.String("AddSignatory");

      writer.String("account_id");
      writer.String(add_signatory.account_id.c_str());

      writer.String("pubkey");
      writer.String(add_signatory.pubkey.to_string().c_str());

      writer.EndObject();
    }

    void BlockSerializer::serialize(PrettyWriter<StringBuffer>& writer,
                                    model::AssignMasterKey& assign_master_key) {
      writer.StartObject();

      writer.String("command_type");
      writer.String("AssignMasterKey");

      writer.String("account_id");
      writer.String(assign_master_key.account_id.c_str());

      writer.String("pubkey");
      writer.String(assign_master_key.pubkey.to_string().c_str());

      writer.EndObject();
    }

    void BlockSerializer::serialize(PrettyWriter<StringBuffer>& writer,
                                    model::CreateAccount& create_account) {
      writer.StartObject();

      writer.String("command_type");
      writer.String("CreateAccount");

      writer.String("domain_id");
      writer.String(create_account.domain_id.c_str());

      writer.String("account_name");
      writer.String(create_account.account_name.c_str());

      writer.String("pubkey");
      writer.String(create_account.pubkey.to_string().c_str());

      writer.EndObject();
    }

    void BlockSerializer::serialize(PrettyWriter<StringBuffer>& writer,
                                    model::CreateAsset& create_asset) {
      writer.StartObject();

      writer.String("command_type");
      writer.String("CreateAsset");

      writer.String("asset_name");
      writer.String(create_asset.asset_name.c_str());

      writer.String("domain_id");
      writer.String(create_asset.domain_id.c_str());

      writer.String("precision");
      writer.Uint(create_asset.precision);

      writer.EndObject();
    }

    void BlockSerializer::serialize(PrettyWriter<StringBuffer>& writer,
                                    model::CreateDomain& create_domain) {
      writer.StartObject();

      writer.String("command_type");
      writer.String("CreateDomain");

      writer.String("domain_name");
      writer.String(create_domain.domain_name.c_str());

      writer.EndObject();
    }

    void BlockSerializer::serialize(PrettyWriter<StringBuffer>& writer,
                                    model::RemoveSignatory& remove_signatory) {
      writer.StartObject();

      writer.String("command_type");
      writer.String("RemoveSignatory");

      writer.String("account_id");
      writer.String(remove_signatory.account_id.c_str());

      writer.String("pubkey");
      writer.String(remove_signatory.pubkey.to_string().c_str());

      writer.EndObject();
    }

    void BlockSerializer::serialize(
        PrettyWriter<StringBuffer>& writer,
        model::SetAccountPermissions& set_account_permissions) {
      writer.StartObject();

      writer.String("command_type");
      writer.String("SetAccountPermissions");

      writer.String("account_id");
      writer.String(set_account_permissions.account_id.c_str());

      writer.String("new_permissions");
      writer.StartObject();

      writer.String("add_signatory");
      writer.Bool(set_account_permissions.new_permissions.add_signatory);

      writer.String("can_transfer");
      writer.Bool(set_account_permissions.new_permissions.can_transfer);

      writer.String("create_accounts");
      writer.Bool(set_account_permissions.new_permissions.create_accounts);

      writer.String("create_assets");
      writer.Bool(set_account_permissions.new_permissions.create_assets);

      writer.String("create_domains");
      writer.Bool(set_account_permissions.new_permissions.create_domains);

      writer.String("issue_assets");
      writer.Bool(set_account_permissions.new_permissions.issue_assets);

      writer.String("read_all_accounts");
      writer.Bool(set_account_permissions.new_permissions.read_all_accounts);

      writer.String("remove_signatory");
      writer.Bool(set_account_permissions.new_permissions.remove_signatory);

      writer.String("set_permissions");
      writer.Bool(set_account_permissions.new_permissions.set_permissions);

      writer.String("set_quorum");
      writer.Bool(set_account_permissions.new_permissions.set_quorum);

      writer.EndObject();

      writer.EndObject();
    }

    void BlockSerializer::serialize(PrettyWriter<StringBuffer>& writer,
                                    model::SetQuorum& set_quorum) {
      writer.StartObject();

      writer.String("command_type");
      writer.String("SetQuorum");

      writer.String("account_id");
      writer.String(set_quorum.account_id.c_str());

      writer.String("new_quorum");
      writer.Uint(set_quorum.new_quorum);

      writer.EndObject();
    }

    void BlockSerializer::serialize(PrettyWriter<StringBuffer>& writer,
                                    model::TransferAsset& transfer_asset) {
      writer.StartObject();

      writer.String("dest_account_id");
      writer.String(transfer_asset.dest_account_id.c_str());

      writer.String("src_account_id");
      writer.String(transfer_asset.src_account_id.c_str());

      writer.String("asset_id");
      writer.String(transfer_asset.asset_id.c_str());

      writer.String("amount");
      writer.Double(std::decimal::decimal64_to_double(transfer_asset.amount));

      writer.EndObject();
    }

  /* Deserialize */

  model::Block BlockSerializer::deserialize(std::vector<uint8_t> bytes) {

  }

  }
}