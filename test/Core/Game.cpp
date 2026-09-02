//
// Created by admin on 2026/9/2.
//

#include "Game.h"
#include <chrono>
#include <iostream>
#include <vector>

#include "CImageManager.h"
#include "Graphic.h"
#include "../Global.h"
#include "SpriteList.h"
#include "UIManager.h"


Game::Game() {
    Global::spriteList = new SpriteList();
    // Global::graphic = new Graphic();
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
}

//主循环
void Game::Mainloop(float deltaTime)
{
    ProcessInput();//输入
    Update(deltaTime);//更新
    Global::graphic->BeginFrame();
    Global::graphic->EndFrame();
    // Draw();//绘制
}
//当键盘按下
void Game::OnKeyPress(int _key) {

}
void Game::OnKeyRelease(int _key) {

}
//更新
void Game::Update(float deltaTime)
{
    if (GetNowTime() - mRecordTime>5) {
        mRecordTime = GetNowTime();
        CreateCSprite("Sprite_gold", XMFLOAT2(0,0));
    }

    Global::spriteList->Update(deltaTime);
}
//绘制函数
void Game::Draw()
{
    // Global::uiManager->Draw();//UI绘制
    Global::spriteList->Draw();


    float time = GetNowTime();
    // 摆动速度
    float speed = 2.0f;
    // 最大角度
    float maxAngle = 45.0f;
    float angle = sin(time * speed)*maxAngle;

    DrawTexture("first3",0,0,800,600);
    DrawTexture("role1",350,0,100,100);
    DrawTexture("line",378,77,5,50,378,77,angle);
    DrawTexture("grad",365,120,32,19,378,77,angle);
}
//输入
void Game::ProcessInput()
{
    // mInput.Update();//输入更新
}
