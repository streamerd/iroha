#include <stdio.h>
#include <stdlib.h>
#include <uv.h>

int main(int argc, char **argv) {
  uv_loop_t *loop = uv_default_loop();



  return uv_run(loop, UV_RUN_DEFAULT);
}