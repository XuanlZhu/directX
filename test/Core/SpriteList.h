//
// Created by admin on 2026/9/2.
//

#pragma once
#include <memory>
#include <vector>

#include "../Sprites/CSprite.h"
using namespace std;

class CCamera;

class SpriteList {
public:

    void Draw();//绘制
    void Append(std::shared_ptr<CSprite> _sprite);//添加精灵
    void Update(float deltaTime);//更新
    void ClearList();//清理表
    std::shared_ptr<CSprite> GetSharedPtr(CSprite* unit);
    // vector<CSprite*> mSprites;
    std::vector<std::shared_ptr<CSprite>> mSprites;
};
