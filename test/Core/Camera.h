//
// Created by admin on 2026/9/7.
//

#pragma once
#include <DirectXMath.h>


class Camera
{
public:
    DirectX::XMFLOAT3 position = { 0.5, 0.0f, -5.0f };
    DirectX::XMFLOAT3 facing = { 0.0f, 0.0f, 1 };

    float roll = 0;
    float yaw = 0.0f;
    float pitch = 0.0f;

    DirectX::XMMATRIX GetViewMatrix();
};
