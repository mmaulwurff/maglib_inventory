cmake_minimum_required(VERSION 3.3)
project(maglib_inventory)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(HEADERS ${PROJECT_SOURCE_DIR}/include/DirectInventoryItem.h
            ${PROJECT_SOURCE_DIR}/include/ItemLayouts.h
            ${PROJECT_SOURCE_DIR}/include/InventoryItemPointerAdapter.h
            ${PROJECT_SOURCE_DIR}/include/PointerInventoryItem.h
            ${PROJECT_SOURCE_DIR}/include/InventoryItemInterface.h
)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG -DDEBUG=0")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG=1")

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(lib)
add_subdirectory(test)
