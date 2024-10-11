add_executable(2dof_example 2dof_example.c)
target_link_libraries(2dof_example 2dof)

if(ENABLE_KORTEX)
  add_executable(kinova_gen3_example kinova_gen3_example.c)
  target_link_libraries(kinova_gen3_example kinova_gen3)
endif()

if(ENABLE_ETHERCAT)
  if(ENABLE_KELO)
    add_executable(freddy_2_example freddy_2_example.c)
    target_link_libraries(freddy_2_example kelo ethercat)

    if(ENABLE_INSTALL_TARGETS)
      install(TARGETS freddy_2_example DESTINATION bin/${PROJECT_NAME})
    endif()
  endif()
endif()
