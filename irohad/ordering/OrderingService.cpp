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

auto console = spdlog::stdout_color_st("orderingService");

namespace ordering {

  void OrderingService::create_proposal() {
    iroha::Proposal proposal;

    // while not empty AND TODO: the number of transactions inside < BLOCK_SIZE
    while (!queue_->empty()) {
      iroha::Transaction tx = queue_->front();
      queue_->pop();
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

    auto &t = *this;
    timer_->on<uvw::TimerEvent>([ &t](const uvw::TimerEvent &e,
                                               uvw::TimerHandle &handle) {
      // TODO
      if (!t.queue_->empty()) {
        ::console->info("{} transactions in ordering service", t.queue_->size());
        t.create_proposal();
      } else {
        ::console->info("No transactions in ordering service");
      }
    });
  }

  void OrderingService::run() {
    console->info("started");
    timer_->start(uvw::TimerHandle::Time{0},
                  uvw::TimerHandle::Time{5000});  // repeated timer
  }

  void OrderingService::stop() { timer_->stop(); }

  void OrderingService::append(const iroha::Transaction& tx){
    queue_->push(tx);
  }

  // for debug
  void OrderingService::simulate_one() {
    iroha::Transaction tx;
    tx.set_test(std::to_string(rand()));
    queue_->push(tx);
  }

  OrderingService::OrderingService(std::shared_ptr<uvw::Loop> loop)
      : loop_{loop} {
    queue_ = std::make_shared<std::queue<iroha::Transaction>>();
    bind_timer();
  }
}
