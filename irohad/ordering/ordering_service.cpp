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

#include <ordering/ordering_service.hpp>

namespace ordering_service {

  /**
   * append to ordering queue
   */
  void append(Transaction& tx) {
    detail::que_.emplace( std::move(tx) );
  }

  /**
   * on_commit, start ordering
   */
  void on_commit(Block& block) {
    // ChainValidatio(block)    if( )
    {
      detail::initialize(block);
    }
    // if( !isOrderigPeer )
    {
      return;
    }

    while(true) {
      uint64_t wait_time = 2; // relate to order
      // wait( waittime );

      // if ( isComitting )
      {
        break;
      }

      if( detail::que_.empty() ) {
        auto block = std::move(detail::makeBlock());
        //if( isComitting )
        {
          //consensus::connection::sendBlock(block);
        }
      }
    }

  }

  namespace detail {

    void initialize(const Block& block) {
      uint64_t last = (--block.body().txs().end())->header().created_time();
      while( !que_.empty() ){
        if( last < que_.top().header().created_time() ) break;
      }
    }

    Block makeBlock() {
      Block block;
      while(!que_.empty()){
        const Transaction& tx = que_.top(); que_.pop();
        block.mutable_body()->add_txs()->CopyFrom(tx);
      }
      return block;
    }
  }

}  // namespace ordering