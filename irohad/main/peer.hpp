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
#include <peer_service/PeerService.hpp>
#include <uvw.hpp>
#include "consensus/consensus_service.hpp"

// Note (@warchant) LOOK AT test_network in test/scenario

namespace iroha {

  class Peer {
   public:
    Peer(ed25519::keypair_t keypair, std::string ip, uint16_t port,
         std::shared_ptr<PeerService> ps) {
      listen_on = ip + ":" + std::to_string(port);
      console_ = spdlog::stdout_color_st(listen_on);
      // set debug logging level
      console_->set_level(spdlog::level::debug);

      // bad solution: we calculate our position (and role) here at peer
      // creation
      auto pos = ps->position(keypair.pubkey);
      if (pos == 0) {
        state_ = std::make_shared<Leader>();
      } else if (pos == 2 * ps->f() + 1) {
        state_ = std::make_shared<ProxyTail>();
      } else if (pos > 0 && pos < 2 * ps->f() + 1) {
        state_ = std::make_shared<Validator>();
      } else {
        state_ = std::make_shared<Member>();
      }

      state_->keypair = std::move(keypair);
      state_->peerService = std::move(ps);

      bind_callbacks();
    }

    /**
     * Run event loop. Blocking operation.
     */
    void run() {
      // run peer service
      state_->peerService->run();
      // timeout is 5 seconds
      state_->orderingService->start(uvw::TimerHandle::Time{5});

      // simulate client
      // creates separate thread which pushes transaction in queue
      state_->orderingService->simulate();

      // temp solution
      grpc::ServerBuilder builder;
      builder.AddListeningPort(listen_on, grpc::InsecureServerCredentials());
      builder.RegisterService(&server_);
      auto server(builder.BuildAndStart());
      // end

      console_->info("Current order: {}", state_->peerService->orderToString());
      console_->debug("started");

      server->Wait();
    }

   private:
    void bind_callbacks() {
      // temp solution
      server_.on<uvw::ErrorEvent>([c{console_}](const uvw::ErrorEvent& e,
                                                auto& s) {
        c->error("{}", e.what());

        // TODO: we need better exception management
        throw std::system_error();
      });

      std::shared_ptr<Member> state{state_};

      server_.on<Proposal*>(
          [ c{console_}, state{state} ](const Proposal* p, auto& s) {
            c->debug("Proposal received");
            state->on_proposal(p);
          });

      server_.on<Vote*>([ c{console_}, state{state} ](const Vote* v, auto& s) {
        c->debug("Vote received");
        state->on_vote(v);
      });

      server_.on<Commit*>(
          [ c{console_}, state{state} ](const Commit* c, auto& s) {
            c->debug("Commit received");
            state->on_commit(c);
          });

      server_.on<Abort*>(
          [ c{console_}, state{state} ](const Abort* c, auto& s) {
            c->debug("Abort received");
            state->on_abort(c);
          });
    }

    std::string listen_on;

    ConsensusService server_;

    std::shared_ptr<Member> state_;
    std::shared_ptr<spdlog::logger> console_;
  };
}
#endif  // IROHA_CONSENSUS_HPP
