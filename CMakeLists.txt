cmake_minimum_required(VERSION 4.2)
project(base)
add_compile_options(/utf-8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(test)

# 加入 DirectXTK
add_subdirectory(DirectXTK)

add_executable(base WIN32
        test/Graphic.cpp
        test/Graphic.h
        test/main.cpp
        test/Global.cpp
        test/Global.h
)


target_link_libraries(base PRIVATE
        d3d11
        dxgi
        d3dcompiler

        DirectXTK
)