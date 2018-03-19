cmake_minimum_required (VERSION 3.2)

project (bulkmt VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
#project (bulkmt VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package (Threads)

#executable
add_executable(bulkmt Bulker_ST.cpp)
add_executable(bulkmt_perf Bulker_ST.cpp)


#compile properties for all targets 
set_target_properties(
bulkmt bulkmt_perf
  PROPERTIES
 CXX_STANDART 14
 CXX_STANDART_REQUIRED ON
 LINKER_LANGUAGE CXX
 COMPILE_OPTIONS -Wpedantic -Wall -Wextra)

target_link_libraries(bulkmt ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(bulkmt_perf ${CMAKE_THREAD_LIBS_INIT})

target_compile_definitions(bulkmt_perf PRIVATE PERF_TEST)

#install Targets
install(TARGETS bulkmt bulkmt_perf RUNTIME DESTINATION bin)
#install(TARGETS bulkmt_perf RUNTIME DESTINATION bin)

#package generation
set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}") 
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}" )
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}" )

set(CPACK_PACKAGE_CONTACT grinich001@yandex.ru)

include(CPack)
