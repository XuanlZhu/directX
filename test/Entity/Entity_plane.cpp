//
// Created by admin on 2026/9/15.
//

#include "Entity_plane.h"

#include <iostream>

#include "DirectXTex.h"
#include "Global.h"
#include "Core/Camera.h"
#include "Mesh/Mesh_pillar.h"
#undef min
#undef max

Entity_plane::Entity_plane() {
    meshFbx = CModel();
    m_texture = Global::graphic->LoadFBXTexture(L"PNG/map.jpg");
    readHightMap("PNG/high3.bmp");//加载高度图
    scale= {2, 2, 2};


    const int size = 50;
    const float length = 50;
    const float step = length / size;

    meshFbx.m_vertices.clear();
    meshFbx.m_indices.clear();

    // =========================
    // 创建 21 × 21 个顶点
    // =========================
    for (int z = 0; z <= size; ++z)
    {
        for (int x = 0; x <= size; ++x)
        {
            float px = -length / 2.0f + x * step;
            float pz = -length / 2.0f + z * step;

            float u = static_cast<float>(x) / size;
            float v = static_cast<float>(z) / size;

            meshFbx.m_vertices.push_back(
            {
                { px, 0.0f, pz },   // position
                { 1.0f, 1.0f, 1.0f }, // color
        { u, v }
            });
        }
    }

    // =========================
    // 创建三角形索引
    // =========================
    for (int z = 0; z < size; ++z)
    {
        for (int x = 0; x < size; ++x)
        {
            int i0 = z * (size + 1) + x;
            int i1 = i0 + 1;
            int i2 = i0 + (size + 1);
            int i3 = i2 + 1;

            // 第一个三角形
            meshFbx.m_indices.push_back(i0);
            meshFbx.m_indices.push_back(i2);
            meshFbx.m_indices.push_back(i1);

            // 第二个三角形
            meshFbx.m_indices.push_back(i1);
            meshFbx.m_indices.push_back(i2);
            meshFbx.m_indices.push_back(i3);
        }
    }

    ApplyHeightMap();//使用高度图偏移顶点
}


void Entity_plane::Update(float deltaTime) {

}

void Entity_plane::Draw() {
    auto pos = WorldToScreen(position);
    // Global::graphic->DrawText2("zhuzi", pos.x, pos.y);

    // 矩阵数据
    MatrixBuffer matrixData;
    matrixData.world = XMMatrixTranspose(GetWorldMatrix());
    matrixData.view =  XMMatrixTranspose(Global::camera->GetViewMatrix());
    matrixData.projection = XMMatrixTranspose(Global::graphic->m_projection);

    // 把矩阵传给 GPU
    Global::graphic->m_context->UpdateSubresource(
        Global::graphic->m_matrixBuffer,
        0,
        nullptr,
        &matrixData,
        0,
        0
    );
    // 绘制立方体
    Global::graphic->DrawPrimitiveIndexed(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
        meshFbx.m_vertices,
        meshFbx.m_indices,
        m_texture
    );
}

void Entity_plane::readHightMap(std::string path)
{
    std::wstring wpath(path.begin(), path.end());

    DirectX::ScratchImage image;

    // 读取 PNG / JPG
    HRESULT hr = DirectX::LoadFromWICFile(
        wpath.c_str(),
        DirectX::WIC_FLAGS_FORCE_RGB,
        nullptr,
        image
    );

    if (FAILED(hr))
    {
        std::cout << "Failed to load height map\n";
        return;
    }

    // 图片信息
    const DirectX::TexMetadata& metadata = image.GetMetadata();

    const size_t width = metadata.width;
    const size_t height = metadata.height;

    std::cout << "HeightMap: "
              << width << " x " << height << '\n';

    // 分配高度数据
    m_heightData.resize(height);

    for (size_t y = 0; y < height; ++y)
    {
        m_heightData[y].resize(width);
    }

    // 获取图片数据
    const DirectX::Image* img = image.GetImage(0, 0, 0);

    if (!img)
    {
        std::cout << "Failed to get image data\n";
        return;
    }

    // 读取高度
    for (size_t y = 0; y < height; ++y)
    {
        const uint8_t* row =
            img->pixels + y * img->rowPitch;

        for (size_t x = 0; x < width; ++x)
        {
            // 一个像素 4 byte：R G B A
            const uint8_t* pixel =
                row + x * 4;

            // 只使用 R 通道作为高度
            float heightValue =
                static_cast<float>(pixel[0]) / 255.0f;

            m_heightData[y][x] = heightValue;
        }
    }
    std::cout << "Height map loaded\n";

    // for (const auto& row : m_heightData)
    // {
    //     for (float height : row)
    //     {
    //         std::cout << height << ' ';
    //     }
    //
    //     std::cout << '\n';
    // }
}

void Entity_plane::ApplyHeightMap()
{
    if (m_heightData.empty())
        return;

    const int meshWidth = 51;
    const int meshHeight = 51;

    const int heightMapWidth = m_heightData[0].size();
    const int heightMapHeight = m_heightData.size();

    // 最大高度
    const float heightScale = 10.0f;

    for (int z = 0; z < meshHeight; ++z)
    {
        for (int x = 0; x < meshWidth; ++x)
        {
            // mesh 坐标映射到高度图坐标
            int heightX =
                x * (heightMapWidth - 1) / (meshWidth - 1);

            int heightZ =
                z * (heightMapHeight - 1) / (meshHeight - 1);

            // 获取高度
            float height =
                m_heightData[heightZ][heightX];

            // 转换成实际地形高度
            height *= heightScale;

            // mesh 顶点索引
            int index =
                z * meshWidth + x;

            // 修改 Y
            meshFbx.m_vertices[index].position.y = height;
        }
    }
}
