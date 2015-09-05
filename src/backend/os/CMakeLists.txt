set(
  SOURCES

  posix.c
  os.c
  )

add_library(ol_backend_os STATIC ${SOURCES})
