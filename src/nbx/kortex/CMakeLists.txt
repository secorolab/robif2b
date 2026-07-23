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

# KortexApiCpp.a bundles its own protobuf runtime. Keep its internal protobuf
# references bound to that runtime and hide the static-library symbols from
# consumers to prevent conflicts with another protobuf runtime in the process.
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
