//
// Created by admin on 2026/9/10.
//

#pragma once
#include "Camera.h"


class CameraTPS : public Camera
{
public:
    CameraTPS();
    void Update(float deltaTime) override;
    void OnMouseMoveLeft(int x, int y,bool isLdown) override;

    XMFLOAT3 dir = {-1, 0, 0};
};
