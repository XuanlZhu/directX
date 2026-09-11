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
#include "CameraFPS.h"
#include "Graphic.h"
#include "../Global.h"
#include "SpriteList.h"
#include "CameraTPS.h"
#include "UIManager.h"
#include "../Entity/Entity.h"
#include "../Entity/EntityManager.h"
#include "Mesh/Mesh.h"


Game::Game() {
    Global::spriteList = new SpriteList();
    Global::graphic = new Graphic();
    Global::imageManager = new CImageManager();
    Global::uiManager = new UIManager();
    Global::camera = new Camera();//相机
    Global::mesh = new Mesh();//网格体
    Global::entityManager = new EntityManager();//实体管理器
    Global::player = CreateEntity("Entity",XMFLOAT3{0,0,0}).get();//玩家

    Global::cameraFPS = new CameraFPS();//FPS相机
    Global::cameraTPS = new CameraTPS();//TPS相机
}
//初始化
void Game::Setup()
{
    Global::camera = Global::cameraTPS;//切换相机
    CreateEntity("Entity_pillar",XMFLOAT3{5,0,5});
    CreateEntity("Entity_pillar",XMFLOAT3{-5,0,5});
    CreateEntity("Entity_pillar",XMFLOAT3{5,0,-5});
    CreateEntity("Entity_pillar",XMFLOAT3{-5,0,-5});
    // CreateEntity("Entity_pillar",XMFLOAT3{5,0,0});

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
    Global::entityManager->Update(deltaTime);
    Global::camera->Update(deltaTime);
}
//绘制函数
void Game::Draw()
{
    Global::entityManager->Draw();//绘制实体
    Global::camera->Draw();
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
    if (_key == EKey::Space) {

    }
    //按F画视锥体
    if (_key == EKey::F) {
        Global::camera->DrawFrustum();
    }


    //相机移动
    if (_key == EKey::W) {
        Global::player->mChangeForward = 1;
    }
    if (_key == EKey::A) {
        Global::player->mChangeLeft = 1;
    }
    if (_key == EKey::S) {
        Global::player->mChangeForward = -1;
    }
    if (_key == EKey::D) {
        Global::player->mChangeLeft = -1;
    }
}
void Game::OnKeyRelease(int _key) {
    if (_key == EKey::W) {
        Global::player->mChangeForward = 0;
    }
    if (_key == EKey::A) {
        Global::player->mChangeLeft = 0;
    }
    if (_key == EKey::S) {
        Global::player->mChangeForward = 0;
    }
    if (_key == EKey::D) {
        Global::player->mChangeLeft = 0;
    }

}
