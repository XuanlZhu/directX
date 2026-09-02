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
}

//主循环
void Game::Mainloop(float deltaTime)
{
    ProcessInput();//输入
    Update(deltaTime);//更新
    Draw();//绘制
}
//当键盘按下
void Game::OnKeyPress(int _key) {

}
void Game::OnKeyRelease(int _key) {

}
//更新
void Game::Update(float deltaTime)
{
    Global::spriteList->Update(deltaTime);
}
//绘制函数
void Game::Draw()
{
    // Global::uiManager->Draw();//UI绘制
    Global::graphic->BeginFrame();
    Global::graphic->EndFrame();
}
//输入
void Game::ProcessInput()
{
    // mInput.Update();//输入更新
}
