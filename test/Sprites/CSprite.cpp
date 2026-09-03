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
    XLocalVertices[0] = XMFLOAT2(0,0);
    XLocalVertices[1] = XMFLOAT2(mWidth,0);
    XLocalVertices[2] = XMFLOAT2(0,mHeight);
    XLocalVertices[3] = XMFLOAT2(mWidth,mHeight);
}


void CSprite::Draw()
{
    //计算旋转后顶点
    mVertexs[0] = CVector2(mPos.x,mPos.y).Rotate(mPos,angleDeg);
    mVertexs[1] = CVector2(mPos.x+ mWidth,mPos.y).Rotate(mPos,angleDeg);
    mVertexs[2] = CVector2(mPos.x,mPos.y+ mHeight).Rotate(mPos,angleDeg);
    mVertexs[3] = CVector2(mPos.x + mWidth,mPos.y+ mHeight).Rotate(mPos,angleDeg);

    //绘制图片
    DrawTexture(pngName,mPos.x,mPos.y,mWidth,mHeight,mPos.x,mPos.y,angleDeg);
    DrawLine(mVertexs[0], mVertexs[1], XMFLOAT3(1,1,1));
    DrawLine(mVertexs[0], mVertexs[2], XMFLOAT3(1,1,1));
    DrawLine(mVertexs[3], mVertexs[1], XMFLOAT3(1,1,1));
    DrawLine(mVertexs[3], mVertexs[2], XMFLOAT3(1,1,1));

    //计算盒子顶点
    float minX = mVertexs[0].x;float maxX = mVertexs[0].x;float minY = mVertexs[0].y;float maxY = mVertexs[0].y;
    for(int i=1;i<4;i++)
    {
        minX = min(minX, mVertexs[i].x);
        maxX = max(maxX, mVertexs[i].x);
        minY = min(minY, mVertexs[i].y);
        maxY = max(maxY, mVertexs[i].y);
    }
    CVector2 box[4];
    box[0] = CVector2(minX,minY); // 左上
    box[1] = CVector2(maxX,minY); // 右上
    box[2] = CVector2(minX,maxY); // 左下
    box[3] = CVector2(maxX,maxY); // 右下
    DrawLine(box[0],box[1],XMFLOAT3(0,0,1));
    DrawLine(box[0],box[2],XMFLOAT3(0,0,1));
    DrawLine(box[1],box[3],XMFLOAT3(0,0,1));
    DrawLine(box[2],box[3],XMFLOAT3(0,0,1));

}

void CSprite::SetPosition(float _x, float _y) {
    mPos.x = _x;mPos.y = _y;
}

void CSprite::SetPosition(XMFLOAT2 _pos) {
    mPos.x = _pos.x;mPos.y = _pos.y;
}

CVector2 CSprite::GetPos(){
    return mPos;
}

void CSprite::Update(float _deltaTime) {
    // mPos = mPos + CVector2(mChangeX,mChangeY).Normalize()*100*_deltaTime;
}

void CSprite::Destroy() {
    isdraw = false;
}

