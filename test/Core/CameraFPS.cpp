//
// Created by admin on 2026/9/10.
//

#include "CameraFPS.h"

#include "Global.h"
#include "Entity/Entity.h"
using namespace DirectX;

void CameraFPS::Update(float deltaTime) {
    XMVECTOR va = XMLoadFloat3(&Global::player->GetPosition());
    XMVECTOR vb = XMVectorSet(0,1,0,1);

    XMVECTOR vc = va + vb;

    XMFLOAT3 c;
    XMStoreFloat3(&c, vc);
    SetPosition(c);
}

void CameraFPS::OnMouseMove(int x, int y, bool isRdown) {
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
    XMVECTOR dir = XMVector3Normalize(XMLoadFloat3(&facing));

    // 世界 Y 轴
    XMVECTOR worldUp = XMVector3Normalize(XMLoadFloat3(&uping));

    // =========================
    // 1. 左右旋转：绕世界 Y 轴
    // =========================

    XMMATRIX yawMatrix = XMMatrixRotationAxis(worldUp,yaw);

    dir = XMVector3TransformNormal(dir,yawMatrix);
    dir = XMVector3Normalize(dir);
    //人物也要旋转
    float yawDegree = yaw * 180.0f / XM_PI;
    Global::player->rotation.y += yawDegree;
    XMStoreFloat3(&Global::player->facing,dir);


    // =========================
    // 2. 计算当前摄像机的右方向
    // =========================

    XMVECTOR right = XMVector3Normalize(
        XMVector3Cross(worldUp, dir)
    );

    // =========================
    // 3. 上下旋转：绕摄像机 Right 轴
    // =========================

    XMMATRIX pitchMatrix = XMMatrixRotationAxis(right,pitch);

    dir = XMVector3TransformNormal(dir,pitchMatrix);

    dir = XMVector3Normalize(dir);

    // 保存新的朝向
    XMStoreFloat3(&facing,dir);
    //设置人物朝向
}
