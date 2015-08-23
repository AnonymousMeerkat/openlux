set(SOURCES
  kelvin.c
  gamma.c
  main.c
  )

add_executable(openlux ${SOURCES})
target_link_libraries(openlux m ${X11_LIBRARIES} ${XF86VM_LIBRARIES})

install(TARGETS
  openlux
  RUNTIME DESTINATION bin
  )
