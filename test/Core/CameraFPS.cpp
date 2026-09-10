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
