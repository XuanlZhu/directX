//
// Created by admin on 2026/9/2.
//

#include "Sprite_gold.h"

Sprite_gold::Sprite_gold() {
    pngName = "gold3";
    // pngName = "claw";
    mWidth = 50;
    mHeight = 50;

    angleDeg = RandomInt(0,360);
    float oWidth = 21;//原始宽高
    float oHeight = 19;//原始宽高
}

void Sprite_gold::Crawled() {

};
