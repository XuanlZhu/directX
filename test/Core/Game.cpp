//
// Created by admin on 2026/9/2.
//

#include "Game.h"
#include <chrono>
#include <iostream>
#include <vector>

#include "Camera.h"
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
    Global::camera = new Camera();//相机

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
    Global::graphic->BeginFrame();//draw在其中调用
    Global::graphic->EndFrame();
}

//更新
void Game::Update(float deltaTime)
{
    // Global::spriteList->Update(deltaTime);
}
//绘制函数
void Game::Draw()
{
    // DrawTexture("first3",0,0,800,600);
    // DrawLine(XMFLOAT2(0,0), XMFLOAT2(200,200), XMFLOAT3(1,1,1));
}
//输入
void Game::ProcessInput()
{
    // mInput.Update();//输入更新
}
//当键盘按下
void Game::OnKeyPress(int _key) {

}
void Game::OnKeyRelease(int _key) {

}
