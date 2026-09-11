//
// Created by admin on 2026/9/11.
//

#include "CModel.h"

#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Core/Graphic.h"

void CModel::Load(std::string _path)
{
    m_vertices.clear();
    m_indices.clear();

    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(
        _path,
        aiProcess_Triangulate |
        aiProcess_GenSmoothNormals |
        aiProcess_FlipUVs
    );

    if (!scene ||
        !scene->mRootNode ||
        (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE))
    {
        std::cout << "Assimp Error: "
                  << importer.GetErrorString()
                  << std::endl;
        return;
    }

    ProcessNode(scene->mRootNode, scene);
}


void CModel::Draw() {
}

void CModel::ProcessNode(aiNode* _node,const aiScene* _scene)
{
    for (unsigned int i = 0;i < _node->mNumMeshes;++i)
    {
        aiMesh* mesh = _scene->mMeshes[_node->mMeshes[i]];
        ProcessMesh(mesh, _scene);
    }

    for (unsigned int i = 0;i < _node->mNumChildren;++i)
    {
        ProcessNode(_node->mChildren[i], _scene);
    }
}

void CModel::ProcessMesh(aiMesh* _mesh,const aiScene* _scene)
{
    uint32_t vertexOffset = static_cast<uint32_t>(m_vertices.size());

    for (unsigned int i = 0;i < _mesh->mNumVertices;++i)
    {
        Vertex3fbx vertex{};

        vertex.position = {
            _mesh->mVertices[i].x,
            _mesh->mVertices[i].y,
            _mesh->mVertices[i].z
        };

        if (_mesh->HasNormals())
        {
            vertex.normal = {
                _mesh->mNormals[i].x,
                _mesh->mNormals[i].y,
                _mesh->mNormals[i].z
            };
        }

        if (_mesh->HasTextureCoords(0))
        {
            vertex.texCoord = {
                _mesh->mTextureCoords[0][i].x,
                _mesh->mTextureCoords[0][i].y
            };
        }

        m_vertices.push_back(vertex);
    }

    for (unsigned int i = 0;i < _mesh->mNumFaces;++i)
    {
        const aiFace& face = _mesh->mFaces[i];

        for (unsigned int j = 0;j < face.mNumIndices;++j)
        {
            m_indices.push_back(
                vertexOffset + face.mIndices[j]
            );
        }
    }
}

