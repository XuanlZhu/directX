//
// Created by admin on 2026/9/21.
//

#include "Entity_player.h"

#include "Global.h"
#include "Core/Camera.h"


Entity_player::Entity_player() {
    meshFbx = CModel("FBX/beachBall.fbx");
    m_texture = Global::graphic->LoadFBXTexture(L"FBX/internal_ground_ao_texture.jpeg");

    scale= {1, 1, 1};
    rotation = {-90, 45, 90};

}
void Entity_player::Update(float deltaTime) {
    XMFLOAT3 dir = {mChangeForward, 0, mChangeLeft};

    XMVECTOR pos = XMLoadFloat3(&position);
    XMVECTOR direction = XMVector3Normalize(XMLoadFloat3(&dir));//归一化方向

    pos += direction * 0.1;
    XMStoreFloat3(&position, pos);
}

void Entity_player::Draw() {
    // 矩阵数据
    MatrixBuffer matrixData;
    matrixData.world = XMMatrixTranspose(GetWorldMatrix());
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
    Global::graphic->DrawPrimitiveIndexed(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
        meshFbx.m_vertices,
        meshFbx.m_indices,
        m_texture
    );
}