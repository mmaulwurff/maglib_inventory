set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build/")

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(maglib_inventory_test main.cpp ${HEADERS})

set_target_properties(maglib_inventory_test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/debug)
set_target_properties(maglib_inventory_test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/release)

target_link_libraries(maglib_inventory_test LINK_PUBLIC maglib_inventory)
