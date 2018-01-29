cmake_minimum_required (VERSION 3.2)

project (ip_print VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

find_package(Doxygen)

option(BUILD_DOCUMENTATION "Create and install the html version (req Doxygen)" ${DOXYGEN_FOUND})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#executable
add_executable(ip_print print_ip.cpp metaprinter.hpp)

#compile properties for all targets 
set_target_properties(
ip_print
  PROPERTIES
 CXX_STANDART 14
 CXX_STANDART_REQUIRED ON
 LINKER_LANGUAGE CXX
 COMPILE_OPTIONS -Wpedantic -Wall -Wextra)

if(BUILD_DOCUMENTATION)
	if(NOT DOXYGEN_FOUND)
		message(FATAL_ERROR "Doxygen not found")
	endif()
	set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/../docs/Doxyfile.in)
    	set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

	configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

	add_custom_target(doc
			COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
			COMMENT "Generating documentation woth Doxygen"
			VERBATIM)
	message("Doxygen configurated")
endif()



#install Targets
install(TARGETS ip_print RUNTIME DESTINATION bin)

#package generation
set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}") 
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}" )
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}" )

set(CPACK_PACKAGE_CONTACT grinich001@yandex.ru)

include(CPack)
