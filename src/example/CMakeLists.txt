add_executable(2dof_example 2dof_example.c)
target_link_libraries(2dof_example 2dof)

if(ENABLE_KORTEX)
  add_executable(kinova_gen3_example kinova_gen3_example.c)
  target_link_libraries(kinova_gen3_example kinova_gen3)

  if(ENABLE_INSTALL_TARGETS)
    set_target_properties(kinova_gen3_example PROPERTIES
      INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}"
      INSTALL_RPATH_USE_LINK_PATH TRUE
    )
    install(TARGETS kinova_gen3_example DESTINATION bin/${PROJECT_NAME})
  endif()
endif()

if(ENABLE_ETHERCAT)
  if(ENABLE_KELO)
    add_executable(eddie_example eddie_example.c)
    target_link_libraries(eddie_example kelo ethercat)

    if(ENABLE_INSTALL_TARGETS)
      set_target_properties(eddie_example PROPERTIES
        INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}"
        INSTALL_RPATH_USE_LINK_PATH TRUE
      )
      install(TARGETS eddie_example DESTINATION bin/${PROJECT_NAME})
    endif()
  endif()
endif()
