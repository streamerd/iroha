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

#ifndef IROHA_DAO_CRYPTO_PROVIDER_HPP
#define IROHA_DAO_CRYPTO_PROVIDER_HPP

#include <dao/dao.hpp>
namespace iroha {
  namespace dao {
    class DaoCryptoProvider {
     public:
      virtual bool verify(const iroha::dao::Transaction& tx) = 0;

      virtual iroha::Keypair::signature_t sign(
          const iroha::dao::Transaction& tx) = 0;
    };
  }
}
#endif  // IROHA_DAO_CRYPTO_PROVIDER_HPP
