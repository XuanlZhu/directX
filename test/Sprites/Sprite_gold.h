//
// Created by admin on 2026/9/2.
//

#pragma once
#include "CSprite.h"


class Sprite_gold : public CSprite
{
public:
    Sprite_gold() {
        pngName = "gold3";
        mWidth = 50;
        mHeight = 50;
        angleDeg = RandomInt(0,360);
    };
};
