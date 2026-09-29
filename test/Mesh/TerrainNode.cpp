//
// Created by admin on 2026/9/28.
//

#include "TerrainNode.h"

#include <iostream>

#include "Global.h"
#include "Core/Camera.h"
#include "Core/Graphic.h"
#include "Entity/Entity_plane.h"
#undef max

void TerrainNode::Separate(int level) {
    // 计算中心点
    float centerX = (min.x + max.x) * 0.5f;
    float centerZ = (min.y + max.y) * 0.5f;
    // 左下
    children[0] = std::make_unique<TerrainNode>();
    children[0]->min = {min.x,min.y};
    children[0]->max = {centerX,centerZ};
    children[0]->entity = entity;
    children[0]->lodLevel = level;
    children[0]->father = this;
    // 右下
    children[1] = std::make_unique<TerrainNode>();
    children[1]->min = {centerX,min.y};
    children[1]->max = {max.x,centerZ};
    children[1]->entity = entity;
    children[1]->lodLevel = level;
    children[1]->father = this;
    // 左上
    children[2] = std::make_unique<TerrainNode>();
    children[2]->min = {min.x,centerZ};
    children[2]->max = {centerX,max.y};
    children[2]->entity = entity;
    children[2]->lodLevel = level;
    children[2]->father = this;
    // 右上
    children[3] = std::make_unique<TerrainNode>();
    children[3]->min = {centerX,centerZ};
    children[3]->max = {max.x,max.y};
    children[3]->entity = entity;
    children[3]->lodLevel = level;
    children[3]->father = this;
}

void TerrainNode::Merge(int level) {

}

void TerrainNode::Draw() {
    if(!isLeaf)//如果是非叶节点
    {
        children[0]->Draw();
        children[1]->Draw();
        children[2]->Draw();
        children[3]->Draw();
    }else {
        // 矩阵数据
        MatrixBuffer matrixData;
        matrixData.world = XMMatrixTranspose(entity->GetWorldMatrix());
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
            // D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
            mesh.m_vertices,
            mesh.m_indices,
            entity->m_texture
        );
    }
}

void TerrainNode::SetLeaf() {
    isLeaf = true;
    //清理节点
    for (auto& child : children)
    {
        child.reset();
    }
    // 清理旧数据
    mesh.m_vertices.clear();mesh.m_indices.clear();
    // 当前区域大小
    float width = max.x - min.x;float depth = max.y - min.y;
    // 根据LOD决定网格密度
    float length = 2;

    if (lodLevel==3) {
        length = 16;
    }else if (lodLevel==2) {
        length = 8;
    }else if (lodLevel==1) {
        length = 2;
    }else if (lodLevel==0) {
        length = 0.5;
    }



    int gridSize = width/length;//多少个方块

    float stepX = width / gridSize;
    float stepZ = depth / gridSize;
    // 生成顶点
    for (int z = 0; z <= gridSize; z++)
    {
        for (int x = 0; x <= gridSize; x++)
        {
            float posX = min.x + x * stepX;
            float posZ = min.y + z * stepZ;
            Vertex3fbx vertex{};
            vertex.position = {
                posX,
                0.0f,  // 高度之后从heightmap采样
                posZ
            };
            vertex.normal = {
                0,
                1,
                0
            };
            vertex.texCoord = {
                (float)x / gridSize,
                (float)z / gridSize
            };
            mesh.m_vertices.push_back(vertex);
        }
    }
    // 生成索引
    for (int z = 0; z < gridSize; z++)
    {
        for (int x = 0; x < gridSize; x++)
        {
            int row1 = z * (gridSize + 1);
            int row2 = (z + 1) * (gridSize + 1);
            // 左下三角
            mesh.m_indices.push_back(row1 + x);
            mesh.m_indices.push_back(row2 + x);
            mesh.m_indices.push_back(row1 + x + 1);
            // 右上三角
            mesh.m_indices.push_back(row1 + x + 1);
            mesh.m_indices.push_back(row2 + x);
            mesh.m_indices.push_back(row2 + x + 1);
        }
    }
}
int count = 0;

int TerrainNode::GetLOD(float distance)
{
    int targetLOD;
    if (distance < 10)
    {
        targetLOD = 0;
    }
    else if (distance < 20)
    {
        targetLOD = 1;
    }
    else if (distance < 30)
    {
        targetLOD = 2;
    }
    else
    {
        targetLOD = 3;
    }
    return targetLOD;
}


//根据摄像机距离判断,太近 → 细分,太远 → 合并
void TerrainNode::UpdateLOD() {
    //先算平面到相机的最近与最远距离，然后决定是否细分

    int targetLOD = GetLOD(GetMinDistance());//目标层级
    // std::cout << "最近层级"<< targetLOD << std::endl;
    // std::cout << "最远层级"<< GetLOD(GetMaxDistance()) << std::endl;
    //---------------------------------
    //细分逻辑：层级不统一
    if (GetLOD(GetMaxDistance())-targetLOD>1)
    // if (targetLOD < lodLevel)
    {
        isLeaf = false;
        // 需要更加精细
        Separate(targetLOD);
    }
    else
    {
        //设置层级
        lodLevel = targetLOD;
        SetLeaf();
    }
    //递归
    for (auto& x:children) {
        // std::cout << "开始UpdateLOD" << std::endl;
        if (x)x->UpdateLOD();
        //递归改为任务队列
        // if (x)entity->queue.push(x.get());
    }
}

float TerrainNode::GetMinDistance() {
    XMFLOAT3 cameraPos = Global::camera->position;
    // 计算相机到矩形平面在 XZ 方向上的最近距离
    float dx = 0.0f;
    float dz = 0.0f;

    if (cameraPos.x < min.x)
        dx = min.x - cameraPos.x;
    else if (cameraPos.x > max.x)
        dx = cameraPos.x - max.x;

    if (cameraPos.z < min.y)
        dz = min.y - cameraPos.z;
    else if (cameraPos.z > max.y)
        dz = cameraPos.z - max.y;

    // Y方向：相机到平面的垂直距离
    float dy = cameraPos.y;

    return sqrt(dx * dx + dy * dy + dz * dz);
}

float TerrainNode::GetMaxDistance() {
    XMFLOAT3 cameraPos = Global::camera->position;
    // X方向：取距离相机最远的边界
    float dx = std::max(std::abs(cameraPos.x - min.x),std::abs(cameraPos.x - max.x));
    // Z方向：取距离相机最远的边界
    float dz = std::max(std::abs(cameraPos.z - min.y),std::abs(cameraPos.z - max.y));
    // Y方向：整个平面都是 Y = 0
    float dy = std::abs(cameraPos.y);
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

void TerrainNode::PrintNodes() {
    std::cout << "节点" << isLeaf<< std::endl;

    for (auto& x:children) {
        if (x)x->PrintNodes();
    }
}
