#include "backend.h"

#include "x11.h"

int ol_backend_init(struct ol_backend_s* backend) {
  int ret = ol_backend_x11.init(&ol_backend_x11);
  if (!ret) {
    *backend = ol_backend_x11;
  }

  return ret;
}
