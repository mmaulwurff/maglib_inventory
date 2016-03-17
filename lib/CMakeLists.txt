set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build/")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build/")

include_directories(${PROJECT_SOURCE_DIR}/include)

set(SOURCES fixed_inventory.cpp)

add_library(maglib_inventory_static STATIC ${SOURCES} ../include/fixed_inventory.h)
add_library(maglib_inventory SHARED ${SOURCES} ../include/fixed_inventory.h)

set_target_properties(maglib_inventory_static PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/debug)
set_target_properties(maglib_inventory_static PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/release)

set_target_properties(maglib_inventory PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/debug)
set_target_properties(maglib_inventory PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/release)

target_include_directories(maglib_inventory PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(maglib_inventory_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
