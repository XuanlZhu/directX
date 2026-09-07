//
// Created by admin on 2026/9/7.
//

#include "Camera.h"

DirectX::XMMATRIX Camera::GetViewMatrix() {
    using namespace DirectX;

    XMVECTOR pos = XMLoadFloat3(&position);
    XMVECTOR dir = XMLoadFloat3(&facing);

    XMVECTOR target = XMVectorAdd(pos, dir);

    XMVECTOR up = XMVectorSet(
        0.0f,
        1.0f,
        0.0f,
        0.0f
    );

    return XMMatrixLookAtLH(
        pos,
        target,
        up
    );
}
