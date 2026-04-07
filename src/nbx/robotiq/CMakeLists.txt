find_package(serial)

# Gripper
if(ENABLE_ROBOTIQ_GRIPPER)
find_package(robotiq_driver_noros)

add_library(robotiq_gripper SHARED robotiq_gripper.cpp)
target_include_directories(robotiq_gripper
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(robotiq_gripper
  PRIVATE
    robotiq::robotiq_driver_noros
)
target_compile_options(robotiq_gripper
  PRIVATE
    -Wall -Wextra -pedantic
)
install(
  TARGETS robotiq_gripper
  EXPORT ${PROJECT_NAME}-targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
endif()

# Force-torque Sensor
if(ENABLE_ROBOTIQ_FT)
find_package(robotiq_ft)

add_library(robotiq_ft_sensor SHARED robotiq_ft_sensor.c)
target_include_directories(robotiq_ft_sensor
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(robotiq_ft_sensor
  PRIVATE
    robotiq::robotiq_ft_driver
)
install(
  TARGETS robotiq_ft_sensor
  EXPORT ${PROJECT_NAME}-targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
endif()
