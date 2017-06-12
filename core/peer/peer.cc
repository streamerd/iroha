#include <stdio.h>
#include <stdlib.h>
#include "peer/api/client-connection/GrpcServerRunner.h"

int main(int argc, char **argv) {

  grpc_connection::GrpcServerRunner serviceManager("0.0.0.0", 50051);
  //serviceManager.addService(someService);
  serviceManager.run();

  return 0;
}
