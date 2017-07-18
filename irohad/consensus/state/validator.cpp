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

#include "consensus/state/validator.hpp"

namespace iroha {

  void Validator::on_proposal(const Proposal *proposal) {
    // TODO

    // I don't like if-else/switch statements for states, can we do something
    // with this?
    // probably in future we may use map for O(1) state -> handler lookup
    // or we may use simply boost::sml
    switch (state) {
      case State::IDLE: {
        // proposal handling logic
        console_->info("Proposal handled");

        // dummy stateful validation
        Vote vote;
        {
          vote.set_next_height(height + 1);
          vote.set_next_gmroot(std::string(hash256_t::size(), 'a'));

          auto sig = vote.mutable_sig();
          std::string pub = this->keypair.pubkey.to_string();
          sig->set_signature(std::string(ed25519::sig_t::size(), 's'));

          // we spend 500 +- 100 ms to validate
          std::this_thread::sleep_for(
              std::chrono::milliseconds(400 + rand() % 200));
        }

        auto ack = this->peerService->proxy_tail()->SendVote(&vote);
        if (ack.type() == ack.VOTE_RECEIVED) {
          console_->info("Sent my vote");
          state = State::VOTED;
        } else {
          // TODO
          console_->error(
              "I sent my vote, but proxy tail did not respond with "
              "ACK.VOTE_RECEIVED");
        }
        break;
      }
      case State::VOTED: {
        console_->info("I already voted");
        break;
      }
      default: {
        // I hope we never reach this code block
        console_->critical("My state is neither IDLE nor VOTED");
        throw std::system_error();
      }
    }
  }

  Role Validator::self() { return Role::VALIDATOR; }

  Validator::Validator() {
    state = State::IDLE;
    console_ = spdlog::get("validator");
    if (!console_) {
      console_ = spdlog::stdout_color_st("validator");
    }
  }
}
