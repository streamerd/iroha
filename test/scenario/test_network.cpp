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

#include <crypto/crypto.hpp>
#include <main/irohad.hpp>
#include <memory>
#include <uvw/loop.hpp>

using namespace iroha;
using namespace ed25519;

#define NPEERS 4

class Network {
 public:
  Network() {
    // generate N keypairs
    std::vector<keypair_t> keypairs;
    for (auto i = 0u; i < NPEERS; i++) {
      auto keypair = create_keypair(create_seed());
      keypairs.emplace_back(keypair);
    }

    // create N peers, each has the same ordering service. each peer is started
    // in a separate thread.
    for (auto i = 0u; i < NPEERS; i++) {
      auto host = std::string("127.0.0.1");
      uint16_t port = (uint16_t)(10000u + i);
      std::shared_ptr<PeerService> peer_service =
          std::make_shared<PeerService>();

      // for each peer add the same set of public keys as initial peerService
      for (auto j = 0u; j < NPEERS; j++) {
        auto pk = keypairs[j].pubkey;
        std::shared_ptr<NetworkNode> nn =
            std::make_shared<NetworkNode>(host, port, pk);

        peer_service->peers.push_back(std::move(nn));
      }

      std::shared_ptr<Irohad> peer(
          new Irohad(keypairs[i], host, port, peer_service));

      // run every peer in a separate thread
      auto thread = std::thread([peer{peer}]() { peer->run(); });

      thread.detach();

      peers.push_back(std::move(peer));
      threads.push_back(std::move(thread));
    }
  }

  std::vector<std::shared_ptr<Irohad>> peers;
  std::vector<std::thread> threads;
};

int main(){  // client thread
  Network network;

  while (1) {
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
  }

  return 0;
}
