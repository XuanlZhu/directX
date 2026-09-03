//
// Created by admin on 2026/9/2.
//

#pragma once
#include <unordered_map>
#include <string>
#include <wrl/client.h>

#include "DDSTextureLoader.h"


class CImageManager
{
public:
    ID3D11ShaderResourceView* GetImagebyPath(std::string path);
    ID3D11ShaderResourceView* GetImage(std::string name);

    std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> mImages;
    std::unordered_map<std::string, std::vector<std::vector<int>>> mAalpha;
};
