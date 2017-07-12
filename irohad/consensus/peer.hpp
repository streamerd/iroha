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
#include <boost/sml.hpp>
#include <common/types.hpp>
#include <peer_service/PeerService.hpp>
#include "consensus_service.hpp"
#include "smlogger.hpp"

// Note (@warchant) LOOK AT test_network in test/scenario

namespace iroha {

  namespace sml = boost::sml;

  struct PeerState {
    ed25519::keypair_t keypair;
    PeerService peerService;

    /**
     * imitation of ledger, for debug
     */
    uint64_t height;
    std::vector<int> ledger;
  };

  class Peer {
   public:
    Peer(ed25519::keypair_t keypair, std::string ip, uint16_t port,
         PeerService ps) {
      listen_on = ip + ":" + std::to_string(port);
      console_ = spdlog::stdout_color_st(listen_on);
      // set debug logging level
      console_->set_level(spdlog::level::debug);

      state_.keypair = keypair;
      state_.peerService = ps;

      bind_callbacks();
    }

    /**
     * Run event loop. Blocking operation.
     */
    void run() {
      // run peer service
      state_.peerService.run();

      // temp solution
      grpc::ServerBuilder builder;
      builder.AddListeningPort(listen_on, grpc::InsecureServerCredentials());
      builder.RegisterService(&server_);
      auto server(builder.BuildAndStart());

      console_->info("Current order: {}", state_.peerService.orderToString());
      console_->debug("started");

      server->Wait();

    }

   private:
    void bind_callbacks() {
      server_.on<uvw::ErrorEvent>([c{console_}](const uvw::ErrorEvent& e,
                                                auto& s) {
        c->error("{}", e.what());
        throw std::system_error();
      });

      /*
      server_.on<Proposal*>([c{console_}](const Proposal& p, auto& s) {
        c->debug("Proposal received");
      });

      server_.on<Vote*>([c{console_}](const Vote& v, auto& s) {
        c->debug("Vote received");
      });

      server_.on<Commit>*([c{console_}](const Commit& c, auto& s) {
        c->debug("Commit received");
      });
       */
    }

    std::string listen_on;

    std::shared_ptr<spdlog::logger> console_;

    ConsensusService server_;

    PeerState state_;
    smlogger logger_;  // for debug
    // sml::sm<PeerState, sml::logger<smlogger>> sm_;
  };
}
#endif  // IROHA_CONSENSUS_HPP
