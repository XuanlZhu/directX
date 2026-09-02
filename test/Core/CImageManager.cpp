//
// Created by admin on 2026/9/2.
//

#include "CImageManager.h"

#include <iostream>
#include <string>

#include "Global.h"
#include "Graphic.h"
#include "WICTextureLoader.h"

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

    return mImages[path].Get();
}