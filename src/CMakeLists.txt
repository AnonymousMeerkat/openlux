add_subdirectory(backend)

set(
  SOURCES

  animate.c
  color.c
  kelvin.c
  main.c
  )

add_executable(openlux ${SOURCES})
target_link_libraries(
  openlux

  m ${X11_X11_LIB} ${X11_Xxf86vm_LIB}
  ol_backend_os ol_backend_video ol_backend_gamma ol_backend_time
  )

install(
  TARGETS
  openlux

  RUNTIME DESTINATION bin
  )
