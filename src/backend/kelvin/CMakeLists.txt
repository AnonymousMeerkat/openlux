set(SOURCES
  kelvin.c
  tanner.c
  neilb.c
  flux.c
  flux_data.c
  )

add_library(ol_backend_kelvin STATIC ${SOURCES})
