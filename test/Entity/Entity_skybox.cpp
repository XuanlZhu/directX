//
// Created by admin on 2026/9/14.
//

#include "Entity_skybox.h"

#include "Global.h"
#include "Core/Camera.h"

Entity_skybox::Entity_skybox() {
    m_texture = Global::graphic->LoadFBXTexture(L"PNG/images.jpg");
    meshFbx = CModel();
    scale    = {50, 50, 50};
    meshFbx.m_vertices =
{
    // =========================
    // 前 z = -1
    // =========================
        {{-1.0f, -1.0f, -1.0f}, {1,1,1}, {0.25f, 2.0f/3.0f}},
        {{-1.0f,  1.0f, -1.0f}, {1,1,1}, {0.25f, 1.0f/3.0f}},
        {{ 1.0f,  1.0f, -1.0f}, {1,1,1}, {0.50f, 1.0f/3.0f}},
        {{ 1.0f, -1.0f, -1.0f}, {1,1,1}, {0.50f, 2.0f/3.0f}},

        // =========================
        // 后 z = +1
        // =========================
        {{ 1.0f, -1.0f,  1.0f}, {1,1,1}, {0.75f, 2.0f/3.0f}},
        {{ 1.0f,  1.0f,  1.0f}, {1,1,1}, {0.75f, 1.0f/3.0f}},
        {{-1.0f,  1.0f,  1.0f}, {1,1,1}, {1.00f, 1.0f/3.0f}},
        {{-1.0f, -1.0f,  1.0f}, {1,1,1}, {1.00f, 2.0f/3.0f}},

        // =========================
        // 左 x = -1
        // =========================
        {{-1.0f, -1.0f,  1.0f}, {1,1,1}, {0.00f, 2.0f/3.0f}},
        {{-1.0f,  1.0f,  1.0f}, {1,1,1}, {0.00f, 1.0f/3.0f}},
        {{-1.0f,  1.0f, -1.0f}, {1,1,1}, {0.25f, 1.0f/3.0f}},
        {{-1.0f, -1.0f, -1.0f}, {1,1,1}, {0.25f, 2.0f/3.0f}},

        // =========================
        // 右 x = +1
        // =========================
        {{ 1.0f, -1.0f, -1.0f}, {1,1,1}, {0.50f, 2.0f/3.0f}},
        {{ 1.0f,  1.0f, -1.0f}, {1,1,1}, {0.50f, 1.0f/3.0f}},
        {{ 1.0f,  1.0f,  1.0f}, {1,1,1}, {0.75f, 1.0f/3.0f}},
        {{ 1.0f, -1.0f,  1.0f}, {1,1,1}, {0.75f, 2.0f/3.0f}},

        // =========================
        // 上 y = +1
        // =========================
        {{-1.0f,  1.0f,  1.0f}, {1,1,1}, {0.25f, 0.00f}},
        {{-1.0f,  1.0f, -1.0f}, {1,1,1}, {0.25f, 1.0f/3.0f}},
        {{ 1.0f,  1.0f, -1.0f}, {1,1,1}, {0.50f, 1.0f/3.0f}},
        {{ 1.0f,  1.0f,  1.0f}, {1,1,1}, {0.50f, 0.00f}},

        // =========================
        // 下 y = -1
        // =========================
        {{-1.0f, -1.0f, -1.0f}, {1,1,1}, {0.25f, 2.0f/3.0f}},
        {{-1.0f, -1.0f,  1.0f}, {1,1,1}, {0.25f, 1.00f}},
        {{ 1.0f, -1.0f,  1.0f}, {1,1,1}, {0.50f, 1.00f}},
        {{ 1.0f, -1.0f, -1.0f}, {1,1,1}, {0.50f, 2.0f/3.0f}},
    };

    meshFbx.m_indices =
{
    // 前
    0, 2, 1,
    0, 3, 2,

    // 后
    4, 6, 5,
    4, 7, 6,

    // 左
    8, 10, 9,
    8, 11, 10,

    // 右
    12, 14, 13,
    12, 15, 14,

    // 上
    16, 18, 17,
    16, 19, 18,

    // 下
    20, 22, 21,
    20, 23, 22
};
}

void Entity_skybox::Update(float deltaTime) {
}

void Entity_skybox::Draw() {
    auto pos = WorldToScreen(position);
    // Global::graphic->DrawText2("zhuzi", pos.x, pos.y);

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
