set(SOURCES
  video.c
  )

if (BUILD_X11)
  set(SOURCES ${SOURCES} x11.c)
endif (BUILD_X11)

if (BUILD_IOS)
  set(SOURCES ${SOURCES} ios.c)
endif (BUILD_IOS)

add_library(ol_backend_video STATIC ${SOURCES})
