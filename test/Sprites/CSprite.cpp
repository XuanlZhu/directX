//
// Created by admin on 2026/9/2.
//

#include "CSprite.h"

#include <iostream>

#include "../Global.h"
#include "../Core/CImageManager.h"
#include "../Core/Graphic.h"

CSprite::CSprite(){
    // mImage = Global::imageManager->GetImage(pngName);

}
void CSprite::OnCreated() {
    mVertexs[0] = XMFLOAT2(mPos.x,mPos.y);
    mVertexs[1] = XMFLOAT2(mPos.x+ mLength,mPos.y);
    mVertexs[2] = XMFLOAT2(mPos.x,mPos.y+ mLength);
    mVertexs[3] = XMFLOAT2(mPos.x + mLength,mPos.y+ mLength);
}


void CSprite::Draw()
{

    // std::cout << pngName<< mPos.x <<mPos.y <<angleDeg << std::endl;
    DrawTexture(pngName,mPos.x,mPos.y,50,50,mPos.x,mPos.y,angleDeg);
    DrawLine(mVertexs[0], mVertexs[1], XMFLOAT3(1,1,1));
    DrawLine(mVertexs[0], mVertexs[2], XMFLOAT3(1,1,1));
    DrawLine(mVertexs[3], mVertexs[1], XMFLOAT3(1,1,1));
    DrawLine(mVertexs[3], mVertexs[2], XMFLOAT3(1,1,1));

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

