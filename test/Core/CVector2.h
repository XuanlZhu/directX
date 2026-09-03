//
// Created by admin on 2026/9/2.
//

#pragma once
#include <cmath>

class CVector2
{
public:
    float x;
    float y;
    CVector2();
    CVector2(float _x, float _y);
    //加法
    CVector2 operator+(const CVector2& _other) const;
    //减法
    CVector2 operator-(const CVector2& _other) const;
    //乘法
    CVector2 operator*(float _value) const;
    float Length() const;//长度
    CVector2 Normalize() const;//归1化
    float Dot(CVector2 _other){return x * _other.x + y * _other.y;};
    float ToAngle(){return std::atan2(y, x) * 180.0f / 3.1415926f;};//转角度
    CVector2 Rotate(float _angle);
    CVector2 Rotate(CVector2 pos,float angleDeg);

};
