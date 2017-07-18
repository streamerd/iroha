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
#include <consensus/consensus_client.hpp>
#include <uvw/emitter.hpp>
#include <uvw/timer.hpp>

namespace iroha {
  struct NetworkNode: ConsensusClient {
    NetworkNode(std::string ip_, uint16_t port_, ed25519::pubkey_t pub_);

    std::string ip;
    uint16_t port;
    ed25519::pubkey_t pubkey;

    bool online{false};
    // used to ping node with some certain timer
    std::shared_ptr<uvw::TimerHandle> timer;
  };

  class PeerService : public uvw::Emitter<PeerService> {
   public:
    std::vector<std::shared_ptr<NetworkNode>> peers;

    std::shared_ptr<NetworkNode> leader();
    std::shared_ptr<NetworkNode> proxy_tail();
    size_t f() const;
    size_t position(ed25519::pubkey_t pub);

    /**
     * Dummy function for "PeerService" run
     */
    void run();
    void makeOrder();

    // debug function
    std::string orderToString() {
      std::string s = "[\n";
      auto size = peers.size();
      for (auto i = 0u; i < size; i++) {
        s += "  ";
        s += peers[i]->pubkey.to_hexstring();
        s += " " + std::to_string(peers[i]->port);
        if (i != size - 1) s += ",\n";
      }
      s += "\n]\n";
      return s;
    }
  };
}

#endif  // IROHA_PEERSERVICE_HPP
