//
// Created by admin on 2026/9/1.
//

#include "Global.h"

#include <chrono>
#include "Core/Game.h"
#include "Core/SpriteList.h"

std::wstring StringToWString(std::string str) {
    int size_needed = MultiByteToWideChar(
        CP_UTF8,
        0,
        str.c_str(),
        -1,
        nullptr,
        0
    );

    std::wstring wstr(size_needed, 0);

    MultiByteToWideChar(
        CP_UTF8,
        0,
        str.c_str(),
        -1,
        wstr.data(),
        size_needed
    );

    return wstr;
}

float GetNowTime() {
    auto ts = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<float>(ts-Global::game->mStartTime).count();
}

std::weak_ptr<CSprite> CreateCSprite(std::string _sprite, XMFLOAT2 _pos) {
    std::shared_ptr<CSprite> sprite;;//创建精灵类，下发到当前精灵表

    sprite = std::make_shared<CSprite>();

    sprite->SetPosition(_pos);
    Global::spriteList->Append(sprite);
    return sprite;
}
