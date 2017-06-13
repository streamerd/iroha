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

#include "client.h"
#include <thread>

using namespace iroha;

int main(int argc, char** argv) {
  size_t size = 0;
  try {
    if (argc == 2) {
      size = std::stoi(argv[1]);
    } else {
      throw std::exception();
    }
  } catch (std::exception &e) {
    std::cout << "First argument must be number of tx that will be sent\n";
    exit(1);
  }


  RandomClient client(grpc::CreateChannel("localhost:12345",
                                          grpc::InsecureChannelCredentials()));


  // Spawn reader thread that loops indefinitely
  std::thread thread_ = std::thread(&RandomClient::AsyncCompleteRpc, &client);


  for (uint32_t i = 0; i < size; i++) {
    client.SendRandomTx();  // The actual RPC call!
  }

  std::cout << "Press control-c to quit" << std::endl << std::endl;
  thread_.join();  // blocks forever

  return 0;
}