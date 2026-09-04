//
// Created by admin on 2026/9/2.
//

#include "Game.h"
#include <chrono>
#include <iostream>
#include <vector>

#include "CImageManager.h"
#include "EKey.h"
#include "Graphic.h"
#include "../Global.h"
#include "SpriteList.h"
#include "UIManager.h"


Game::Game() {
    Global::spriteList = new SpriteList();
    Global::graphic = new Graphic();
    Global::imageManager = new CImageManager();
    Global::uiManager = new UIManager();
    //按键绑定
    // mInput.OnKeyDown = [this](int _key)
    // {
    //     OnKeyPress(_key);
    // };
    // mInput.OnKeyUp = [this](int _key)
    // {
    //     OnKeyRelease(_key);
    // };
}
//初始化
void Game::Setup()
{
    for (int i = 0; i < 5; i++) {
        CreateCSprite("Sprite_gold", XMFLOAT2(RandomInt(0,800),RandomInt(200,600)));
    }
    Global::claw = CreateCSprite("Sprite_claw", XMFLOAT2(365+16,120)).lock().get();
}

//主循环
void Game::Mainloop(float deltaTime)
{
    ProcessInput();//输入
    // std::cout << "开始Update" << std::endl;
    Update(deltaTime);//更新
    // std::cout << "开始BeginFrame" << std::endl;
    Global::graphic->BeginFrame();
    // std::cout << "开始EndFrame" << std::endl;
    Global::graphic->EndFrame();
    // Draw();//绘制
}

//更新
void Game::Update(float deltaTime)
{
    // if (GetNowTime() - mRecordTime>1) {
    //     mRecordTime = GetNowTime();
    //     CreateCSprite("Sprite_gold", XMFLOAT2(RandomInt(0,600),RandomInt(200,500)));
    // }

    Global::spriteList->Update(deltaTime);
}
//绘制函数
void Game::Draw()
{
    // Global::uiManager->Draw();//UI绘制
    // std::cout << "Game::Draw" << std::endl;

    float time = GetNowTime();
    // std::cout << "GetNowTime" << std::endl;
    // 摆动速度
    float speed = 2.0f;
    // 最大角度
    float maxAngle = 45.0f;
    float angle = sin(time * speed)*maxAngle;

    DrawTexture("first3",0,0,800,600);
    DrawTexture("role1",350,0,100,100);
    // DrawTexture("line",378,77,5,50,378,77,angle);
    // DrawTexture("grad",365,120,32,19,378,77,angle);
    // std::cout << "Game::Draw结束，进入Global::spriteList->Draw" << std::endl;
    Global::spriteList->Draw();
}
//输入
void Game::ProcessInput()
{
    // mInput.Update();//输入更新
}
//当键盘按下
void Game::OnKeyPress(int _key) {
    if (_key == EKey::LClick) {
        // std::cout << "鼠标左键按下" << std::endl;
        Global::claw->ThrowOut();
    }

    if (_key == EKey::W) {
        // Global::claw->mChangeY = -1;
    }
    if (_key == EKey::A) {
        // Global::claw->mChangeX = -1;
    }
    if (_key == EKey::S) {
        // Global::claw->mChangeY = 1;
    }
    if (_key == EKey::D) {
        // Global::claw->mChangeX = 1;
    }
}
void Game::OnKeyRelease(int _key) {
    if (_key == EKey::W) {
        Global::claw->mChangeY = 0;
    }
    if (_key == EKey::A) {
        Global::claw->mChangeX = 0;
    }
    if (_key == EKey::S) {
        Global::claw->mChangeY = 0;
    }
    if (_key == EKey::D) {
        Global::claw->mChangeX = 0;
    }
}
