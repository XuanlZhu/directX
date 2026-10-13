//
// Created by admin on 2026/10/12.
//

#include "Entity_cylinder.h"
#include <iostream>

#include "Global.h"
#include "Core/Camera.h"
#include "Mesh/Mesh_pillar.h"
#undef min
#undef max

Entity_cylinder::Entity_cylinder() {
    meshFbx = CModel("FBX/Stone_Pillar_New_LP.fbx");
    scale= {0.1, 0.1, 0.1};
    rotation = {-90, 0, 0};
}

void Entity_cylinder::Update(float deltaTime) {
}

void Entity_cylinder::Draw() {
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
