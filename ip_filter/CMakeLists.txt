cmake_minimum_required (VERSION 3.2)

project (ip_filter VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#executable
add_executable(ip_filter ip_filter.cpp filter_utils.cpp)

#tests
add_executable (test_filter_exec tests.cpp filter_utils.cpp)
enable_testing()
add_test(test_filters test_filter_exec)

#compile properties for all targets 
set_target_properties(
ip_filter test_filter_exec
  PROPERTIES
# CXX_STANDART 14
# CXX_STANDART_REQUIRED ON
 COMPILE_OPTIONS -Wpedantic -Wall -Wextra)

#compile properties for tests only
set_target_properties(test_filter_exec PROPERTIES
 COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
 INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)


#linking tests with dummy lib and boost
target_link_libraries(test_filter_exec ${Boost_LIBRARIES} )


#install Targets
install(TARGETS ip_filter RUNTIME DESTINATION bin)

#package generation
set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}") 
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}" )
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}" )

set(CPACK_PACKAGE_CONTACT grinich001@yandex.ru)

include(CPack)

