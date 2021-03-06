cmake_minimum_required (VERSION 2.6)
project(a2GoAT)

# check for in-source build, forbid it!
if(CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR AND NOT MSVC_IDE)
  message(FATAL_ERROR "\nIn-source build attempt detected!\n"
    "Please create a new directory (e.g. build) and run `cmake ..`.\n"
    "Also don't forget to delete the created CMakeCache.txt and CMakeFiles dir"
    )
endif()
    
include(cmake/settings.cmake)
include(cmake/doxygen.cmake)

message(STATUS "*** Build Type:      " ${CMAKE_BUILD_TYPE})
message(STATUS "*** Compiler Flags:  " ${DEFAULT_COMPILE_FLAGS})
message(STATUS "*** Install libs to: " ${LIBRARY_OUTPUT_PATH})
message(STATUS "*** Install bin to:  " ${EXECUTABLE_OUTPUT_PATH})

# require a fairly recent ROOT version
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
find_package(ROOT "5.30" REQUIRED)

# since all subprojects need ROOT, set that up here
include_directories(${ROOT_INCLUDES})
link_directories(${ROOT_LIBRARY_DIR})

include_directories(inc)
set(ROOT_LIBRARIES "${ROOT_LIBRARIES} -lEG")
message(${ROOT_LIBRARIES})

add_executable(goat
   src/GoAT.cc 
   src/GSort.cc
   src/GParticleReconstruction.cc
   src/GDataChecks.cc
   src/GoATTreeManager.cc
   src/GAcquTreeManager.cc
   )
target_link_libraries(goat ${ROOT_LIBRARIES})

add_executable(pi0-dylan
   src/PPi0Dylan.cc
   src/PPhysics.cc
   src/GoATTreeManager.cc
   src/GAcquTreeManager.cc
   )
target_link_libraries(pi0-dylan ${ROOT_LIBRARIES})

# install some scripts to the bin directory
# by creating symlinks
file(GLOB CORE_EXTRA_SCRIPTS "scripts/*")
foreach(f ${CORE_EXTRA_SCRIPTS})
  get_filename_component(f_name ${f} NAME)
  add_custom_target(link_${f_name} ALL
    COMMAND ${CMAKE_COMMAND} -E create_symlink "${f}" "${EXECUTABLE_OUTPUT_PATH}/${f_name}")
endforeach()
