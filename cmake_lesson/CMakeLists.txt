cmake_minimum_required (VERSION 3.2)

project (cmakeProject VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

configure_file(
  "${PROJECT_SOURCE_DIR}/Version.h.in"
  "${PROJECT_SOURCE_DIR}/Version.h"
)
#configure_file(Version.h.in Version.h)


#executable
add_executable(cmakeExec main.cpp)


#dummy library
add_library(version lib.cpp)

#tests
#add_executable (test_version test_version.cpp)
enable_testing()
add_test(test_version test_version)

#compile properties for all targets 
set_target_properties(cmakeExec version  PROPERTIES
 CXX_STANDART 14
 CXX_STANDART_REQUIRED ON
 COMPILE_OPTIONS -Wpedantic -Wall -Wextra)

#compile properties for tests only
set_target_properties(test_version PROPERTIES
 COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
 INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)

#linking exec with dummy lib
target_link_libraries( cmakeExec version)

#linking tests with dummy lib and boost
target_link_libraries(test_version ${Boost_LIBRARIES} version)



#install Targets
install(TARGETS cmakeExec RUNTIME DESTINATION bin)

#package generation
set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}") 
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}" )
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}" )

set(CPACK_PACKAGE_CONTACT grinich001@yandex.ru)

include(CPack)

