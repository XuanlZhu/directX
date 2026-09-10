//
// Created by admin on 2026/9/10.
//

#include "CameraTPS.h"

#include <algorithm>

#include "Global.h"
#include "Entity/Entity.h"

CameraTPS::CameraTPS() {
    dir = XMFLOAT3{-1, 0, 0};
}

void CameraTPS::Update(float deltaTime) {
    XMVECTOR va = XMLoadFloat3(&Global::player->GetPosition());
    XMVECTOR vb = XMVector3Normalize(XMLoadFloat3(&dir));

    XMVECTOR vc = va + vb*3.5;

    XMFLOAT3 c;
    XMStoreFloat3(&c, vc);
    SetPosition(c);//设置相机位置

    XMVECTOR dir2 = XMVector3Normalize(va - vc);
    XMStoreFloat3(&facing, dir2);//设置相机朝向
}


void CameraTPS::OnMouseMoveLeft(int x, int y, bool isLdown) {
    static int lastX = x;static int lastY = y;
    int dx = x - lastX;int dy = y - lastY;
    lastX = x;lastY = y;
    if (!isLdown)return;

    float sensitivity = 0.002f;

    mYaw   += dx * sensitivity;
    mPitch += dy * sensitivity;

    // 限制俯仰角，防止摄像机翻转
    mPitch = std::clamp(
        mPitch,
        -XM_PIDIV2 + 0.01f,
         XM_PIDIV2 - 0.01f
    );

    // 根据 yaw / pitch 计算自拍杆方向
    float cosPitch = cosf(mPitch);

    dir = {
        -cosPitch * cosf(-mYaw),
         sinf(mPitch),
        -cosPitch * sinf(-mYaw)
    };
}
