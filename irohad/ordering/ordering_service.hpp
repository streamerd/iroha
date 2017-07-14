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

#ifndef IROHA_ORDERING_SERVICE_HPP
#define IROHA_ORDERING_SERVICE_HPP

#include <queue>

#include <block.pb.h>

namespace ordering_service {

  using Transaction = iroha::protocol::Transaction;
  using Block = iroha::protocol::Block;

  struct TransactionComparator
      : public std::binary_function<Transaction, Transaction, bool> {
    bool operator()(const Transaction& lhs, const Transaction& rhs) const {
      return lhs.header().created_time() < rhs.header().created_time();
    }
  };

  /**
   * append to ordering queue
   */
  void append(Transaction&);

  /**
   * on_commit, start ordering
   */
  void on_commit(Block&);

  namespace detail {

    void initialize(const Block&);
    Block makeBlock();

    static std::priority_queue<Transaction,std::vector<Transaction>,TransactionComparator> que_;
  }
}  // namespace ordering

#endif  // IROHA_ORDERING_SERVICE_HPP
