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

#include <spdlog/spdlog.h>
#include <boost/sml.hpp>
#include <common/types.hpp>
#include <peer_service/PeerService.hpp>
#include <uvw/udp.hpp>
#include "smlogger.hpp"

// Note (@warchant) LOOK AT test_network in test/scenario

namespace iroha {

  namespace sml = boost::sml;

  struct PeerState {
    ed25519::keypair_t keypair;
    PeerService network;

    /**
     * imitation of ledger, for debug
     */
    uint64_t height;
    std::vector<int> ledger;
  };

  class Peer {
   public:
    Peer(ed25519::keypair_t keypair, uvw::Addr addr, PeerService ps,
         std::shared_ptr<uvw::Loop> loop = uvw::Loop::getDefault()) {
      console_ =
          spdlog::stdout_color_st(addr.ip + ":" + std::to_string(addr.port));
      // set debug logging level
      console_->set_level(spdlog::level::debug);

      loop_ = loop;
      server_ = loop_->resource<uvw::UDPHandle>();

      state_.keypair = keypair;
      state_.network = ps;

      // callback on any error event
      server_->on<uvw::ErrorEvent>([console_{console_}](
          const uvw::ErrorEvent &e, uvw::UDPHandle &u) {
        console_->error("{}", e.what());
        throw std::system_error();
      });

      // event is fired when we receive something on our socket
      server_->on<uvw::UDPDataEvent>([console_{console_}](
          const uvw::UDPDataEvent &e, uvw::UDPHandle &u) {

        // receive data here!!
        std::string s{e.data.get(), e.data.get() + e.length};
        console_->debug("LENGTH: {}, DATA: {}", e.length, s);
      });

      server_->bind(addr);
      server_->recv();

      console_->debug("started");
    }

    /**
     * Run event loop. Blocking operation.
     */
    void run() {
      state_.network.run();

      console_->info("Current order: {}", state_.network.orderToString());

      loop_->run();
    }



   private:
    std::shared_ptr<spdlog::logger> console_;
    std::shared_ptr<uvw::Loop> loop_;
    std::shared_ptr<uvw::UDPHandle> server_;

    PeerState state_;
    smlogger logger_;  // for debug
    // sml::sm<PeerState, sml::logger<smlogger>> sm_;
  };
}
#endif  // IROHA_CONSENSUS_HPP
