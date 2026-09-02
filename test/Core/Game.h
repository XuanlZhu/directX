//
// Created by admin on 2026/9/2.
//

#pragma once
#include <graphics.h>
#include <thread>
#include <atomic>
#include <chrono>
// #include "CInput.h"

class CSprite;
class CScene;

class Game
{
public:
    Game();

    void Mainloop(float deltaTime);//主循环
    virtual void OnKeyPress(int _key);
    virtual void OnKeyRelease(int _key);
    std::chrono::time_point<std::chrono::high_resolution_clock> mStartTime = std::chrono::high_resolution_clock::now();
protected:
    virtual void Setup();//初始化
    virtual void Update(float _deltaTime);//每帧更新
    virtual void Draw();//绘制
    virtual void ProcessInput();//输入处理

};
