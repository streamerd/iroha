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

#ifndef IROHA_PEERSERVICE_HPP
#define IROHA_PEERSERVICE_HPP

#include <common/types.hpp>
#include <uvw/emitter.hpp>

namespace iroha {
  struct NetworkNode {
    std::string ip;
    uint16_t port;
    ed25519::pubkey_t pubkey;
  };

  struct PeerService {
    std::vector<NetworkNode> peers;

    /**
     * Dummy function for "PeerService" run
     */
    void run() {
      // now order is static: lexicographical sort of public keys for peers
      makeOrder();
    }

    void makeOrder() {
      std::sort(peers.begin(), peers.end(), [](const NetworkNode &a,
                                               const NetworkNode &b) {
        return std::lexicographical_compare(a.pubkey.begin(), a.pubkey.end(),
                                            b.pubkey.begin(), b.pubkey.end());
      });
    }

    // debug function
    std::string orderToString() {
      std::string s = "[\n";
      auto size = peers.size();
      for (auto i = 0u; i < size; i++) {
        s += "  ";
        s += peers[i].pubkey.to_hexstring();
        if (i != size - 1) s += ",\n";
      }
      s += "\n]\n";
      return s;
    }
  };
}

#endif  // IROHA_PEERSERVICE_HPP
