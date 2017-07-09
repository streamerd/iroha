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

#include "connection/communicator.hpp"
#include <vector>
#include <pair>
/*
~~ Storing ~~

proxy:
`idle: O(1)`
`ready: O(n)`

validator:
`idle: O(1)`
`preparing: O(1)`

leader:
`idle: O(1)`
`preparing: O(n)`
`waiting for commit: O(1)` 
*/
namespace consensus {
  namespace roles {

    using predicate = std::pair<Peer, bool>;

    struct RoleData {};

    // Storing data for caching peers in first phase
    struct Leader : RoleData {
      /**
       * Peers list and their responce status
       */
      std::vector<predicate> peers;
      /**
       * Amount of peers that was asked so far
       */
      size_t asked;

      Leader(const std::vector<Peer> &peers);
    };

    struct Verifier : RoleData {
      // nothing to store
    };

    // Storing data for caching peers in third phase
    struct Proxy : RoleData {
      std::vector<predicate> unconfirmed;

      Proxy(const std::vector<Peer> &peers);
    };


  }
}
