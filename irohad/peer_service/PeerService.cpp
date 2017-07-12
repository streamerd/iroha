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

#include "PeerService.hpp"

NetworkNode::NetworkNode(std::string ip_, uint16_t port_,
                         ed25519::pubkey_t pub_)
    : ConsensusClient(ip_, port_),
      ip(std::move(ip_)),
      port(std::move(port_)),
      pubkey(std::move(pub_)) {}

std::shared_ptr<NetworkNode> PeerService::leader() {
  // TODO
  return peers[0];
}

std::shared_ptr<NetworkNode> PeerService::proxy_tail() {
  // TODO
  return peers[2 * f() + 1];
}

size_t PeerService::f() const { return (peers.size() - 1) / 3; }

size_t PeerService::position(ed25519::pubkey_t pub) {
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

void PeerService::run() {
  // now order is static: lexicographical sort of public keys for peers
  makeOrder();
}

void PeerService::makeOrder() {
  std::sort(peers.begin(), peers.end(), [](std::shared_ptr<NetworkNode> a,
                                           std::shared_ptr<NetworkNode> b) {
    return std::lexicographical_compare(a->pubkey.begin(), a->pubkey.end(),
                                        b->pubkey.begin(), b->pubkey.end());
  });
}
