//
// Created by admin on 2026/9/10.
//

#include "Entity_pillar.h"

#include "Global.h"
#include "Core/Camera.h"
#include "Mesh/Mesh_pillar.h"

Entity_pillar::Entity_pillar() {
    mesh = Mesh_pillar();
    meshFbx = CModel("FBX/SCP-096.fbx");
    scale= {100, 100, 100};
    rotation = {-90, 0, 90};
}

void Entity_pillar::Update(float deltaTime) {

}

void Entity_pillar::Draw() {
    auto pos = WorldToScreen(position);
    Global::graphic->DrawText2("zhuzi", pos.x, pos.y);

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
        meshFbx.m_indices
    );

    // 绘制立方体
    // Global::graphic->DrawPrimitive3D(
    //     D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
    //     mesh.vertices,
    //     std::size(mesh.vertices),
    //     sizeof(Vertex3)
    // );
}
