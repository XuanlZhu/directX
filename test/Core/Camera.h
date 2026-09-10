//
// Created by admin on 2026/9/7.
//

#pragma once
#include <DirectXMath.h>
using namespace DirectX;

class Camera
{
public:
    Camera();
    XMMATRIX GetViewMatrix();
    virtual void Update(float deltaTime);
    virtual void OnMouseMove(int x, int y,bool isRdown);
    virtual void OnMouseMoveLeft(int x, int y,bool isLdown);
    void SetPosition(XMFLOAT3 pos);



    XMFLOAT3 position = { 0.5, 0.0f, -5.0f };
    XMFLOAT3 facing = { 0.0f, 0.0f, 1 };
    XMFLOAT3 uping = { 0, 1, 0 };

    float roll = 0;
    float yaw = 0.0f;
    float pitch = 0.0f;
    float mChangeX = 0.0f;
    float mChangeY = 0.0f;
    float mSpeed = 1;
    float mYaw = 0.0f;
    float mPitch = 0.0f;
};
