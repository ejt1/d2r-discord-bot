find_package(dpp CONFIG REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)

set(D2RDB_SRCS
  "${CMAKE_CURRENT_SOURCE_DIR}/src/Services/Classic/D2rServicesConnection.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/Services/Classic/DiabloCloneExternalListener.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/Services/Classic/GameVersionListener.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/Services/Classic/GenericResourceListener.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/Services/Classic/LeaderboardListener.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/Services/Classic/LimitedTimeEventListener.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/Services/Classic/TerrorZonesListener.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/Services/AuthenticationListener.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/Services/ConnectionService.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/Services/SessionListener.cpp"
  
  "${CMAKE_CURRENT_SOURCE_DIR}/src/BnetSession.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/Client.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/ClientRepository.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/DiabloClone.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/RealmSession.cpp"

  "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

add_executable(d2r-discord-bot ${D2RDB_SRCS})

target_include_directories(d2r-discord-bot PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")

target_link_libraries(d2r-discord-bot
  PRIVATE
    bgs::bgs-sdk

    dpp::dpp
    yaml-cpp)

install(TARGETS d2r-discord-bot DESTINATION ${CMAKE_INSTALL_PREFIX})

if (WIN32)
  install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/libcrypto-3-x64.dll"
    "${CMAKE_CURRENT_BINARY_DIR}/libssl-3-x64.dll"
    DESTINATION ${CMAKE_INSTALL_PREFIX})

  if (CMAKE_BUILD_TYPE STREQUAL "Release")
    install(FILES
      "${CMAKE_CURRENT_BINARY_DIR}/zlib1.dll"
      DESTINATION ${CMAKE_INSTALL_PREFIX})
  else()
    install(FILES
      "${CMAKE_CURRENT_BINARY_DIR}/zlibd1.dll"
      DESTINATION ${CMAKE_INSTALL_PREFIX})
  endif()
endif()

install(FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/app.dist.yml"
  DESTINATION ${CMAKE_INSTALL_PREFIX})

install(
  IMPORTED_RUNTIME_ARTIFACTS
    dpp::dpp
    yaml-cpp
  DESTINATION
    ${CMAKE_INSTALL_PREFIX})
