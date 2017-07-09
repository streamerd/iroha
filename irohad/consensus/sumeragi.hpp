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

#include <consensus/roles.hpp>
#include <consensus/connection/communicator.hpp>
#include <ametsuchi/ametsuchi.hpp>

namespace consensus {
  namespace sumeragi {

    using state_t = int;

    enum class LeaderStates : state_t {
      IDLE,
      PREPARING,
      FINISHING,
    };

    enum class ValidatorStates : state_t {
      IDLE,
      PREPARING,
    };

    enum class ProxyStates : state_t {
      IDLE,
      PREPARING,
    };

    class Sumeragi {
    public:
      Sumeragi(std::shared_ptr<Comminicator>&, Peer self);

      /**
       * Function for incoming messages from other peers
       */
      void onMessage(std::pair<Peer, data_t>);
      /**
       * Entry point for the leader
       */
      void propose(model::Block&);
    private:
      std::shared_ptr<Comminicator> &comm;
      Peer self;
      state_t state;
    };
  }
}
