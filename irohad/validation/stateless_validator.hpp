/*
Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#ifndef IROHA_VALIDATION_STATELESS_VALIDATOR_HPP
#define IROHA_VALIDATION_STATELESS_VALIDATOR_HPP

#include <model/transaction.hpp>
#include <model/model_crypto_provider.hpp>

namespace iroha {
  namespace validation {
    using model::Transaction;
    using model::ModelCryptoProvider;

    class StatelessValidator {
      public:
          explicit StatelessValidator(
            model::ModelCryptoProvider& crypto_provider);

          /**
           * Perform stateless validation on a given transaction
           * @param transaction transaction to be validated
           * @return true if given transaction is valid, false otherwise
           */
          virtual bool validate(const model::Transaction& transaction) const;

      private:
          static constexpr uint64_t MAX_DELAY =
            1000 * 3600 * 24;  // max-delay between tx creation and validation
          const model::ModelCryptoProvider& crypto_provider_;
    };

  }  // namespace validation
}  // namespace iroha

#endif  // IROHA_VALIDATION_STATELESS_VALIDATOR_HPP
