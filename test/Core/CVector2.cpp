//
// Created by admin on 2026/9/2.
//

#include "CVector2.h"

#include <cmath>

CVector2::CVector2()
{
    x = 0;
    y = 0;
}

CVector2::CVector2(float _x,float _y)
{
    x = _x;
    y = _y;
}



CVector2 CVector2::operator+(const CVector2& _other) const
{
    return CVector2(x + _other.x,y + _other.y);
}



CVector2 CVector2::operator-(const CVector2& _other) const
{
    return CVector2(x - _other.x,y - _other.y);
}



CVector2 CVector2::operator*(float _value) const
{
    return CVector2(x * _value,y * _value);
}

float CVector2::Length() const {
    return std::sqrt(x * x + y * y);
}

CVector2 CVector2::Normalize() const {
    float len = Length();
    //防止除0
    if(len == 0)
    {
        return CVector2(0, 0);
    }
    return CVector2(x / len,y / len);
}

CVector2 CVector2::Rotate(float _angle)
{
    // 角度转弧度
    float rad = _angle * 3.1415926535f / 180.0f;

    float cosA = cos(rad);
    float sinA = sin(rad);

    return CVector2(x * cosA - y * sinA,x * sinA + y * cosA);
}

CVector2 CVector2::Rotate(CVector2 pos, float angleDeg) {
    // 1. 把自身坐标转换到以旋转中心为原点
    float x = this->x - pos.x;
    float y = this->y - pos.y;
    // 2. 角度转弧度
    float rad = angleDeg * 3.14159265f / 180.0f;

    float cosA = cosf(rad);
    float sinA = sinf(rad);
    // 3. 旋转
    float rx = x * cosA - y * sinA;
    float ry = x * sinA + y * cosA;
    // 4. 移回世界坐标
    return CVector2(
        rx + pos.x,
        ry + pos.y
    );
}
