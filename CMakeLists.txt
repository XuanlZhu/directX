cmake_minimum_required(VERSION 4.2)
project(base)
add_compile_options(/utf-8)

add_definitions(
    -D_WIN32_WINNT=0x0601
    -DNTDDI_VERSION=0x06010000
)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(test)

# 加入 DirectXTK
add_subdirectory(DirectXTK)

add_executable(base WIN32
        test/Core/Graphic.cpp
        test/Core/Graphic.h
        test/main.cpp
        test/Global.cpp
        test/Global.h
        test/Core/Game.cpp
        test/Core/Game.h
        test/Core/SpriteList.cpp
        test/Core/SpriteList.h
        test/Core/CSprite.cpp
        test/Core/CSprite.h
        test/Core/CImageManager.cpp
        test/Core/CImageManager.h
        test/Core/UIManager.cpp
        test/Core/UIManager.h
)


target_link_libraries(base PRIVATE
        d3d11
        dxgi
        d3dcompiler

        DirectXTK
)