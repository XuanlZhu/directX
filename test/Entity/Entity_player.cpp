//
// Created by admin on 2026/9/21.
//

#include "Entity_player.h"

#include <iostream>

#include "Global.h"
#include "Core/Camera.h"


Entity_player::Entity_player() {
    meshFbx = CModel("FBX/SCP-096.fbx");
    m_texture = Global::graphic->LoadFBXTexture(L"FBX/096-V_Dif_001.png");

    scale= {100, 100, 100};
    rotation = {-90, 180, 90};

}
extern XMFLOAT3 ReflectPoint(Plane plane,XMFLOAT3 point);
extern XMFLOAT3 ReflectDirection(const Plane& plane, const XMFLOAT3& direction);

void Entity_player::Update(float deltaTime) {
    if (mChangeForward!=0 || mChangeLeft!=0) {velocityY=0.01;}//运动状态
    XMFLOAT3 dir = {mChangeForward, mChangeUp, mChangeLeft};

    XMVECTOR dir2 = XMVector3Normalize(XMVectorScale(XMLoadFloat3(&facing),mChangeForward));
    XMVECTOR right = XMVector3Normalize(XMVector3Cross(XMVectorSet(0,1,0,0),XMLoadFloat3(&facing)));

    dir2 = XMVector3Normalize(dir2 - XMVectorScale(right,mChangeLeft));//实际移动方向
    //移动方向射线
    XMFLOAT3 dir3;XMStoreFloat3(&dir3, dir2);
    auto position2 = position;
    position2.y += 0.8;
    auto d2 = RayIntersectOBB(position2,dir3);
    std::cout << d2 << std::endl;
    XMVECTOR pos = XMLoadFloat3(&position);

    pos += dir2 * 0.1;
    if (d2>=0.5 || d2==-1) {
        XMStoreFloat3(&position, pos);
    }
    //-------------------------------
    // Global::ball->position = ReflectPoint(Global::water->plane1,position);
    //碰撞检测
    auto pos2 = position;
    float dis = 1;
    pos2.y = pos2.y+dis;
    auto face = XMFLOAT3{0,-1,0};
    auto d = RayIntersect(pos2,face);

    if (d>=0.2+dis && velocityY!=0 ) {
        position.y -= 1*deltaTime;
    }else if (d!=-1) {
        position.y = position.y + 0.2+dis-d;
        velocityY = 0;//非运动状态
    }
    // }else if (d>=0) {
    //     position.y = position.y +1-d;
    // }
    // if (d>dis+0.05) {
    //     position.y -= 0.5;//重力
    // }
    // if(d>=0) {
    //     position.y += 1-d;//爬坡
    // }

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