//
// Created by admin on 2026/9/7.
//

#pragma once
#include <DirectXMath.h>
#include <vector>
struct Vertex3;
using namespace DirectX;

struct Plane
{
    XMFLOAT3 normal;
    float d;

    float Distance(const XMFLOAT3& point)
    {
        return normal.x * point.x +normal.y * point.y +normal.z * point.z +d;
    }
};

class Camera
{
public:
    Camera();
    XMMATRIX GetViewMatrix();
    virtual void Update(float deltaTime);
    virtual void OnMouseMove(int x, int y,bool isRdown);
    virtual void OnMouseMoveLeft(int x, int y,bool isLdown);
    void SetPosition(XMFLOAT3 pos);
    void DrawFrustum();//画视锥体
    std::vector<Vertex3> VerticesFrustum;//视锥体顶点
    virtual void Draw();// 绘制

    XMFLOAT3 position = { 0.5, 0.0f, -5.0f };//世界坐标
    XMFLOAT3 facing = { 0.0f, 0.0f, 1 };//世界坐标
    XMFLOAT3 uping = { 0, 1, 0 };//世界坐标

    float roll = 0;
    float yaw = 0.0f;
    float pitch = 0.0f;
    float mChangeX = 0.0f;
    float mChangeY = 0.0f;
    float mSpeed = 10;
    float mYaw = 0.0f;
    float mPitch = 0.0f;

    Plane plane1;
    Plane plane2;
};
