//
// Created by admin on 2026/9/2.
//

#pragma once

#include <DirectXMath.h>
#include <graphics.h>
#include <string>

#include "DDSTextureLoader.h"

using namespace DirectX;


class CSprite
{
public:
    CSprite();// 构造
    CSprite(std::string _path);
    CSprite(std::string _path,float _length);

    virtual void Draw();// 绘制
    void SetPosition(float x, float y);// 设置位置
    void SetPosition(XMFLOAT2 pos);// 设置位置
    XMFLOAT2 GetPos();// 获取位置
    virtual void Update(float deltaTime);
    virtual void Destroy();//销毁,子类要重写

    bool isdraw = true;//精灵表删除标记
    float mSpeed = 5;//速度
    float mChangeX=0;//偏移
    float mChangeY=0;
    float mLength=50;//长度
    XMFLOAT2 mLookat = XMFLOAT2(1,0);
protected:
    ID3D11ShaderResourceView* mImage = nullptr;//图片
    XMFLOAT2 mPos;//位置
    std::string mImagePath;//图片路径，需要使用资源管理器，而不是持有mImage
};
