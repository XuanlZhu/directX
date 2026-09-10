//
// Created by admin on 2026/9/10.
//

#pragma once
#include "Camera.h"


class CameraFPS :public Camera
{
public:
    void Update(float deltaTime) override;
};
