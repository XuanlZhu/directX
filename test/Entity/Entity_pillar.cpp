//
// Created by admin on 2026/9/10.
//

#include "Entity_pillar.h"

#include <iostream>

#include "Global.h"
#include "Core/Camera.h"
#include "Mesh/Mesh_pillar.h"
#undef min
#undef max

Entity_pillar::Entity_pillar() {
    mesh = Mesh_pillar();
    meshFbx = CModel("FBX/SCP-096.fbx");
    scale= {100, 100, 100};
    rotation = {-90, 45, 90};

}

void Entity_pillar::GetLocalAABB()
{

    XMFLOAT3 min = meshFbx.m_vertices[0].position;
    XMFLOAT3 max = meshFbx.m_vertices[0].position;

    for (const auto& vertex : meshFbx.m_vertices)
    {
        const XMFLOAT3& p = vertex.position;

        min.x = std::min(min.x, p.x);
        min.y = std::min(min.y, p.y);
        min.z = std::min(min.z, p.z);

        max.x = std::max(max.x, p.x);
        max.y = std::max(max.y, p.y);
        max.z = std::max(max.z, p.z);
    }
    // 8 个 AABB 顶点
    std::vector<Vertex3> localBox =
    {
        {{min.x, min.y, min.z}, {1, 0, 0}},
        {{max.x, min.y, min.z}, {1, 0, 0}},
        {{max.x, max.y, min.z}, {1, 0, 0}},
        {{min.x, max.y, min.z}, {1, 0, 0}},

        {{min.x, min.y, max.z}, {1, 0, 0}},
        {{max.x, min.y, max.z}, {1, 0, 0}},
        {{max.x, max.y, max.z}, {1, 0, 0}},
        {{min.x, max.y, max.z}, {1, 0, 0}}
    };

    // AABB 顶点线
    VerticesLocalBox =
    {
        // 前面 z = min.z
        {{min.x, min.y, min.z}, {1, 0, 0}}, // 0
        {{max.x, min.y, min.z}, {1, 0, 0}}, // 1

        {{max.x, min.y, min.z}, {1, 0, 0}}, // 1
        {{max.x, max.y, min.z}, {1, 0, 0}}, // 2

        {{max.x, max.y, min.z}, {1, 0, 0}}, // 2
        {{min.x, max.y, min.z}, {1, 0, 0}}, // 3

        {{min.x, max.y, min.z}, {1, 0, 0}}, // 3
        {{min.x, min.y, min.z}, {1, 0, 0}}, // 0


        // 后面 z = max.z
        {{min.x, min.y, max.z}, {1, 0, 0}}, // 4
        {{max.x, min.y, max.z}, {1, 0, 0}}, // 5

        {{max.x, min.y, max.z}, {1, 0, 0}}, // 5
        {{max.x, max.y, max.z}, {1, 0, 0}}, // 6

        {{max.x, max.y, max.z}, {1, 0, 0}}, // 6
        {{min.x, max.y, max.z}, {1, 0, 0}}, // 7

        {{min.x, max.y, max.z}, {1, 0, 0}}, // 7
        {{min.x, min.y, max.z}, {1, 0, 0}}, // 4


        // 连接前后
        {{min.x, min.y, min.z}, {1, 0, 0}}, // 0
        {{min.x, min.y, max.z}, {1, 0, 0}}, // 4

        {{max.x, min.y, min.z}, {1, 0, 0}}, // 1
        {{max.x, min.y, max.z}, {1, 0, 0}}, // 5

        {{max.x, max.y, min.z}, {1, 0, 0}}, // 2
        {{max.x, max.y, max.z}, {1, 0, 0}}, // 6

        {{min.x, max.y, min.z}, {1, 0, 0}}, // 3
        {{min.x, max.y, max.z}, {1, 0, 0}}  // 7
    };

    auto world = GetWorldMatrix();
    // 第一个点变换到世界空间
    XMVECTOR p = XMVector3TransformCoord(XMLoadFloat3(&localBox[0].position), world);

    XMStoreFloat3(&min, p);
    XMStoreFloat3(&max, p);

    // 变换8个角点，然后重新求 min/max
    for (int i = 1; i < 8; ++i)
    {
        p = XMVector3TransformCoord(XMLoadFloat3(&localBox[i].position), world);

        XMFLOAT3 point;
        XMStoreFloat3(&point, p);
        // std::cout << "世界坐标"<< std::endl;
        // std::cout << point.x<<"|" << point.y<<"|" << point.z<< std::endl;

        min.x = std::min(min.x, point.x);
        min.y = std::min(min.y, point.y);
        min.z = std::min(min.z, point.z);

        max.x = std::max(max.x, point.x);
        max.y = std::max(max.y, point.y);
        max.z = std::max(max.z, point.z);
    }

    // 世界OBB 顶点线
    WorldAABB =
    {
        // 前面 z = min.z
        {{min.x, min.y, min.z}, {1, 0, 0}}, // 0
        {{max.x, min.y, min.z}, {1, 0, 0}}, // 1

        {{max.x, min.y, min.z}, {1, 0, 0}}, // 1
        {{max.x, max.y, min.z}, {1, 0, 0}}, // 2

        {{max.x, max.y, min.z}, {1, 0, 0}}, // 2
        {{min.x, max.y, min.z}, {1, 0, 0}}, // 3

        {{min.x, max.y, min.z}, {1, 0, 0}}, // 3
        {{min.x, min.y, min.z}, {1, 0, 0}}, // 0


        // 后面 z = max.z
        {{min.x, min.y, max.z}, {1, 0, 0}}, // 4
        {{max.x, min.y, max.z}, {1, 0, 0}}, // 5

        {{max.x, min.y, max.z}, {1, 0, 0}}, // 5
        {{max.x, max.y, max.z}, {1, 0, 0}}, // 6

        {{max.x, max.y, max.z}, {1, 0, 0}}, // 6
        {{min.x, max.y, max.z}, {1, 0, 0}}, // 7

        {{min.x, max.y, max.z}, {1, 0, 0}}, // 7
        {{min.x, min.y, max.z}, {1, 0, 0}}, // 4


        // 连接前后
        {{min.x, min.y, min.z}, {1, 0, 0}}, // 0
        {{min.x, min.y, max.z}, {1, 0, 0}}, // 4

        {{max.x, min.y, min.z}, {1, 0, 0}}, // 1
        {{max.x, min.y, max.z}, {1, 0, 0}}, // 5

        {{max.x, max.y, min.z}, {1, 0, 0}}, // 2
        {{max.x, max.y, max.z}, {1, 0, 0}}, // 6

        {{min.x, max.y, min.z}, {1, 0, 0}}, // 3
        {{min.x, max.y, max.z}, {1, 0, 0}}  // 7
    };
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
        meshFbx.m_indices,
        m_texture
    );

    GetLocalAABB();
    // 绘制AABB包围盒
    Global::graphic->DrawPrimitive3D(
        D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
        VerticesLocalBox,
        VerticesLocalBox.size(),
        sizeof(Vertex3)
    );
    // 把矩阵传给 GPU
    matrixData.world = XMMatrixIdentity();
    Global::graphic->m_context->UpdateSubresource(
        Global::graphic->m_matrixBuffer,
        0,
        nullptr,
        &matrixData,
        0,
        0
    );
    // 绘制OBB包围盒
    Global::graphic->DrawPrimitive3D(
        D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
        WorldAABB,
        WorldAABB.size(),
        sizeof(Vertex3)
    );
}
