//
// Created by admin on 2026/9/10.
//

#include "Entity.h"

#include <array>

#include "Global.h"
#include "Core/Camera.h"
#undef min
#undef max

Entity::Entity() {
    m_texture = Global::graphic->LoadFBXTexture(L"FBX/096-V_Dif_001.png");
    meshFbx = CModel("FBX/beachBall.fbx");
}

void Entity::Draw() {
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
    Global::graphic->DrawPrimitive3D(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
        mesh.vertices,
        std::size(mesh.vertices),
        sizeof(Vertex3)
    );
}

void Entity::Update(float deltaTime) {
    XMFLOAT3 dir = {mChangeForward, 0, mChangeLeft};

    XMVECTOR pos = XMLoadFloat3(&position);
    XMVECTOR direction = XMVector3Normalize(XMLoadFloat3(&dir));//归一化方向

    pos += direction * 0.1;
    XMStoreFloat3(&position, pos);
}

void Entity::SetPosition(XMFLOAT3 pos) {
    position = pos;
}

XMFLOAT3 Entity::GetPosition() {
    return position;
}

XMMATRIX Entity::GetLocalMatrix() {
    // 缩放
    XMMATRIX scaleX = XMMatrixScaling(
        scale.x,
        scale.y,
        scale.z
    );
    // 旋转
    XMMATRIX rotationX = XMMatrixRotationRollPitchYaw(
        XMConvertToRadians(rotation.x),
        XMConvertToRadians(rotation.y),
        XMConvertToRadians(rotation.z)
    );
    // 位移
    XMMATRIX translation = XMMatrixTranslation(
        position.x,
        position.y,
        position.z
    );
    // World = Scale * Rotation * Translation
    return scaleX * rotationX * translation;
}

void Entity::Rotate(float x, float y, float z) {
}

XMFLOAT3 Entity::Getfacing() {
    XMVECTOR defaultFacing = XMLoadFloat3(&facing);
    XMVECTOR result = XMVector3TransformNormal(defaultFacing, GetWorldMatrix());

    XMFLOAT3 resultFloat;
    XMStoreFloat3(&resultFloat, XMVector3Normalize(result));

    return resultFloat;
}

std::string Entity::GetName() {
    std::string name = typeid(*this).name();
    name = name.substr(6);
    return name;
}

std::array<XMFLOAT3, 2> Entity::GetAABBbox() {
    // =========================
    // 1. 求 Local AABB
    // =========================
    XMFLOAT3 localMin = meshFbx.m_vertices[0].position;
    XMFLOAT3 localMax = meshFbx.m_vertices[0].position;

    for (const auto& vertex : meshFbx.m_vertices)
    {
        const XMFLOAT3& p = vertex.position;

        localMin.x = std::min(localMin.x, p.x);
        localMin.y = std::min(localMin.y, p.y);
        localMin.z = std::min(localMin.z, p.z);

        localMax.x = std::max(localMax.x, p.x);
        localMax.y = std::max(localMax.y, p.y);
        localMax.z = std::max(localMax.z, p.z);
    }

    // =========================
    // 2. Local AABB 的 8 个角点
    // =========================
    std::array<XMFLOAT3, 8> corners =
    {
        XMFLOAT3(localMin.x, localMin.y, localMin.z),
        XMFLOAT3(localMax.x, localMin.y, localMin.z),
        XMFLOAT3(localMax.x, localMax.y, localMin.z),
        XMFLOAT3(localMin.x, localMax.y, localMin.z),

        XMFLOAT3(localMin.x, localMin.y, localMax.z),
        XMFLOAT3(localMax.x, localMin.y, localMax.z),
        XMFLOAT3(localMax.x, localMax.y, localMax.z),
        XMFLOAT3(localMin.x, localMax.y, localMax.z)
    };

    // =========================
    // 3. 变换第一个角点
    // =========================
    XMMATRIX world = GetWorldMatrix();

    XMVECTOR p =
        XMVector3TransformCoord(
            XMLoadFloat3(&corners[0]),
            world
        );

    XMFLOAT3 worldMin;
    XMFLOAT3 worldMax;

    XMStoreFloat3(&worldMin, p);
    XMStoreFloat3(&worldMax, p);

    // =========================
    // 4. 变换剩余 7 个角点
    //    重新计算 World AABB
    // =========================
    for (int i = 1; i < 8; ++i)
    {
        p =
            XMVector3TransformCoord(
                XMLoadFloat3(&corners[i]),
                world
            );

        XMFLOAT3 point;
        XMStoreFloat3(&point, p);

        worldMin.x = std::min(worldMin.x, point.x);
        worldMin.y = std::min(worldMin.y, point.y);
        worldMin.z = std::min(worldMin.z, point.z);

        worldMax.x = std::max(worldMax.x, point.x);
        worldMax.y = std::max(worldMax.y, point.y);
        worldMax.z = std::max(worldMax.z, point.z);
    }

    // =========================
    // 5. 返回 World AABB
    // =========================
    return { worldMin, worldMax };
}

//设置父节点
void Entity::SetFather(Entity *entity) {
    RemoveFather();
    father = entity;
    entity->children.push_back(this);
}

//移除父节点
void Entity::RemoveFather() {
    if (father) {
        auto it = std::find(father->children.begin(),father->children.end(),this);
        if (it != father->children.end())father->children.erase(it);
    }
}

XMMATRIX Entity::GetWorldMatrix() {
    XMMATRIX it = GetLocalMatrix();//先求自己的矩阵
    if (father) {
        it = it*father->GetWorldMatrix();
    }
    return it;
}
