#include <stdio.h>
#include <stdlib.h>
#include <uv.h>

uv_tcp_t server;
uv_loop_t *loop;

void alloc_buffer(uv_handle_t *handle, size_t size, uv_buf_t *buf) {
  buf->base = (char *)malloc(size);
  buf->len = size;
}

void on_read(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf) {
  uv_write_t *req = (uv_write_t *)malloc(sizeof(uv_write_t));

  if (nread == -1) {
    /* if (uv_last_error(loop).code != UV_EOF) { */
    /* } */

    uv_close((uv_handle_t *)stream, NULL);
  }

  int r = uv_write(req, stream, buf, 1, NULL);

  if (r) {
    /* error */
  }

  free(buf->base);
}

void on_connection(uv_stream_t *server, int status) {
  uv_tcp_t *client = (uv_tcp_t *)malloc(sizeof(uv_tcp_t));

  if (status == -1) {
    /* error */
  }

  uv_tcp_init(loop, client);

  if (uv_accept(server, (uv_stream_t *)client) == 0) {
    int r = uv_read_start((uv_stream_t *)client, alloc_buffer, on_read);

    if (r) {
      /* error */
    }
  } else {
    uv_close((uv_handle_t *)client, NULL);
  }
}

int main(int argc, char **argv) {
  loop = uv_default_loop();

  struct sockaddr_in addr;
  uv_ip4_addr("0.0.0.0", 3000, &addr);

  uv_tcp_init(loop, &server);
  uv_tcp_bind(&server, (const struct sockaddr *)&addr, 0);

  int r = uv_listen((uv_stream_t *)&server, 128, on_connection);

  if (r) {
    /* error */
  }

  return uv_run(loop, UV_RUN_DEFAULT);
}