//
// Created by admin on 2026/9/10.
//

#include "Entity.h"

#include "Global.h"
#include "Core/Camera.h"

Entity::Entity() {
    m_texture = Global::graphic->LoadFBXTexture(L"FBX/096-V_Dif_001.png");
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
