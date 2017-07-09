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

#include <ed25519.h>
#include <vector>
#include <pair>

// *** this defs shouldn't be there
using pubkey_t = ed25519::pubkey_t;
using Peer = pubkey_t;
using data_t = std::string;
// ***

struct Comminicator {
  /**
   * Retrieve list of the peers
   * peer's role in sumeragi should be determined based on this call
   */
  virtual std::vector<Peer> getPeers() = 0;

  /**
   * Sends multicast data to all listed peers
   * If any error occured return false
   * TODO: maybe return some err_t?
   */
  virtual bool send(const std::vector<Peer>&, const data_t&) = 0;

  /**
   * Sends data to specified peer
   * If any error occured return false
   */
  virtual bool send(const Peer&, const data_t&) = 0;

  /**
   * Recieve message from the peer network
   * Returns the pubkey of the sender and its data
   */
  // consensus itself should have a function onMessage
  // virtual std::pair<Peer, data_t> recv() = 0;

  Peer self;
};
