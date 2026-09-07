//
// Created by admin on 2026/9/7.
//

#include "Camera.h"

#include <algorithm>
using namespace DirectX;
#include "DirectXMath.h"

Camera::Camera() {
    facing = DirectX::XMFLOAT3( 0.707, 0,  0.707);
    position = DirectX::XMFLOAT3{ -2, 2, -5.0f };
}

DirectX::XMMATRIX Camera::GetViewMatrix() {
    using namespace DirectX;

    XMVECTOR pos = XMLoadFloat3(&position);
    XMVECTOR dir = XMLoadFloat3(&facing);

    XMVECTOR target = XMVectorAdd(pos, dir);

    XMVECTOR up = XMLoadFloat3(&uping);

    return XMMatrixLookAtLH(
        pos,
        target,
        up
    );
}

void Camera::Update(float deltaTime) {
    XMVECTOR face = DirectX::XMLoadFloat3(&facing);
    XMVECTOR up     = DirectX::XMLoadFloat3(&uping);

    XMVECTOR left = DirectX::XMVector3Cross(up, face);
    // 归一化
    left = DirectX::XMVector3Normalize(left);
    face = DirectX::XMVector3Normalize(face);

    XMVECTOR pos = DirectX::XMLoadFloat3(&position);

    // 前后移动
    pos += face * mChangeY * deltaTime * mSpeed;
    // 左右移动
    pos += left * mChangeX * deltaTime * mSpeed;
    // 写回 position
    DirectX::XMStoreFloat3(&position, pos);
}

void Camera::OnMouseMove(int x, int y,bool isRdown){
    static int lastX = x;
    static int lastY = y;

    int dx = x - lastX;
    int dy = y - lastY;

    lastX = x;
    lastY = y;

    if (!isRdown)return;

    float sensitivity = 0.002f;

    float yaw = dx * sensitivity;
    float pitch = dy * sensitivity;

    using namespace DirectX;

    // 当前朝向
    XMVECTOR dir = XMVector3Normalize(
        XMLoadFloat3(&facing)
    );

    // 世界 Y 轴
    XMVECTOR worldUp = XMVector3Normalize(
        XMLoadFloat3(&uping)
    );

    // =========================
    // 1. 左右旋转：绕世界 Y 轴
    // =========================

    XMMATRIX yawMatrix = XMMatrixRotationAxis(
        worldUp,
        yaw
    );

    dir = XMVector3TransformNormal(
        dir,
        yawMatrix
    );

    dir = XMVector3Normalize(dir);

    // =========================
    // 2. 计算当前摄像机的右方向
    // =========================

    XMVECTOR right = XMVector3Normalize(
        XMVector3Cross(worldUp, dir)
    );

    // =========================
    // 3. 上下旋转：绕摄像机 Right 轴
    // =========================

    XMMATRIX pitchMatrix = XMMatrixRotationAxis(
        right,
        pitch
    );

    dir = XMVector3TransformNormal(
        dir,
        pitchMatrix
    );

    dir = XMVector3Normalize(dir);

    // 保存新的朝向
    XMStoreFloat3(
        &facing,
        dir
    );
}
