//
// Created by admin on 2026/9/7.
//

#pragma once
#include <DirectXMath.h>


class Camera
{
public:
    Camera();
    DirectX::XMFLOAT3 position = { 0.5, 0.0f, -5.0f };
    DirectX::XMFLOAT3 facing = { 0.0f, 0.0f, 1 };
    DirectX::XMFLOAT3 uping = { 0, 1, 0 };

    float roll = 0;
    float yaw = 0.0f;
    float pitch = 0.0f;
    float mChangeX = 0.0f;
    float mChangeY = 0.0f;
    float mSpeed = 1;
    float mYaw = 0.0f;
    float mPitch = 0.0f;

    DirectX::XMMATRIX GetViewMatrix();
    void Update(float deltaTime);
    void OnMouseMove(int x, int y,bool isRdown);
};
