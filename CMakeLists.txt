# Created by the script cgal_create_CMakeLists
# This is the CMake script for compiling a set of CGAL applications.

project( main )

cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BUILD_TYPE Release)

if(UNIX) 
    message(STATUS ">>> Linux")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
elseif(WIN32)
    message(STATUS ">>> Not Linux")
    set(CMAKE_CXX_FLAGS_RELEASE "-Ox")
endif()

# CGAL and its components
find_package( CGAL QUIET COMPONENTS  )

if ( NOT CGAL_FOUND )

  message(STATUS "This project requires the CGAL library, and will not be compiled.")
  return()  

endif()

# include helper file
include( ${CGAL_USE_FILE} )

# Boost and its components
find_package( Boost REQUIRED )

if ( NOT Boost_FOUND )

  message(STATUS "This project requires the Boost library, and will not be compiled.")

  return()  

endif()

include_directories( ${PROJECT_SOURCE_DIR}/include )

add_executable( mismoradio_de  mismoradio_de.cpp )
add_executable( distintoradio_de distintoradio_de.cpp )
add_executable( mismoradio_gd  mismoradio_gd.cpp )
add_executable( distintoradio_gd distintoradio_gd.cpp )
add_executable( playground playground.cpp )

add_to_cached_list( CGAL_EXECUTABLE_TARGETS mismoradio_de )
add_to_cached_list( CGAL_EXECUTABLE_TARGETS distintoradio_de )
add_to_cached_list( CGAL_EXECUTABLE_TARGETS mismoradio_de )
add_to_cached_list( CGAL_EXECUTABLE_TARGETS distintoradio_de )
add_to_cached_list( CGAL_EXECUTABLE_TARGETS playground )

# Link the executable to CGAL and third-party libraries
target_link_libraries(mismoradio_de   ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES} )
target_link_libraries(distintoradio_de   ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES} )
target_link_libraries(mismoradio_gd   ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES} )
target_link_libraries(distintoradio_gd   ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES} )
target_link_libraries(playground   ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES} )
