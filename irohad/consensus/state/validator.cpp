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

#include "validator.hpp"

static auto console = spdlog::stdout_color_st("validator");

namespace iroha {

  void Validator::on_proposal(const Proposal *proposal) {
    // TODO

    // I don't like if-else/switch statements for states, can we do something
    // with this?
    // probably in future we may use map for O(1) state -> handler lookup
    // or we may use simply boost::sml
    switch (state) {
      case State::IDLE: {

      }
      case State::VOTED: {
        console->info("I already voted");
        break;
      }
      default: {
        // I hope we never reach this code block
        console->critical("My state is neither IDLE nor VOTED");
        throw std::system_error();
      }
    }
  }

  Role Validator::self() { return Role::VALIDATOR; }

  Validator::Validator() { state = State::IDLE; }
}
