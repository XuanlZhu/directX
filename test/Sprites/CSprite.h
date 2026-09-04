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
    void SetPosition(CVector2 pos);// 设置位置
    CVector2 GetPos();// 获取位置
    virtual void Update(float deltaTime);
    virtual void Destroy();//销毁,子类要重写
    virtual void OnCreated();//当创建
    virtual void ThrowOut(){};//扔出
    virtual void Crawled(){};//被抓取


    bool isdraw = true;//精灵表删除标记
    CVector2 dir;//方向
    // bool isThrow = false;//扔出中
    float mSpeed = 100;//速度
    float mChangeX=0;//偏移
    float mChangeY=0;


    CVector2 mLookat = CVector2(1,0);

    CVector2 mPosInit;//初始位置
    CVector2 mPos;//位置
    float mLength=50;//长度

    CVector2 mVertexs[4];//顶点
    std::string pngName = "claw";
    // ID3D11ShaderResourceView* mImage = Global::imageManager->GetImage(pngName);//图片
    float oWidth = 50;//原始宽高
    float oHeight = 50;//原始宽高
    float mWidth = 50;//宽
    float mHeight = 50;//高
    float angleDeg = 0;//旋转角
    XMFLOAT2 XAnchor{0,0};//锚点
    XMFLOAT2 XScale{1,1};//缩放
    XMFLOAT2 XPosition{0,0};//世界坐标
    XMFLOAT2 XLocalVertices[4];//局部顶点
    XMFLOAT2 XWorldVertices[4];//世界顶点
    XMFLOAT2 XBoxVertices[4];//包围盒顶点
    XMMATRIX GetWorldMatrix() {
        // 缩放
        XMMATRIX scale = XMMatrixScaling(XScale.x,XScale.y,1.0f);
        // 旋转
        XMMATRIX rotation = XMMatrixRotationZ(XMConvertToRadians(angleDeg));
        // 平移
        XMMATRIX translation = XMMatrixTranslation(mPos.x,mPos.y,0.0f);
        // 世界矩阵
        return scale * rotation * translation;
    };//世界矩阵
    void GetWorldVertices() {
        XMMATRIX world = GetWorldMatrix();
        for(int i = 0; i < 4; i++)
        {
            XMVECTOR local = XMVectorSet(XLocalVertices[i].x,XLocalVertices[i].y,0.0f,1.0f);//创建局部坐标
            XMVECTOR worldPos = XMVector3Transform(local,world);//计算世界坐标，向量*矩阵
            XMStoreFloat2(&XWorldVertices[i],worldPos);//转回Float2
        }
    };//获取世界顶点
    void GetBoxVertices() {
        //计算包围盒顶点
        float minX = XWorldVertices[0].x;float maxX = XWorldVertices[0].x;float minY = XWorldVertices[0].y;float maxY = XWorldVertices[0].y;
        for(int i=1;i<4;i++)
        {
            minX = min(minX, XWorldVertices[i].x);
            maxX = max(maxX, XWorldVertices[i].x);
            minY = min(minY, XWorldVertices[i].y);
            maxY = max(maxY, XWorldVertices[i].y);
        }
        XBoxVertices[0] = XMFLOAT2(minX,minY); // 左上
        XBoxVertices[1] = XMFLOAT2(maxX,minY); // 右上
        XBoxVertices[2] = XMFLOAT2(minX,maxY); // 左下
        XBoxVertices[3] = XMFLOAT2(maxX,maxY); // 右下
    };//获取包围盒顶点
    CSprite* CollisionDetection();
    bool BoxDetection(CSprite* other);//包围盒检测
    bool OBBDetection(CSprite* other);//顶点检测
    bool PixelDetection(CSprite* other);//像素检测


};
