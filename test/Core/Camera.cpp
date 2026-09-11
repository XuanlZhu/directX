//
// Created by admin on 2026/9/7.
//

#include "Camera.h"

#include <algorithm>
#include <iostream>

#include "Global.h"
#include "Graphic.h"
using namespace DirectX;
#include "DirectXMath.h"

Camera::Camera() {
    facing = DirectX::XMFLOAT3( 1, 0,  0);
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

void Camera::OnMouseMoveLeft(int x, int y, bool isLdown) {
}

void Camera::SetPosition(XMFLOAT3 pos) {
    position = pos;
}

void Camera::DrawFrustum()
{
    std::cout << "画视锥体" << std::endl;
    VerticesFrustum.clear();

    const float fovY = XMConvertToRadians(60.0f);
    const float aspect = 800.0f / 600.0f;

    const float nearZ = 0.1f;
    const float farZ = 100.0f;

    // 计算近平面、远平面的宽高
    const float nearHeight = 2.0f * tanf(fovY * 0.5f) * nearZ;
    const float nearWidth  = nearHeight * aspect;

    const float farHeight = 2.0f * tanf(fovY * 0.5f) * farZ;
    const float farWidth  = farHeight * aspect;

    XMVECTOR pos = XMLoadFloat3(&this->position);

    XMVECTOR forward = XMVector3Normalize(
        XMLoadFloat3(&facing)
    );

    XMVECTOR up = XMVector3Normalize(
        XMLoadFloat3(&uping)
    );

    // 左手坐标系：
    // right = up × forward
    XMVECTOR right = XMVector3Normalize(
        XMVector3Cross(up, forward)
    );

    // 重新计算真正正交的 up
    up = XMVector3Normalize(
        XMVector3Cross(forward, right)
    );

    // 近平面中心
    XMVECTOR nearCenter =
        pos + forward * nearZ;

    // 远平面中心
    XMVECTOR farCenter =
        pos + forward * farZ;

    const float nearHalfWidth = nearWidth * 0.5f;
    const float nearHalfHeight = nearHeight * 0.5f;

    const float farHalfWidth = farWidth * 0.5f;
    const float farHalfHeight = farHeight * 0.5f;

    // =========================
    // 近平面四个角
    // =========================

    XMVECTOR nearLT =
        nearCenter
        + up * nearHalfHeight
        - right * nearHalfWidth;

    XMVECTOR nearRT =
        nearCenter
        + up * nearHalfHeight
        + right * nearHalfWidth;

    XMVECTOR nearLB =
        nearCenter
        - up * nearHalfHeight
        - right * nearHalfWidth;

    XMVECTOR nearRB =
        nearCenter
        - up * nearHalfHeight
        + right * nearHalfWidth;

    // =========================
    // 远平面四个角
    // =========================

    XMVECTOR farLT =
        farCenter
        + up * farHalfHeight
        - right * farHalfWidth;

    XMVECTOR farRT =
        farCenter
        + up * farHalfHeight
        + right * farHalfWidth;

    XMVECTOR farLB =
        farCenter
        - up * farHalfHeight
        - right * farHalfWidth;

    XMVECTOR farRB =
        farCenter
        - up * farHalfHeight
        + right * farHalfWidth;

    // =========================
    // 添加一条线
    // =========================

    auto AddLine =
        [this](XMVECTOR _a, XMVECTOR _b)
    {
        Vertex3 a;
        Vertex3 b;

        XMStoreFloat3(&a.position, _a);
        XMStoreFloat3(&b.position, _b);

        a.color = { 1.0f, 0.0f, 0.0f };
        b.color = { 1.0f, 0.0f, 0.0f };

        VerticesFrustum.push_back(a);
        VerticesFrustum.push_back(b);
    };

    // =========================
    // 近平面
    // =========================

    AddLine(nearLT, nearRT);
    AddLine(nearRT, nearRB);
    AddLine(nearRB, nearLB);
    AddLine(nearLB, nearLT);

    // =========================
    // 远平面
    // =========================

    AddLine(farLT, farRT);
    AddLine(farRT, farRB);
    AddLine(farRB, farLB);
    AddLine(farLB, farLT);

    // =========================
    // 连接近平面和远平面
    // =========================

    AddLine(nearLT, farLT);
    AddLine(nearRT, farRT);
    AddLine(nearLB, farLB);
    AddLine(nearRB, farRB);


}

void Camera::Draw() {
    // 矩阵数据
    MatrixBuffer matrixData;
    matrixData.world = XMMatrixIdentity();
    matrixData.view =  XMMatrixTranspose(Global::camera->GetViewMatrix());
    matrixData.projection = XMMatrixTranspose(Global::graphic->m_projection);

    // 把矩阵传给 GPU
    Global::graphic->m_context->UpdateSubresource(
        Global::graphic->m_matrixBuffer,
        0,
        nullptr,
        &matrixData,
        0,
        0
    );
    // 绘制立方体
    Global::graphic->DrawPrimitive3D(
        D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
        VerticesFrustum,
        VerticesFrustum.size(),
        sizeof(Vertex3)
    );
}

