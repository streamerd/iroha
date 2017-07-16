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

#include "model/converters/pb_block_factory.hpp"

namespace iroha {
  namespace model {
    namespace converters {

      protocol::Block PbBlockFactory::serialize(model::Block &block) {
        protocol::Block pb_block;

        // -----|Header|-----
        auto header = pb_block.mutable_header();
        header->set_created_time(block.created_ts);
        // todo set signatures

        // -----|Meta|-----
        auto meta = pb_block.mutable_meta();
        meta->set_tx_number(block.txs_number);
        meta->set_height(block.height);
        meta->set_merkle_root(block.merkle_root.data(),
                              block.merkle_root.size());
        meta->set_prev_block_hash(block.prev_hash.data(),
                                  block.prev_hash.size());

        // -----|Body|-----
        auto body = pb_block.mutable_body();
        // todo set transactions

        return pb_block;
      }

      model::Block PbBlockFactory::deserialize(protocol::Block &pb_block) {
        model::Block block;

        // -----|Header|-----
        block.created_ts = pb_block.header().created_time();
        // todo set signatures

        // -----|Meta|-----
        auto meta = pb_block.meta();
        // potential dangerous cast
        block.txs_number = (uint16_t) meta.tx_number();
        block.height = meta.height();
        std::copy(meta.merkle_root().begin(), meta.merkle_root().end(),
                  block.merkle_root.begin());
        std::copy(meta.prev_block_hash().begin(), meta.prev_block_hash().end(),
                  block.prev_hash.begin());

        // -----|Body|-----
        auto body = pb_block.body();
        // todo set transactions

        return block;
      }
    } // namespace converters
  }  // namespace model
}  // namespace iroha