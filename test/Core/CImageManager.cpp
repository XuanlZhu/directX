//
// Created by admin on 2026/9/2.
//

#include "CImageManager.h"

#include <iostream>
#include <string>

#include "Global.h"
#include "Graphic.h"
#include "WICTextureLoader.h"
#include <DirectXTex.h>

ID3D11ShaderResourceView* CImageManager::GetImagebyPath(std::string path) {

    auto it = mImages.find(path); //已经加载
    if(it != mImages.end()) { return it->second.Get(); } //第一次加载

    std::wstring wpath = StringToWString(path);

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
    HRESULT hr = DirectX::CreateWICTextureFromFile(
        Global::graphic->m_device,
        Global::graphic->m_context,
        wpath.c_str(),
        nullptr,
        texture.GetAddressOf()
    );
    if (FAILED(hr))
    {
        std::cout << "资源加载错误" << std::endl;
        return nullptr;
    }
    // 放入缓存
    mImages[path] = texture;

    return mImages[path].Get();
}


ID3D11ShaderResourceView* CImageManager::GetImage(std::string path) {
    path = "PNG/" + path + ".png";
    auto it = mImages.find(path); //已经加载
    if(it != mImages.end()) { return it->second.Get(); } //第一次加载

    std::wstring wpath = StringToWString(path);

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
    HRESULT hr = DirectX::CreateWICTextureFromFile(
        Global::graphic->m_device,
        Global::graphic->m_context,
        wpath.c_str(),
        nullptr,
        texture.GetAddressOf()
    );
    if (FAILED(hr))
    {
        std::cout << "资源加载错误" << std::endl;
        return nullptr;
    }
    // 放入缓存
    mImages[path] = texture;


    // 保存alpha数据
    DirectX::ScratchImage image;
    hr = DirectX::LoadFromWICFile(
        wpath.c_str(),
        DirectX::WIC_FLAGS_NONE,
        nullptr,
        image
    );
    if(SUCCEEDED(hr))
    {
        auto metadata = image.GetMetadata();
        int width = metadata.width;
        int height = metadata.height;
        std::vector<std::vector<int>> alpha;
        alpha.resize(height);
        for(int y = 0; y < height; y++)
        {
            alpha[y].resize(width);
        }
        auto pixels = image.GetPixels();
        for(int y = 0; y < height; y++)
        {
            for(int x = 0; x < width; x++)
            {
                int index = (y * width + x) * 4;
                //RGBA
                unsigned char a = pixels[index + 3];
                alpha[y][x] = a;
            }
        }
        mAalpha[path] = alpha;
    }

    return mImages[path].Get();
}