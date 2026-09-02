//
// Created by admin on 2026/9/1.
//

#pragma once

#include <DirectXMath.h>
#include <memory>
#include <Windows.h>
#include <string>
class UIManager;
using namespace DirectX;

class CSprite;
class Graphic;
class CImageManager;
class SpriteList;
class Game;

class Global {
public:
    inline static Game* game = nullptr;//游戏
    inline static SpriteList* spriteList = nullptr;//精灵表
    inline static CImageManager* imageManager = nullptr;//图片管理器
    inline static Graphic* graphic = nullptr;//渲染器
    inline static UIManager* uiManager = nullptr;//ui管理器
};


std::wstring StringToWString(std::string str);
float GetNowTime();//单位是秒
std::weak_ptr<CSprite> CreateCSprite(std::string _sprite, XMFLOAT2 _pos);//创建精灵，并下发到精灵表