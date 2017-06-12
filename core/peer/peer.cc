#include <stdio.h>
#include <stdlib.h>
#include <uv.h>
#include "peer/api/client-connection/GrpcServerRunner.h"

int main(int argc, char **argv) {

  grpc_connection::GrpcServerRunner serviceManager("0.0.0.0", 50051);
  //serviceManager.addService(someService);
  serviceManager.run();

  // TODO: do we need it?
  uv_loop_t *loop = uv_default_loop();
  return uv_run(loop, UV_RUN_DEFAULT);
}
