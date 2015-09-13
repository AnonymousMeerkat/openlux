set(SOURCES
  time.c
  posix.c
  )

if (BUILD_IOS)
  set(SOURCES ${SOURCES} mach.c)
endif (BUILD_IOS)

add_library(ol_backend_time STATIC ${SOURCES})
