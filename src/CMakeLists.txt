add_subdirectory(backend)

set(
  SOURCES

  color.c
  kelvin.c
  gamma.c
  main.c
  )

add_executable(openlux ${SOURCES})
target_link_libraries(
  openlux

  m ${X11_LIBRARIES} ${XF86VM_LIBRARIES}
  ol_backend_os ol_backend_video ol_backend_gamma
  )

install(
  TARGETS
  openlux

  RUNTIME DESTINATION bin
  )
