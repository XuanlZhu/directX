//
// Created by admin on 2026/9/2.
//
#pragma once
#include "SpriteList.h"
#include <iostream>

void SpriteList::Update(float _deltaTime) {

    auto sprites = mSprites;
    for(auto sprite : sprites)
    {
        if(sprite)
        {
            sprite->Update(_deltaTime);
        }
    }
    ClearList();//清理
}

void SpriteList::ClearList() {
    mSprites.erase(
        std::remove_if(mSprites.begin(),mSprites.end(),
            [](auto _sprite)
            {
                return !_sprite->isdraw;
            }
        ),
        mSprites.end()
    );
}

std::shared_ptr<CSprite> SpriteList::GetSharedPtr(CSprite* _unit) {
    for (auto& unit : mSprites)
    {
        if (unit.get() == _unit)
        {
            return unit;
        }
    }
    return nullptr;
}

void SpriteList::Draw()
{
    std::cout << "SpriteList::Draw" << std::endl;
    //裁切
    for(auto sprite : mSprites)
    {
        sprite->Draw();
    }
    std::cout << "SpriteList::Draw结束" << std::endl;
}

void SpriteList::Append(std::shared_ptr<CSprite> _sprite) {
    mSprites.push_back(_sprite);
}