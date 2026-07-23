find_package(KortexApi)


add_library(kinova_gen3 SHARED kinova_gen3.cpp)

target_include_directories(kinova_gen3
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  PRIVATE
    ${KORTEXAPI_INCLUDE_DIRS}
)

target_link_libraries(kinova_gen3
  PRIVATE
    ${KORTEXAPI_LIBRARIES}
)

# KortexApiCpp.a contains its own protobuf 3.5 runtime, while generated
# motion-spec executables also use the system protobuf for frame logging.  Keep
# Kortex's internal protobuf references bound to its bundled runtime and hide
# those static-library symbols from the executable, preventing cross-runtime
# descriptor/arena calls during process startup.
target_link_options(kinova_gen3
  PRIVATE
    "-Wl,-Bsymbolic"
    "-Wl,--exclude-libs,ALL"
)

target_compile_definitions(kinova_gen3
  PRIVATE
    _OS_UNIX
)

target_compile_options(kinova_gen3
  PRIVATE
    -Wall -Wextra -pedantic
)

install(
  TARGETS kinova_gen3
  EXPORT ${PROJECT_NAME}-targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
