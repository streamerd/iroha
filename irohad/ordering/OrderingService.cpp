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

#include "OrderingService.hpp"
#include <spdlog/spdlog.h>

static auto console = spdlog::stdout_color_st("orderingService");

namespace iroha {

  void iroha::OrderingService::create_proposal() {
    Proposal proposal;

    // while not empty AND TODO: the number of transactions inside < BLOCK_SIZE
    while (!queue_.empty()) {
      Transaction tx = queue_.front();
      proposal.add_transactions(tx.SerializeAsString());
    }

    publish(proposal);
  }

  void OrderingService::bind_timer() {
    timer_ = loop_->resource<uvw::TimerHandle>();
    timer_->on<uvw::ErrorEvent>(
        [](const uvw::ErrorEvent &e, uvw::TimerHandle &handle) {
          throw "timer is dead";  // TODO
        });

    timer_->on<uvw::TimerEvent>(
        [&console](const uvw::ErrorEvent &e, uvw::TimerHandle &handle) {
          // TODO
          if (!queue_->empty()) {
            console->info("{} transactions in ordering service", queue_->size());
            create_proposal();
          } else {
            console->info("No transactions in ordering service");
          }
        });
  }

  void OrderingService::start(uvw::TimerHandle::Time time) {
    timer_->start(time, uvw::TimerHandle::Time{1});  // repeated timer
  }

  void OrderingService::stop() { timer_->stop(); }

  template <typename T>
  void OrderingService::push(T &&tx) {
    queue_->push(std::forward<T>(tx));
  }

  void OrderingService::simulate() {
    std::thread([queue{queue_}]() {
      while (1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        Transaction tx;
        tx.set_test(std::to_string(q++));
        queue->push(tx);
      }
    }).detach();
  }
}
