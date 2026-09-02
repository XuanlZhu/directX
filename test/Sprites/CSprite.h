//
// Created by admin on 2026/9/2.
//

#pragma once

#include <DirectXMath.h>
#include <string>

#include "DDSTextureLoader.h"
#include "Global.h"
#include "Core/CImageManager.h"
#include "Core/CVector2.h"

using namespace DirectX;


class CSprite
{
public:
    CSprite();// 构造

    virtual void Draw();// 绘制
    void SetPosition(float x, float y);// 设置位置
    void SetPosition(XMFLOAT2 pos);// 设置位置
    XMFLOAT2 GetPos();// 获取位置
    virtual void Update(float deltaTime);
    virtual void Destroy();//销毁,子类要重写
    virtual void OnCreated();//当创建

    bool isdraw = true;//精灵表删除标记
    float mSpeed = 5;//速度
    float mChangeX=0;//偏移
    float mChangeY=0;

    XMFLOAT2 mLookat = XMFLOAT2(1,0);

    XMFLOAT2 mPos{};//位置
    float mLength=50;//长度
    float angleDeg = 30;//旋转角
    XMFLOAT2 mVertexs[4];//顶点
    std::string pngName = "gold3";
    // ID3D11ShaderResourceView* mImage = Global::imageManager->GetImage(pngName);//图片
};
