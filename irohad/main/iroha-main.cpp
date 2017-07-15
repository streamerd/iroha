/*
Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <uvw.hpp>
#include <string>
#include <logger/logger.hpp>
#include <main/irohad.hpp>
#include <crypto/crypto.hpp>

int main(int argc, char *argv[]) {
  auto loop = uvw::Loop::getDefault();

  loop->run();


  logger::setGlobalLogLevel(logger::level::debug);
  auto log = logger::Logger("main");
  log.debug("  print debug level log!");
  log.info("   print info level log!");
  log.warning("print waring level log!");
  log.error("  print error level log!");

  log.info(R"()");
  log.info(R"( dP  888888ba   .88888.  dP     dP   .d888888 )");
  log.info(R"( 88  88    `8b d8'   `8b 88     88  d8'    88 )");
  log.info(R"( 88 a88aaaa8P' 88     88 88aaaaa88a 88aaaaa88a)");
  log.info(R"( 88  88   `8b. 88     88 88     88  88     88 )");
  log.info(R"( 88  88     88 Y8.   .8P 88     88  88     88 )");
  log.info(R"( dP  dP     dP  `8888P'  dP     dP  88     88 )");
  log.info(R"( ---------o=========================o---------)");
  log.info(R"(         = 分散台帳Application いろは =         )");
  log.info(R"( ---------o=========================o---------)");

  auto peer_service =
    std::make_shared<iroha::PeerService>();
  auto keypair = iroha::create_keypair(iroha::create_seed());

/* ToDo configable(like sumeragi.json) , or use option
  for (auto j = 0u; j < NPEERS; j++) {
    auto pk = keypairs[j].pubkey;
    std::shared_ptr<NetworkNode> nn =
      std::make_shared<NetworkNode>(host, port, pk);

    peer_service->peers.push_back(std::move(nn));
  }
*/

  iroha::Irohad irohad(keypair,"0.0.0.0",16826,peer_service);
  irohad.run();
  return 0;
}
