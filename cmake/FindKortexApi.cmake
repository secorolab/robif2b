include(GNUInstallDirs)

# check if the KortexApi library is installed
if(NOT EXISTS "${CMAKE_INSTALL_FULL_LIBDIR}/libKortexApiCpp.a" OR NOT EXISTS "${CMAKE_INSTALL_FULL_INCLUDEDIR}/kortex_api/client/SessionManager.h")
  message(WARNING "KortexApi library not found in ${CMAKE_INSTALL_FULL_LIBDIR} or ${CMAKE_INSTALL_FULL_INCLUDEDIR}/kortex_api/")

  # if auto-download is enabled, download the library
  if(ENABLE_KORTEX_API_AUTO_DOWNLOAD)
    Set(FETCHCONTENT_QUIET FALSE)

    File(DOWNLOAD 
        https://artifactory.kinovaapps.com:443/artifactory/generic-public/kortex/API/2.6.0/linux_x86_64_gcc_5.4.zip
        ${CMAKE_CURRENT_BINARY_DIR}/kortex_api.zip
        SHOW_PROGRESS STATUS DOWNLOAD_RESULT
    )
    list(GET DOWNLOAD_RESULT 0 DOWNLOAD_RESULT_CODE)
    if(NOT DOWNLOAD_RESULT_CODE EQUAL 0)
        message(FATAL_ERROR "Failed downloading! Error: ${DOWNLOAD_RESULT}.")
    endif()
    message("File downloaded successfully at ${CMAKE_CURRENT_BINARY_DIR}/kortex_api.zip")

    # create kortex_api directory in install directory
    file(MAKE_DIRECTORY ${CMAKE_INSTALL_FULL_INCLUDEDIR}/kortex_api)
    message("Unzipping to ${CMAKE_CURRENT_BINARY_DIR}/kortex_api")
    execute_process(COMMAND unzip ${CMAKE_CURRENT_BINARY_DIR}/kortex_api.zip -d ${CMAKE_CURRENT_BINARY_DIR}/kortex_api OUTPUT_QUIET)
    message("Copying library...")
    file(COPY ${CMAKE_CURRENT_BINARY_DIR}/kortex_api/lib/release/libKortexApiCpp.a DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR})
    file(COPY ${CMAKE_CURRENT_BINARY_DIR}/kortex_api/lib/release/libKortexApiCpp_without_protobuf.a DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR})
    message("Copying include directory...")
    file(COPY ${CMAKE_CURRENT_BINARY_DIR}/kortex_api/include/ DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR}/kortex_api)
    message("Cleaning...")
    file(REMOVE ${CMAKE_CURRENT_BINARY_DIR}/kortex_api.zip)
    file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/kortex_api)
    message("Done.")
  else()
    message(FATAL_ERROR "KortexApi library not found. Please install manually the library to ${CMAKE_INSTALL_FULL_LIBDIR} and header files to ${CMAKE_INSTALL_FULL_INCLUDEDIR}/kortex_api/ or enable auto-download.")
  endif()
endif()

set(KORTEXAPI_INCLUDE_DIRS
  "${CMAKE_INSTALL_FULL_INCLUDEDIR}/kortex_api"
  "${CMAKE_INSTALL_FULL_INCLUDEDIR}/kortex_api/client"
  "${CMAKE_INSTALL_FULL_INCLUDEDIR}/kortex_api/client_stubs"
  "${CMAKE_INSTALL_FULL_INCLUDEDIR}/kortex_api/common"
  "${CMAKE_INSTALL_FULL_INCLUDEDIR}/kortex_api/google"
  "${CMAKE_INSTALL_FULL_INCLUDEDIR}/kortex_api/messages"
)

set(KORTEXAPI_LIBRARIES
  "${CMAKE_INSTALL_FULL_LIBDIR}/libKortexApiCpp.a"
  "${CMAKE_INSTALL_FULL_LIBDIR}/libKortexApiCpp_without_protobuf.a"
  pthread
)
