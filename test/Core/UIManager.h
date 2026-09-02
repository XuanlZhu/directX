//
// Created by admin on 2026/9/2.
//

#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>


class UIManager
{
public:
    UIManager();
    void OnCreate();
    void Update(float _deltaTime);//更新
    void Draw();//绘制
    void PrintAttribute(std::string key);
    void Lclick(){mLclick = true;};
    void Rclick(){mLclick = true;};
    // UI_panel* GetElementByid(std::string id){return mPanelMap[id];};

    bool mLclick = false;
    bool mRclick = false;
    // CVector2 printPos;
    // std::shared_ptr<UI_panel> hoverElement;
    // std::unordered_map<std::string, UI_panel*> mPanelMap;
    // UI_panel mRootPanel;
    // std::vector<std::shared_ptr<UI_panel>> mPanels=
    // {
    //     std::make_shared<UI_ability_pick_panel>()
    // };

};
