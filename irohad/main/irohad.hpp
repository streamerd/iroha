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

#ifndef IROHA_CONSENSUS_HPP
#define IROHA_CONSENSUS_HPP

#include <grpc++/grpc++.h>
#include <spdlog/spdlog.h>
#include <common/types.hpp>
#include <consensus/messages.hpp>
#include <consensus/state/leader.hpp>
#include <consensus/state/member.hpp>
#include <consensus/state/proxy_tail.hpp>
#include <ordering/OrderingService.hpp>
#include <peer_service/PeerService.hpp>
#include <uvw.hpp>
#include "consensus/consensus_service.hpp"


namespace iroha {

  class Irohad {
   public:
      Irohad(ed25519::keypair_t keypair, std::string ip, uint16_t port,
         std::shared_ptr<PeerService> ps, std::shared_ptr<uvw::Loop> loop = uvw::Loop::getDefault());

    /**
     * Run event loop. Blocking operation.
     */
    void run();

   private:
    void bind_callbacks();

    std::string listen_on;

    ConsensusService server_;

    std::shared_ptr<ordering::OrderingService> orderingService;
    std::shared_ptr<Member> state_;
    std::shared_ptr<spdlog::logger> console_;

    std::shared_ptr<uvw::Loop> loop_;
  };
}
#endif  // IROHA_CONSENSUS_HPP
