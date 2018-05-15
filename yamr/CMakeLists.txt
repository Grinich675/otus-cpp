cmake_minimum_required(VERSION 3.2)

project (join_server VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
#project (join_server VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package (Threads)

set(_SOURCES
	FileInputFormat.cpp 
	FileOutputFormat.cpp
	FileSplit.cpp
	RecordReader.cpp
	yamr.cpp	
)

add_executable (yamr ${_SOURCES})

#compile properties for all targets 
set_target_properties(
yamr
  PROPERTIES
 CXX_STANDART 14
 CXX_STANDART_REQUIRED ON
 LINKER_LANGUAGE CXX
 COMPILE_OPTIONS -Wpedantic -Wall -Wextra)

target_link_libraries(yamr ${CMAKE_THREAD_LIBS_INIT})

#install Targets
install(TARGETS yamr RUNTIME DESTINATION bin)

#package generation
set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}") 
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}" )
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}" )

set(CPACK_PACKAGE_CONTACT grinich001@yandex.ru)

include(CPack)
