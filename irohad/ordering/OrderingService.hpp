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

#ifndef IROHA_ORDERINGSERVICE_HPP
#define IROHA_ORDERINGSERVICE_HPP

#include <spdlog/spdlog.h>
#include <consensus/messages.hpp>
#include <queue>
#include <uvw.hpp>

namespace ordering {

  class OrderingService : public uvw::Emitter<OrderingService> {
   public:
    OrderingService(std::shared_ptr<uvw::Loop> loop = uvw::Loop::getDefault());

    void run();
    void stop();

    void simulate_one();

    void append(const iroha::Transaction& tx);
   private:
    void bind_timer();

    void create_proposal();

    std::shared_ptr<uvw::TimerHandle> timer_;

    // TODO:  should be changed to MPSC lock-free bounded queue
    // (multiple producers single consumer, does not use locks, bounded by
    // "block size")
    std::shared_ptr<std::queue<iroha::Transaction>> queue_;

    std::shared_ptr<uvw::Loop> loop_;
  };
}

#endif  // IROHA_ORDERINGSERVICE_HPP
