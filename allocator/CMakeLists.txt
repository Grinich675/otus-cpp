cmake_minimum_required (VERSION 3.2)

project (allocator VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#executable
add_executable(allocator allocator.cpp)

#library with custom allocator
add_library(custom_allocator custom_allocator.hpp)

#library with custom container
add_library(custom_list custom_list.hpp)


#compile properties for all targets 
set_target_properties(
allocator custom_allocator custom_list
  PROPERTIES
 CXX_STANDART 14
 CXX_STANDART_REQUIRED ON
 LINKER_LANGUAGE CXX
 COMPILE_OPTIONS -Wpedantic -Wall -Wextra)



#linking tests with dummy lib and boost
target_link_libraries(allocator custom_allocator custom_list )


#install Targets
install(TARGETS allocator RUNTIME DESTINATION bin)

#package generation
set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}") 
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}" )
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}" )

set(CPACK_PACKAGE_CONTACT grinich001@yandex.ru)

include(CPack)
