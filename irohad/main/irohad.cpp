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

#include "irohad.hpp"

namespace iroha {
  void Irohad::run() {
    // run peer service
    state_->peerService->run();

    // temp solution

    // end

    console_->info("Current order: {}", state_->peerService->orderToString());
    console_->debug("started");

    loop_->run();
  }

  void Irohad::bind_callbacks() {
    // temp solution
    server_.on<uvw::ErrorEvent>([c{console_}](const uvw::ErrorEvent& e,
                                              auto& s) {
      c->error("{}", e.what());

      // TODO: we need better exception management
      throw std::system_error();
    });

    std::shared_ptr<Member> state{state_};

    server_.on<Proposal*>(
        [ console{console_}, state{state} ](Proposal * p, auto& s) {
          console->debug("Proposal received");
          state->on_proposal(p);
        });

    server_.on<Vote*>([ console{console_}, state{state} ](Vote * v, auto& s) {
      console->debug("Vote received");
      state->on_vote(v);
    });

    server_.on<Commit*>(
        [ console{console_}, state{state} ](Commit * c, auto& s) {
          console->debug("Commit received");
          state->on_commit(c);
        });

    server_.on<Abort*>([ console{console_}, state{state} ](Abort * a, auto& s) {
      console->debug("Abort received");
      state->on_abort(a);
    });

    orderingService->on<Proposal>(
        [ c{console_}, state{state} ](Proposal & p, auto& s) {
          c->debug("Leader received proposal from OS, size: {}",
                   p.transactions_size());
          state->on_proposal(&p);
        });

    orderingService->on<uvw::ErrorEvent>([console{console_}](const auto &e, auto &s){
      console->error("{}", e.what());
    });
  }

  Irohad::Irohad(ed25519::keypair_t keypair, std::string ip, uint16_t port,
             std::shared_ptr<PeerService> ps, std::shared_ptr<uvw::Loop> loop) {
    loop_ = loop;
    listen_on = ip + ":" + std::to_string(port);
    console_ = spdlog::stdout_color_st(listen_on);
    // set debug logging level
    console_->set_level(spdlog::level::debug);

    orderingService = std::make_shared<ordering::OrderingService>(loop);

    // bad solution: we calculate our position (and role) here at peer
    // creation
    auto pos = ps->position(keypair.pubkey);
    if (pos == 0) {
      state_ = std::make_shared<Leader>();

      // run timer in ordering service
      this->orderingService->run();

      // simulate client
      // creates separate thread which pushes transaction in queue
      std::thread([os{this->orderingService}]() {
        while (1) {
          std::this_thread::sleep_for(std::chrono::milliseconds(1500));
          os->simulate_one();
        }
      }).detach();

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
}