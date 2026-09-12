//
// Created by admin on 2026/9/10.
//

#pragma once
#include <DirectXMath.h>
#include <vector>

#include "../Mesh/Mesh.h"
#include "Core/Graphic.h"
#include "Mesh/CModel.h"
using namespace DirectX;

class Entity
{
public:
    Entity();
    virtual void Draw();// 绘制
    virtual void Update(float deltaTime);
    void SetPosition(XMFLOAT3 pos);
    XMFLOAT3 GetPosition();

    XMMATRIX GetWorldMatrix();
    virtual void Rotate(float x,float y,float z);//欧拉旋转
    XMFLOAT3 Getfacing();

    Mesh mesh;//网格体
    CModel meshFbx;//网格体
    float mChangeForward = 0;
    float mChangeLeft = 0;

    XMFLOAT3 position = {0, 0, 0};
    XMFLOAT3 rotation = {0, 0, 0};
    XMFLOAT3 scale    = {1, 1, 1};
    bool isdraw = false;
private:
    XMFLOAT3 facing    = {1, 0, 0};
};
