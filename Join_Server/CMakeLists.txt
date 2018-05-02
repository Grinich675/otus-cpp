cmake_minimum_required(VERSION 3.2)

project (join_server VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
#project (join_server VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package (Threads)
find_package(Boost COMPONENTS system REQUIRED)

set(_SOURCES
	main.cpp 
	Server.cpp 
	RequestHandler.cpp 
	Client.cpp
	Client_manager.hpp 
	DBMS/Commands.cpp
	DBMS/CmdExecutor.hpp 
	DBMS/DB.cpp 
	DBMS/DBMS.cpp 
	DBMS/Table.cpp
)

add_executable (join_server ${_SOURCES})

#compile properties for all targets 
set_target_properties(
join_server
  PROPERTIES
 CXX_STANDART 14
 CXX_STANDART_REQUIRED ON
 LINKER_LANGUAGE CXX
 COMPILE_OPTIONS -Wpedantic -Wall -Wextra)

target_link_libraries(join_server ${CMAKE_THREAD_LIBS_INIT}  ${Boost_LIBRARIES} )

#install Targets
install(TARGETS join_server RUNTIME DESTINATION bin)

#package generation
set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}") 
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}" )
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}" )

set(CPACK_PACKAGE_CONTACT grinich001@yandex.ru)

include(CPack)
