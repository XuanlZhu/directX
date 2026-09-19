//
// Created by admin on 2026/9/19.
//

#include "Entity_ball.h"
#include <iostream>

#include "Global.h"
#include "Core/Camera.h"
#include "Mesh/Mesh_pillar.h"
#undef min
#undef max

Entity_ball::Entity_ball() {
    mesh = Mesh_pillar();
    meshFbx = CModel("FBX/SCP-096.fbx");
    scale= {500, 500, 500};
    rotation = {-90, 45, 90};

}
void Entity_ball::Update(float deltaTime) {
}

void Entity_ball::Draw() {
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