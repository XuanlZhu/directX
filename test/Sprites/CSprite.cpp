//
// Created by admin on 2026/9/2.
//

#include "CSprite.h"
#undef min
#undef max
#include <algorithm>
#include <iostream>

#include "../Global.h"
#include "../Core/CImageManager.h"
#include "../Core/Graphic.h"
#include "Core/SpriteList.h"

CSprite::CSprite(){
    // mImage = Global::imageManager->GetImage(pngName);

}
void CSprite::OnCreated() {
    XLocalVertices[0] = XMFLOAT2(0,0);
    XLocalVertices[1] = XMFLOAT2(mWidth,0);
    XLocalVertices[2] = XMFLOAT2(0,mHeight);
    XLocalVertices[3] = XMFLOAT2(mWidth,mHeight);
}

void CSprite::CollisionDetection() {
    //获取其他精灵
    std::vector<CSprite*> sprites;
    for (auto& sprite : Global::spriteList->mSprites)
    {
        if (sprite.get()!=this) {sprites.push_back(sprite.get());}
    }
    // 按距离排序（近 -> 远）
    std::sort(sprites.begin(),sprites.end(),[this](CSprite* a, CSprite* b){
        float da = (a->mPos-mPos).Length();float db = (b->mPos-mPos).Length();
        return da < db;
    });
    //包围盒检测
    if (BoxDetection(sprites[0]))
    {
        // std::cout << "包围盒碰撞" << std::endl;
        if (OBBDetection(sprites[0])) {
            // std::cout << "OBB碰撞" << std::endl;
            if (PixelDetection(sprites[0])) {
                std::cout << "像素碰撞" << std::endl;
            }
        }
    }
}

bool CSprite::BoxDetection(CSprite* other) {
    //包围盒1
    float minX1 = XBoxVertices[0].x;float maxX1 = XBoxVertices[1].x;float minY1 = XBoxVertices[0].y;float maxY1 = XBoxVertices[2].y;
    // 对方包围盒
    float minX2 = other->XBoxVertices[0].x;float maxX2 = other->XBoxVertices[1].x;float minY2 = other->XBoxVertices[0].y;float maxY2 = other->XBoxVertices[2].y;
    // AABB碰撞检测
    if(maxX1 < minX2 ||minX1 > maxX2 ||maxY1 < minY2 ||minY1 > maxY2){return false;}
    return true;
}

bool CSprite::OBBDetection(CSprite* other) {
    // 两个矩形需要检测的轴
    std::vector<XMFLOAT2> axes;
    // 获取A的边
    XMFLOAT2 edgesA[2];
    edgesA[0] = {
        XWorldVertices[1].x - XWorldVertices[0].x,
        XWorldVertices[1].y - XWorldVertices[0].y
    };
    edgesA[1] = {
        XWorldVertices[2].x - XWorldVertices[0].x,
        XWorldVertices[2].y - XWorldVertices[0].y
    };
    // 获取B的边
    XMFLOAT2 edgesB[2];
    edgesB[0] = {
        other->XWorldVertices[1].x - other->XWorldVertices[0].x,
        other->XWorldVertices[1].y - other->XWorldVertices[0].y
    };
    edgesB[1] = {
        other->XWorldVertices[2].x - other->XWorldVertices[0].x,
        other->XWorldVertices[2].y - other->XWorldVertices[0].y
    };
    // 法线作为检测轴
    axes.push_back({-edgesA[0].y, edgesA[0].x});
    axes.push_back({-edgesA[1].y, edgesA[1].x});
    axes.push_back({-edgesB[0].y, edgesB[0].x});
    axes.push_back({-edgesB[1].y, edgesB[1].x});
    // 检测4个轴
    for(auto axis : axes)
    {
        float minA = FLT_MAX;
        float maxA = -FLT_MAX;
        float minB = FLT_MAX;
        float maxB = -FLT_MAX;
        // A投影
        for(int i=0;i<4;i++)
        {
            float projection =XWorldVertices[i].x * axis.x +XWorldVertices[i].y * axis.y;//求点积
            minA = std::min(minA, projection);
            maxA = std::max(maxA, projection);
        }
        // B投影
        for(int i=0;i<4;i++)
        {
            float projection =other->XWorldVertices[i].x * axis.x +other->XWorldVertices[i].y * axis.y;
            minB = std::min(minB, projection);
            maxB = std::max(maxB, projection);
        }
        // 没有重叠
        if(maxA < minB || maxB < minA)
        {
            return false;
        }
    }
    return true;
}

bool CSprite::PixelDetection(CSprite* other) {
    CSprite* B = this;CSprite* A = other;
    // std::cout << "开始像素碰撞" << std::endl;

    XMMATRIX worldA = A->GetWorldMatrix();
    XMMATRIX worldB = B->GetWorldMatrix();
    auto isInside = [&](int x,int y)
    {
        XMFLOAT2 posB;//世界坐标
        XMVECTOR local = XMVectorSet(x,y,0,1);//创建局部坐标
        XMVECTOR worldPos = XMVector3Transform(local,worldB);//计算世界坐标，向量*矩阵
        XMStoreFloat2(&posB,worldPos);//转回Float2
        // 判断点是否在A的OBB内部
        for(int i = 0; i < 4; i++)
        {
            XMFLOAT2 a = A->XWorldVertices[i];XMFLOAT2 b = A->XWorldVertices[(i+1)%4];
            // 边向量
            float edgeX = b.x - a.x;float edgeY = b.y - a.y;
            // 点到边
            float pointX = posB.x - a.x;float pointY = posB.y - a.y;
            // 2D叉乘
            float cross =edgeX * pointY -edgeY * pointX;
            if(cross < 0){
                return false;
            }
        }
        return true;
    };
    //对于B的每个像素点
    for(int i=0;i<B->mWidth;i++) {
        for(int j=0;j<B->mHeight;j++) {
            // std::cout << "B->mWidth"<<B->mWidth << std::endl;
            std::cout << "遍历像素"<<i <<","<<j << std::endl;
            //对于B的每个像素，如果在A内部
            if (isInside(i,j)) {
                std::cout << "在A内部"<<i <<","<<j << std::endl;
                float alphaB =  Global::imageManager->GetImageAalpha(B->pngName,i,j);//获取B的alpha值
                XMFLOAT2 posB;//世界坐标
                XMVECTOR local = XMVectorSet(i,j,0,1);//创建局部坐标
                XMVECTOR worldPos = XMVector3Transform(local,worldB);//计算世界坐标，向量*矩阵
                XMStoreFloat2(&posB,worldPos);//转回Float2

                XMFLOAT2 posA;//世界坐标
                XMVECTOR localA = XMVectorSet(posB.x,posB.y,0,1);//创建局部坐标
                XMVECTOR worldPosA = XMVector3Transform(localA,XMMatrixInverse(nullptr, worldA));//使用逆矩阵获取局部坐标
                XMStoreFloat2(&posA,worldPosA);//转回Float2

                float alphaA =  Global::imageManager->GetImageAalpha(A->pngName,posA.x,posA.y);//获取A的alpha值

                std::cout << "alphaA"<<alphaA << std::endl;
                std::cout << "alphaA"<<alphaB << std::endl;
                if(alphaA>0 && alphaB>0) {
                    return true;
                }
            }
        }
    }

    return false;
}

void CSprite::Update(float deltaTime) {
    mPos = mPos + CVector2(mChangeX,mChangeY).Normalize()*100*deltaTime;//移动
    GetWorldVertices();//计算世界顶点
    GetBoxVertices();//计算包围盒顶点
    if(this==Global::claw) {
        CollisionDetection();//碰撞检测
    }
}

void CSprite::Draw()
{
    //矩形框
    DrawLine(XWorldVertices[0], XWorldVertices[1], XMFLOAT3(1,1,1));
    DrawLine(XWorldVertices[0], XWorldVertices[2], XMFLOAT3(1,1,1));
    DrawLine(XWorldVertices[3], XWorldVertices[1], XMFLOAT3(1,1,1));
    DrawLine(XWorldVertices[3], XWorldVertices[2], XMFLOAT3(1,1,1));
    //绘制图片
    DrawTexture(pngName,mPos.x,mPos.y,mWidth,mHeight,mPos.x,mPos.y,angleDeg);
    //包围盒
    DrawLine(XBoxVertices[0],XBoxVertices[1],XMFLOAT3(0,0,1));
    DrawLine(XBoxVertices[0],XBoxVertices[2],XMFLOAT3(0,0,1));
    DrawLine(XBoxVertices[1],XBoxVertices[3],XMFLOAT3(0,0,1));
    DrawLine(XBoxVertices[2],XBoxVertices[3],XMFLOAT3(0,0,1));

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

void CSprite::Destroy() {
    isdraw = false;
}

// void CSprite::Draw()
// {
//     //计算旋转后顶点
//     mVertexs[0] = CVector2(mPos.x,mPos.y).Rotate(mPos,angleDeg);
//     mVertexs[1] = CVector2(mPos.x+ mWidth,mPos.y).Rotate(mPos,angleDeg);
//     mVertexs[2] = CVector2(mPos.x,mPos.y+ mHeight).Rotate(mPos,angleDeg);
//     mVertexs[3] = CVector2(mPos.x + mWidth,mPos.y+ mHeight).Rotate(mPos,angleDeg);
//
//     //绘制图片
//     DrawTexture(pngName,mPos.x,mPos.y,mWidth,mHeight,mPos.x,mPos.y,angleDeg);
//     DrawLine(mVertexs[0], mVertexs[1], XMFLOAT3(1,1,1));
//     DrawLine(mVertexs[0], mVertexs[2], XMFLOAT3(1,1,1));
//     DrawLine(mVertexs[3], mVertexs[1], XMFLOAT3(1,1,1));
//     DrawLine(mVertexs[3], mVertexs[2], XMFLOAT3(1,1,1));
//
//     //计算盒子顶点
//     float minX = mVertexs[0].x;float maxX = mVertexs[0].x;float minY = mVertexs[0].y;float maxY = mVertexs[0].y;
//     for(int i=1;i<4;i++)
//     {
//         minX = min(minX, mVertexs[i].x);
//         maxX = max(maxX, mVertexs[i].x);
//         minY = min(minY, mVertexs[i].y);
//         maxY = max(maxY, mVertexs[i].y);
//     }
//     CVector2 box[4];
//     box[0] = CVector2(minX,minY); // 左上
//     box[1] = CVector2(maxX,minY); // 右上
//     box[2] = CVector2(minX,maxY); // 左下
//     box[3] = CVector2(maxX,maxY); // 右下
//     DrawLine(box[0],box[1],XMFLOAT3(0,0,1));
//     DrawLine(box[0],box[2],XMFLOAT3(0,0,1));
//     DrawLine(box[1],box[3],XMFLOAT3(0,0,1));
//     DrawLine(box[2],box[3],XMFLOAT3(0,0,1));
//
// }

