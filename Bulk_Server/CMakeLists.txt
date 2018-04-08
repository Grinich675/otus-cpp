cmake_minimum_required(VERSION 3.2)

project (bulk_server VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
#project (bulk_server VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package (Threads)
find_package(Boost COMPONENTS system REQUIRED)

add_executable (bulk_server main.cpp server.cpp Handler.cpp Loggers.cpp Session.cpp Storage.cpp Controller.cpp)

#compile properties for all targets 
set_target_properties(
bulk_server
  PROPERTIES
 CXX_STANDART 14
 CXX_STANDART_REQUIRED ON
 LINKER_LANGUAGE CXX
 COMPILE_OPTIONS -Wpedantic -Wall -Wextra)

target_link_libraries(bulk_server ${CMAKE_THREAD_LIBS_INIT}  ${Boost_LIBRARIES} )

#install Targets
install(TARGETS bulk_server RUNTIME DESTINATION bin)

#package generation
set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}") 
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}" )
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}" )

set(CPACK_PACKAGE_CONTACT grinich001@yandex.ru)

include(CPack)
