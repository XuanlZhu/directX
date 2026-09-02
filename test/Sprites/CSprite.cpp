//
// Created by admin on 2026/9/2.
//

#include "CSprite.h"

#include "../Global.h"
#include "../Core/CImageManager.h"

CSprite::CSprite(){

}
CSprite::CSprite(std::string _path) {
    mImage = Global::imageManager->GetImage(_path);
}
CSprite::CSprite(std::string _path, float length) {
    mImage = Global::imageManager->GetImage(_path);
    mLength = length;
}


void CSprite::Draw()
{
    DrawTexture(pngName,0,0,100,100);
}

void CSprite::SetPosition(float _x, float _y) {
    mPos.x = _x;mPos.y = _y;
}

void CSprite::SetPosition(XMFLOAT2 _pos) {
    mPos.x = _pos.x;mPos.y = _pos.y;
}

XMFLOAT2 CSprite::GetPos(){
    return mPos;
}

void CSprite::Update(float _deltaTime) {
}

void CSprite::Destroy() {
    isdraw = false;
}