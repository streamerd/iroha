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

#ifndef IROHA_POSTGRES_WSV_COMMAND_HPP
#define IROHA_POSTGRES_WSV_COMMAND_HPP

#include "ametsuchi/wsv_command.hpp"
#include <pqxx/nontransaction>

namespace iroha {
  namespace ametsuchi {
    class PostgresWsvCommand : public WsvCommand {
     public:
      PostgresWsvCommand(std::unique_ptr<pqxx::nontransaction> &transaction);
      bool upsertAccount(const model::Account &account) override;
      bool insertAsset(const model::Asset &asset) override;
      bool upsertAccountAsset(const model::AccountAsset &asset) override;
      bool insertSignatory(const ed25519::pubkey_t &signatory) override;
      bool insertAccountSignatory(const std::string &account_id,
                                  const ed25519::pubkey_t &signatory) override;
      bool deleteAccountSignatory(const std::string &account_id,
                                  const ed25519::pubkey_t &signatory) override;
      bool upsertPeer(const model::Peer &peer) override;
      bool deletePeer(const model::Peer &peer) override;
      bool insertDomain(const model::Domain &domain) override;

     private:
      std::unique_ptr<pqxx::nontransaction> &transaction_;
    };
  } // namespace ametsuchi
} // namespace iroha

#endif //IROHA_POSTGRES_WSV_COMMAND_HPP