//
// Created by admin on 2026/9/2.
//

#pragma once
#include "CSprite.h"


class Sprite_claw : public CSprite
{
public:
    Sprite_claw() {
        pngName = "claw";
        mWidth = 32;
        mHeight = 19;
        mPosInit = CVector2(365,120);
        angleDeg = 45;
    };
};
