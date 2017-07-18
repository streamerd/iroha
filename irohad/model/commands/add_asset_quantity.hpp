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

#ifndef IROHA_ADD_ASSET_QUANTITY_HPP
#define IROHA_ADD_ASSET_QUANTITY_HPP

#include <model/command.hpp>
#include <string>
#include "common/types.hpp"

namespace iroha {
  namespace model {

    /**
     * Add amount of asset to an account
     */
    struct AddAssetQuantity : public Command {
      /**
       * Account where to add assets
       */
      std::string account_id;

      /**
       * Asset to issue
       * Note: must exist in the system
       */
      std::string asset_id;

      /**
       * Amount to add to account asset
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
#endif  // IROHA_ADD_ASSET_QUANTITY_HPP
