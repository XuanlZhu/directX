//
// Created by admin on 2026/9/8.
//

#pragma once
#include <DirectXMath.h>
#include <vector>

#include "Core/CVector2.h"
#include "Core/Graphic.h"
using namespace DirectX;

class Mesh
{
public:
    Mesh();
    virtual void Draw();// 绘制
    virtual void Update(float deltaTime);
    XMMATRIX GetWorldMatrix();
    virtual void Rotate(float x,float y,float z);//欧拉旋转
    XMFLOAT3 Getfacing();
    void Move(bool forward);

    std::vector<Vertex3> vertices;
    XMFLOAT3 position = {0, 0, 0};
    XMFLOAT3 rotation = {0, 0, 0};
    XMFLOAT3 scale    = {1, 1, 1};
private:
    XMFLOAT3 facing    = {1, 0, 0};
};

// class Mesh
// {
// public:
//     virtual void Draw();// 绘制
//     virtual void Update(float deltaTime);
//
//
//     float mSpeed = 100;//速度
//     float mChangeX=0;//偏移
//     float mChangeY=0;
//
//     CVector2 mLookat = CVector2(1,0);
//
//     XMFLOAT2 XAnchor{0,0};//锚点
//     XMFLOAT2 XScale{1,1};//缩放
//     XMFLOAT2 XPosition{0,0};//世界坐标
//     XMFLOAT2 XOriginLocalVertices[4];//原始局部顶点，没有经过锚点偏移
//     XMFLOAT2 XLocalVertices[4];//局部顶点
//     XMFLOAT2 XWorldVertices[4];//世界顶点
//     XMMATRIX GetWorldMatrix();
//     void GetWorldVertices();
// };
