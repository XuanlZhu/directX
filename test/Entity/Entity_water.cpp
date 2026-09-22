//
// Created by admin on 2026/9/20.
//

#include "Entity_water.h"
#include <iostream>

#include "DirectXTex.h"
#include "Global.h"
#include "Core/Camera.h"
#include "Mesh/Mesh_pillar.h"
#undef min
#undef max

Entity_water::Entity_water() {
    meshFbx = CModel();
    m_texture = Global::graphic->LoadFBXTexture(L"PNG/water.png");
    scale= {0.3, 0.3, 0.3};
    plane1.normal = XMFLOAT3{0,1,0};
    plane1.d = 0;

    const int size = 50;       // 20 × 20 个格子
    const float length = 50;
    const float step = length / size;

    meshFbx.m_vertices.clear();
    meshFbx.m_indices.clear();

    // =========================
    // 创建 21 × 21 个顶点
    // =========================
    for (int z = 0; z <= size; ++z)
    {
        for (int x = 0; x <= size; ++x)
        {
            float px = -length / 2.0f + x * step;
            float pz = -length / 2.0f + z * step;

            float u = static_cast<float>(x) / size;
            float v = static_cast<float>(z) / size;

            meshFbx.m_vertices.push_back(
            {
                { px, 0.0f, pz },   // position
                { 1.0f, 1.0f, 1.0f }, // color
        { u, v }
            });
        }
    }

    // =========================
    // 创建三角形索引
    // =========================
    for (int z = 0; z < size; ++z)
    {
        for (int x = 0; x < size; ++x)
        {
            int i0 = z * (size + 1) + x;
            int i1 = i0 + 1;
            int i2 = i0 + (size + 1);
            int i3 = i2 + 1;

            // 第一个三角形
            meshFbx.m_indices.push_back(i0);
            meshFbx.m_indices.push_back(i2);
            meshFbx.m_indices.push_back(i1);

            // 第二个三角形
            meshFbx.m_indices.push_back(i1);
            meshFbx.m_indices.push_back(i2);
            meshFbx.m_indices.push_back(i3);
        }
    }
}


void Entity_water::Update(float deltaTime) {

}


XMFLOAT3 ReflectPoint(Plane plane,XMFLOAT3 point)
{
    float distance = plane.Distance(point);

    XMFLOAT3 result;

    result.x = point.x - 2.0f * distance * plane.normal.x;
    result.y = point.y - 2.0f * distance * plane.normal.y;
    result.z = point.z - 2.0f * distance * plane.normal.z;

    return result;
}
XMFLOAT3 ReflectDirection(const Plane& plane, const XMFLOAT3& direction)
{
    float dot =
        direction.x * plane.normal.x +
        direction.y * plane.normal.y +
        direction.z * plane.normal.z;

    return {
        direction.x - 2.0f * dot * plane.normal.x,
        direction.y - 2.0f * dot * plane.normal.y,
        direction.z - 2.0f * dot * plane.normal.z
    };
}

void Entity_water::Draw() {
    //相机对称位置
    Global::camera2->position = ReflectPoint(plane1,Global::camera->position);
    Global::camera2->facing = ReflectDirection(plane1, Global::camera->facing);
    Global::camera2->uping = ReflectDirection(plane1, Global::camera->uping);

    // 矩阵数据
    MatrixBuffer matrixData;
    matrixData.world = XMMatrixTranspose(GetWorldMatrix());
    matrixData.view =  XMMatrixTranspose(Global::camera2->GetViewMatrix());//改为相机2
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
    //修改渲染视图
    // Global::graphic->m_context->OMSetRenderTargets(1,&Global::graphic->m_reflectionRTV,Global::graphic->m_depthStencilView);
    // for(auto& entity : Global::entityManager->mEntity) {
    //     if (entity.get()!=this) {
    //         entity->DrawReflection();
    //     }
    // }
    // //恢复渲染视图
    // Global::graphic->m_context->OMSetRenderTargets(1,&Global::graphic->m_renderTargetView,Global::graphic->m_depthStencilView);
    // //写入t1纹理槽
    // Global::graphic->m_context->PSSetShaderResources(
    //     1,      // 对应 t1
    //     1,
    //     &Global::graphic->m_reflectionSRV
    // );



    //-------------------------------------------
    // 矩阵数据
    // MatrixBuffer matrixData;
    // matrixData.world = XMMatrixTranspose(GetWorldMatrix());
    // matrixData.view =  XMMatrixTranspose(Global::camera->GetViewMatrix());
    // matrixData.projection = XMMatrixTranspose(Global::graphic->m_projection);
    //
    // // 把矩阵传给 GPU
    // Global::graphic->m_context->UpdateSubresource(
    //     Global::graphic->m_matrixBuffer,
    //     0,
    //     nullptr,
    //     &matrixData,
    //     0,
    //     0
    // );
    // // 绘制立方体
    // Global::graphic->DrawPrimitiveIndexed(
    //     D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
    //     meshFbx.m_vertices,
    //     meshFbx.m_indices,
    //     m_texture
    // );



    //渲染反射视图


    // 2. 水面写入 Stencil
    // DrawWaterToStencil();
    // 3. 使用镜像相机
    // DrawScene(camera2);


}
