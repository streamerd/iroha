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
  struct NetworkNode : public ConsensusClient {
    NetworkNode(std::string ip_, uint16_t port_, ed25519::pubkey_t pub_)
        : ConsensusClient(ip_, port_),
          ip(std::move(ip_)),
          port(std::move(port_)),
          pubkey(std::move(pub_)) {}

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

    std::shared_ptr<NetworkNode> leader() {
      // TODO
      return peers[0];
    }

    std::shared_ptr<NetworkNode> proxy_tail() {
      // TODO
      size_t n = peers.size();
      size_t f = (n - 1) / 3;
      return peers[2 * f + 1];
    }

    size_t position(ed25519::pubkey_t pub) {
      size_t p = 0;
      for (auto &&peer : peers) {
        if (peer->pubkey == pub) {
          return p;
        }

        p++;
      }

      // temp
      // TODO: pubkey can not be found among peers
      throw std::system_error();
    }

    /**
     * Dummy function for "PeerService" run
     */
    void run() {
      // now order is static: lexicographical sort of public keys for peers
      makeOrder();
    }

    void makeOrder() {
      std::sort(peers.begin(), peers.end(), [](std::shared_ptr<NetworkNode> a,
                                               std::shared_ptr<NetworkNode> b) {
        return std::lexicographical_compare(a->pubkey.begin(), a->pubkey.end(),
                                            b->pubkey.begin(), b->pubkey.end());
      });
    }

    // debug function
    std::string orderToString() {
      std::string s = "[\n";
      auto size = peers.size();
      for (auto i = 0u; i < size; i++) {
        s += "  ";
        s += peers[i]->pubkey.to_hexstring();
        if (i != size - 1) s += ",\n";
      }
      s += "\n]\n";
      return s;
    }
  };
}

#endif  // IROHA_PEERSERVICE_HPP
