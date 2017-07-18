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

#ifndef IROHA_TRANSFER_ASSET_HPP
#define IROHA_TRANSFER_ASSET_HPP

#include <model/command.hpp>
#include <string>
#include "common/types.hpp"

namespace iroha {
  namespace model {

    /**
     * Transfer asset from one account to another
     */
    struct TransferAsset : public Command {
      /**
       * Source account
       */
      std::string src_account_id;

      /**
       * Destination account
       */
      std::string dest_account_id;

      /**
       * Asset to transfer. Identifier is asset_id
       */
      std::string asset_id;
      /**
       * Amount of transferred asset
       */
      Amount amount;

      bool validate(ametsuchi::WsvQuery& queries,
                    const Account& creator) override;
      bool execute(ametsuchi::WsvQuery& queries,
                   ametsuchi::WsvCommand& commands) override;

      bool operator==(const Command& command) const override;
      bool operator!=(const Command& command) const override;
    };
  }  // namespace model
}  // namespace iroha
#endif  // IROHA_TRANSFER_ASSET_HPP
