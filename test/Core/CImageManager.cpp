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

        int width = (int)metadata.width;
        int height = (int)metadata.height;
        std::vector<std::vector<int>> alpha;
        // 第一维是x(宽)
        alpha.resize(width);
        for(int x = 0; x < width; x++)
        {
            // 第二维是y(高)
            alpha[x].resize(height);
        }
        auto img = image.GetImage(0,0,0);

        for(int y = 0; y < height; y++)
        {
            const unsigned char* row =
                img->pixels + y * img->rowPitch;
            for(int x = 0; x < width; x++)
            {
                unsigned char a = row[x * 4 + 3];

                alpha[x][y] = (int)a;
            }
        }
        mAalpha[path] = alpha;
    }

    return mImages[path].Get();
}

int CImageManager::GetImageAalpha(std::string name, int x, int y) {
    name = "PNG/" + name + ".png";
    // std::cout << "缩放为像素" << x << "|" << y<< std::endl;
    // std::cout << "alpha大小X" << mAalpha[name].size() << std::endl;
    // std::cout << "alpha大小Y" << mAalpha[name][0].size()<< std::endl;
    // std::cout << mAalpha[name][0][0] << std::endl;
    // std::cout << "Aalpha大小X" << mAalpha[name].size() << x << std::endl;
    // std::cout << "Aalpha大小Y" << mAalpha[name][0].size() << y << std::endl;
    return mAalpha[name][x][y];
}
