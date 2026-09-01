cmake_minimum_required(VERSION 4.2)
project(base)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(test)

add_executable(base WIN32
        test/common.cpp
        test/common.h
        test/Graphic.cpp
        test/Graphic.h
        test/main.cpp
        test/myHead.h
        test/Singleton.h)

target_include_directories(base PRIVATE
        ${CMAKE_SOURCE_DIR}/thirdparty/DX/include
)

target_link_directories(base PRIVATE
        ${CMAKE_SOURCE_DIR}/thirdparty/DX/lib
)

target_link_libraries(base PRIVATE
        d3d9
        d3dx9
)