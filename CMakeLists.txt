cmake_minimum_required(VERSION 3.3)
project(maglib_inventory)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(HEADERS
        ${PROJECT_SOURCE_DIR}/include/ItemLayouts.h
        ${PROJECT_SOURCE_DIR}/include/managed_pointer.h
        ${PROJECT_SOURCE_DIR}/include/inv_cell.h
        ${PROJECT_SOURCE_DIR}/include/fixed_size_inv.h
        ${PROJECT_SOURCE_DIR}/include/has_method.h
        ${PROJECT_SOURCE_DIR}/include/dynamic_inv.h
        ${PROJECT_SOURCE_DIR}/include/t_iterator.h
)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG -DDEBUG=0")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG=1")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weffc++ -Wall -Werror -pedantic")

set(CMAKE_CXX_STANDARD 14)

#add_subdirectory(lib)
add_subdirectory(test)

find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
        ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
        )
endif(DOXYGEN_FOUND)