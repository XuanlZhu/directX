#include "Graphic.h"

#include <iostream>
#include <d3dcompiler.h>

#include "Camera.h"
#include "Game.h"
#include "SpriteFont.h"
#include "../Global.h"
#include "Mesh/Mesh.h"


// Graphic::Graphic() {
//     test_texture = LoadTexture("PNG/first3.png");
// }

Graphic::~Graphic()
{
    if(m_renderTargetView)
    {
        m_renderTargetView->Release();
        m_renderTargetView = nullptr;
    }

    if(m_swapChain)
    {
        m_swapChain->Release();
        m_swapChain = nullptr;
    }

    if(m_context)
    {
        m_context->Release();
        m_context = nullptr;
    }

    if(m_device)
    {
        m_device->Release();
        m_device = nullptr;
    }
}

bool Graphic::Initialize(HWND hWnd)
{
    m_hWnd = hWnd;

    RECT rc;
    GetClientRect(hWnd, &rc);

    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    DXGI_SWAP_CHAIN_DESC swapDesc = {};

    swapDesc.BufferCount = 1;
    swapDesc.BufferDesc.Width = width;
    swapDesc.BufferDesc.Height = height;
    swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapDesc.OutputWindow = hWnd;
    swapDesc.SampleDesc.Count = 1;
    swapDesc.Windowed = TRUE;

    D3D_FEATURE_LEVEL featureLevel;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &swapDesc,
        &m_swapChain,
        &m_device,
        &featureLevel,
        &m_context
    );

    if(FAILED(hr))
        return false;


    ID3D11Texture2D* backBuffer = nullptr;

    hr = m_swapChain->GetBuffer(
        0,
        __uuidof(ID3D11Texture2D),
        (void**)&backBuffer
    );

    if(FAILED(hr))
        return false;


    hr = m_device->CreateRenderTargetView(
        backBuffer,
        nullptr,
        &m_renderTargetView
    );
    // 设置 Viewport
    D3D11_VIEWPORT viewport{};

    viewport.Width  = (float)width;
    viewport.Height = (float)height;

    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;


    m_context->RSSetViewports(
        1,
        &viewport
    );

    backBuffer->Release();

    if(FAILED(hr))
        return false;

    CreateVertexBuffer();
    CreatePixelShader();
    CreateVertexShader();
    CreateSampler();
    CreateBlendState();
    InitlineVertex();
    InitVertex3();
    //投影矩阵
    m_projection = DirectX::XMMatrixPerspectiveFovLH(
        DirectX::XMConvertToRadians(60.0f),
        800.0f / 600.0f,
        0.1f,
        100.0f
    );
    //创建m_matrixBuffer
    D3D11_BUFFER_DESC desc = {};
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.ByteWidth = sizeof(MatrixBuffer);
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    m_device->CreateBuffer(
        &desc,
        nullptr,
        &m_matrixBuffer
    );
    //创建深度状态
    D3D11_DEPTH_STENCIL_DESC depthDesc = {};
    depthDesc.DepthEnable = TRUE;
    depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthDesc.DepthFunc = D3D11_COMPARISON_LESS;
    m_device->CreateDepthStencilState(
        &depthDesc,
        &m_depthStencilState
    );
    // 1. 创建深度缓冲区
    ID3D11Texture2D* depthBuffer = nullptr;
    D3D11_TEXTURE2D_DESC depthDesc2 = {};
    depthDesc2.Width = width;
    depthDesc2.Height = height;
    depthDesc2.MipLevels = 1;
    depthDesc2.ArraySize = 1;
    depthDesc2.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthDesc2.SampleDesc.Count = 1;
    depthDesc2.Usage = D3D11_USAGE_DEFAULT;
    depthDesc2.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    m_device->CreateTexture2D(
        &depthDesc2,
        nullptr,
        &depthBuffer
    );
    m_device->CreateDepthStencilView(
        depthBuffer,
        nullptr,
        &m_depthStencilView
    );

    //矩阵初始化------------------------------------------------------
    // VS 使用buff
    m_context->VSSetConstantBuffers(
        0,
        1,
        &m_matrixBuffer
    );
    //文字初始化--------------------------------------------------------
    m_spriteBatch = new SpriteBatch(m_context);

    m_font = new SpriteFont(
        m_device,
        L"font.spritefont"
    );
    //初始化FBX
    InitVertexFBX();

    return true;
}
//地面
std::vector<Vertex3> vertices =
{
    // 第一个三角形
    {{-50.0f, 0.0f, -50.0f}, {0.5f, 0.5f, 0.5f}},
    {{-50.0f, 0.0f,  50.0f}, {0.5f, 0.5f, 0.5f}},
    {{ 50.0f, 0.0f,  50.0f}, {0.5f, 0.5f, 0.5f}},

    // 第二个三角形
    {{-50.0f, 0.0f, -50.0f}, {0.5f, 0.5f, 0.5f}},
    {{ 50.0f, 0.0f,  50.0f}, {0.5f, 0.5f, 0.5f}},
    {{ 50.0f, 0.0f, -50.0f}, {0.5f, 0.5f, 0.5f}},
};


void Graphic::BeginFrame()
{
    m_spriteBatch->Begin();
    //设置渲染目标视图
    m_context->OMSetRenderTargets(
        1,
        &m_renderTargetView,
        m_depthStencilView
    );
    m_context->OMSetDepthStencilState(
        m_depthStencilState,
        0
    );

    // std::cout << "开始绘制" << std::endl;
    float color[4] ={1,1,1,1};
    //清理渲染目标图
    m_context->ClearRenderTargetView(
        m_renderTargetView,
        color
    );
    //清理深度
    m_context->ClearDepthStencilView(
        m_depthStencilView,
        D3D11_CLEAR_DEPTH,
        1.0f,
        0
    );

    Global::game->Draw();//调用game
    //绘制地板----------------------------------------------------
    MatrixBuffer matrixData;
    matrixData.world = XMMatrixTranspose(XMMatrixIdentity());
    matrixData.view =  XMMatrixTranspose(Global::camera->GetViewMatrix());
    matrixData.projection = XMMatrixTranspose(m_projection);
    //把矩阵传给 GPU
    m_context->UpdateSubresource(
        m_matrixBuffer,
        0,
        nullptr,
        &matrixData,
        0,
        0
    );
    //绘制地板
    DrawPrimitive3D(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
        vertices,
        std::size(vertices),
        sizeof(Vertex3)
    );
    //绘制文字
    DrawText2("Press F", 0, 500);


    //-----------------------------------------------------------
    // 世界矩阵
    // XMMATRIX world = Global::mesh->GetWorldMatrix();
    // // 矩阵数据
    // MatrixBuffer matrixData;
    // matrixData.world = XMMatrixTranspose(XMMatrixIdentity());
    // matrixData.view =  XMMatrixTranspose(Global::camera->GetViewMatrix());
    // matrixData.projection = XMMatrixTranspose(m_projection);
    //
    // // // 把矩阵传给 GPU
    // m_context->UpdateSubresource(
    //     m_matrixBuffer,
    //     0,
    //     nullptr,
    //     &matrixData,
    //     0,
    //     0
    // );
    // // VS 使用buff
    // m_context->VSSetConstantBuffers(
    //     0,
    //     1,
    //     &m_matrixBuffer
    // );
    //
    // // 绘制立方体
    // DrawPrimitive3D(
    //     D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
    //     Global::mesh->vertices,
    //     std::size(Global::mesh->vertices),
    //     sizeof(Vertex3)
    // );
}
void Graphic::DrawText2(std::string _text, float _x, float _y) {
    m_font->DrawString(
        m_spriteBatch,
        _text.c_str(),
        DirectX::XMFLOAT2(_x, _y)
    );
}


void Graphic::DrawPrimitive3D(D3D11_PRIMITIVE_TOPOLOGY topology,std::vector<Vertex3> vertices,UINT vertexCount,UINT vertexStride)
{
    if (vertices.empty() || vertexCount == 0 || vertexStride == 0)
        return;

    UINT vertexBufferSize = vertexCount * vertexStride;

    // ========================================
    // 1. 确保 Dynamic Vertex Buffer 足够大
    // ========================================
    if (!m_dynamicVertexBuffer ||
        m_dynamicVertexBufferSize < vertexBufferSize)
    {
        m_dynamicVertexBuffer.Reset();

        D3D11_BUFFER_DESC desc = {};
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.ByteWidth = vertexBufferSize;
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        HRESULT hr = m_device->CreateBuffer(
            &desc,
            nullptr,
            m_dynamicVertexBuffer.GetAddressOf()
        );

        if (FAILED(hr))
            return;

        m_dynamicVertexBufferSize = vertexBufferSize;
    }

    // ========================================
    // 2. 把 CPU 顶点数据写入 Dynamic Buffer
    // ========================================
    D3D11_MAPPED_SUBRESOURCE mapped = {};

    HRESULT hr = m_context->Map(
        m_dynamicVertexBuffer.Get(),
        0,
        D3D11_MAP_WRITE_DISCARD,
        0,
        &mapped
    );

    if (FAILED(hr))
        return;

    memcpy(
        mapped.pData,
        vertices.data(),
        vertexBufferSize
    );

    m_context->Unmap(
        m_dynamicVertexBuffer.Get(),
        0
    );

    // ========================================
    // 3. 设置 Input Layout
    // ========================================
    m_context->IASetInputLayout(
        m_inputLayout3
    );

    // ========================================
    // 4. 设置 Vertex Buffer
    // ========================================
    UINT stride = vertexStride;
    UINT offset = 0;

    ID3D11Buffer* buffer =
        m_dynamicVertexBuffer.Get();

    m_context->IASetVertexBuffers(
        0,
        1,
        &buffer,
        &stride,
        &offset
    );

    // ========================================
    // 5. 设置图元类型
    // ========================================
    m_context->IASetPrimitiveTopology(
        topology
    );

    // ========================================
    // 6. 设置 Vertex Shader
    // ========================================
    m_context->VSSetShader(
        m_vertexShader3,
        nullptr,
        0
    );

    // ========================================
    // 7. 设置 Pixel Shader
    // ========================================
    m_context->PSSetShader(
        m_pixelShader3,
        nullptr,
        0
    );

    // ========================================
    // 8. 设置矩阵 Constant Buffer
    // ========================================
    m_context->VSSetConstantBuffers(
        0,
        1,
        &m_matrixBuffer
    );

    // ========================================
    // 9. 绘制
    // ========================================
    m_context->Draw(
        vertexCount,
        0
    );
}

void Graphic::InitVertexFBX() {
    ID3DBlob* vertexShaderBlobFBX = nullptr;
    ID3DBlob* errorBlobFBX = nullptr;

    HRESULT hr = D3DCompileFromFile(
        L"VertexShaderFBX.hlsl",
        nullptr,
        nullptr,
        "main",
        "vs_5_0",
        0,
        0,
        &vertexShaderBlobFBX,
        &errorBlobFBX
    );

    if (FAILED(hr))
    {
        if (errorBlobFBX)
        {
            std::cerr << (char*)errorBlobFBX->GetBufferPointer() << std::endl;
            errorBlobFBX->Release();
        }

        return;
    }

    hr = m_device->CreateVertexShader(
        vertexShaderBlobFBX->GetBufferPointer(),
        vertexShaderBlobFBX->GetBufferSize(),
        nullptr,
        &m_vertexShaderFBX
    );

    if (FAILED(hr))
    {
        vertexShaderBlobFBX->Release();
        return;
    }


    // ==============================
    // FBX Input Layout
    // ==============================

    D3D11_INPUT_ELEMENT_DESC inputLayoutFBX[] =
    {
        {
            "POSITION",
            0,
            DXGI_FORMAT_R32G32B32_FLOAT,
            0,
            0,
            D3D11_INPUT_PER_VERTEX_DATA,
            0
        },

        {
            "NORMAL",
            0,
            DXGI_FORMAT_R32G32B32_FLOAT,
            0,
            12,
            D3D11_INPUT_PER_VERTEX_DATA,
            0
        },

        {
            "TEXCOORD",
            0,
            DXGI_FORMAT_R32G32_FLOAT,
            0,
            24,
            D3D11_INPUT_PER_VERTEX_DATA,
            0
        }
    };

    hr = m_device->CreateInputLayout(
        inputLayoutFBX,
        ARRAYSIZE(inputLayoutFBX),
        vertexShaderBlobFBX->GetBufferPointer(),
        vertexShaderBlobFBX->GetBufferSize(),
        &m_inputLayoutFBX
    );

    vertexShaderBlobFBX->Release();

    if (FAILED(hr))
        return;


    // ==============================
    // FBX Pixel Shader
    // ==============================

    ID3DBlob* pixelShaderBlobFBX = nullptr;
    ID3DBlob* errorBlobPSFBX = nullptr;

    hr = D3DCompileFromFile(
        L"PixelShaderFBX.hlsl",
        nullptr,
        nullptr,
        "main",
        "ps_5_0",
        0,
        0,
        &pixelShaderBlobFBX,
        &errorBlobPSFBX
    );

    if (FAILED(hr))
    {
        if (errorBlobPSFBX)
        {
            std::cerr << (char*)errorBlobPSFBX->GetBufferPointer() << std::endl;
            errorBlobPSFBX->Release();
        }

        return;
    }

    hr = m_device->CreatePixelShader(
        pixelShaderBlobFBX->GetBufferPointer(),
        pixelShaderBlobFBX->GetBufferSize(),
        nullptr,
        &m_pixelShaderFBX
    );

    pixelShaderBlobFBX->Release();

    if (FAILED(hr))
        return;
}

void Graphic::DrawPrimitiveIndexed(D3D11_PRIMITIVE_TOPOLOGY _topology, const std::vector<Vertex3fbx> &_vertices,const std::vector<uint32_t> &_indices) {
#pragma region FBX
    if (_vertices.empty() || _indices.empty())return;

    UINT vertexBufferSize =
        static_cast<UINT>(
            _vertices.size() * sizeof(Vertex3fbx)
        );

    UINT indexBufferSize =
        static_cast<UINT>(
            _indices.size() * sizeof(uint32_t)
        );

    // ========================================
    // 1. 确保 Dynamic Vertex Buffer 足够大
    // ========================================
    if (!m_dynamicVertexBuffer || m_dynamicVertexBufferSize < vertexBufferSize)
    {
        m_dynamicVertexBuffer.Reset();

        D3D11_BUFFER_DESC desc = {};
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.ByteWidth = vertexBufferSize;
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        HRESULT hr = m_device->CreateBuffer(
            &desc,
            nullptr,
            m_dynamicVertexBuffer.GetAddressOf()
        );

        if (FAILED(hr))
            return;

        m_dynamicVertexBufferSize = vertexBufferSize;
    }

    // ========================================
    // 2. 写入 Vertex Buffer
    // ========================================
    D3D11_MAPPED_SUBRESOURCE mapped = {};

    HRESULT hr = m_context->Map(
        m_dynamicVertexBuffer.Get(),
        0,
        D3D11_MAP_WRITE_DISCARD,
        0,
        &mapped
    );

    if (FAILED(hr))
        return;

    memcpy(
        mapped.pData,
        _vertices.data(),
        vertexBufferSize
    );

    m_context->Unmap(
        m_dynamicVertexBuffer.Get(),
        0
    );

    // ========================================
    // 3. 确保 Dynamic Index Buffer 足够大
    // ========================================
    if (!m_dynamicIndexBuffer || m_dynamicIndexBufferSize < indexBufferSize)
    {
        m_dynamicIndexBuffer.Reset();

        D3D11_BUFFER_DESC desc = {};
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.ByteWidth = indexBufferSize;
        desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        HRESULT hr = m_device->CreateBuffer(
            &desc,
            nullptr,
            m_dynamicIndexBuffer.GetAddressOf()
        );

        if (FAILED(hr))
            return;

        m_dynamicIndexBufferSize = indexBufferSize;
    }

    // ========================================
    // 4. 写入 Index Buffer
    // ========================================
    hr = m_context->Map(
        m_dynamicIndexBuffer.Get(),
        0,
        D3D11_MAP_WRITE_DISCARD,
        0,
        &mapped
    );

    if (FAILED(hr))
        return;

    memcpy(
        mapped.pData,
        _indices.data(),
        indexBufferSize
    );

    m_context->Unmap(
        m_dynamicIndexBuffer.Get(),
        0
    );
    // ========================================
    // 6. 设置 Vertex Buffer
    // ========================================
    UINT stride = sizeof(Vertex3fbx);
    UINT offset = 0;

    ID3D11Buffer* vertexBuffer = m_dynamicVertexBuffer.Get();

    m_context->IASetVertexBuffers(
        0,
        1,
        &vertexBuffer,
        &stride,
        &offset
    );
    // ========================================
    // 11. Constant Buffer
    // ========================================
    m_context->VSSetConstantBuffers(
        0,
        1,
        &m_matrixBuffer
    );
    // ========================================
    // 7. 设置 Index Buffer
    // ========================================
    m_context->IASetIndexBuffer(
        m_dynamicIndexBuffer.Get(),
        DXGI_FORMAT_R32_UINT,
        0
    );

    // ========================================
    // 8. 设置图元类型
    // ========================================
    m_context->IASetPrimitiveTopology(
        _topology
    );

#pragma endregion
    // ========================================
    // 5. 设置 Input Layout
    // ========================================
    m_context->IASetInputLayout(
        m_inputLayoutFBX
    );
    // ========================================
    // 9. Vertex Shader
    // ========================================
    m_context->VSSetShader(
        m_vertexShaderFBX,
        nullptr,
        0
    );
    // ========================================
    // 10. Pixel Shader
    // ========================================
    m_context->PSSetShader(
        m_pixelShader3,
        nullptr,
        0
    );
    // ========================================
    // 12. Indexed Draw
    // ========================================
    m_context->DrawIndexed(
        static_cast<UINT>(_indices.size()),
        0,
        0
    );
}



void Graphic::DrawPrimitiveUP(D3D11_PRIMITIVE_TOPOLOGY topology,const void* vertices,UINT vertexCount,UINT vertexStride) {
    if (!vertices || vertexCount == 0 || vertexStride == 0)return;
    // 设置输入布局
    m_context->IASetInputLayout(m_inputLayout3);
    m_context->VSSetShader(m_vertexShader3, nullptr, 0);
    m_context->PSSetShader(m_pixelShader3, nullptr, 0);

    UINT dataSize = vertexCount * vertexStride;
    // 如果当前 Buffer 不够大，就重新创建
    if (!m_dynamicVertexBuffer || m_dynamicVertexBufferSize < dataSize)
    {
        m_dynamicVertexBuffer.Reset();

        D3D11_BUFFER_DESC desc{};
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.ByteWidth = dataSize;
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        HRESULT hr = m_device->CreateBuffer(
            &desc,
            nullptr,
            &m_dynamicVertexBuffer
        );

        if (FAILED(hr))
            return;

        m_dynamicVertexBufferSize = dataSize;
    }

    // 把 CPU 顶点数据写入 Dynamic Vertex Buffer
    D3D11_MAPPED_SUBRESOURCE mapped{};

    HRESULT hr = m_context->Map(
        m_dynamicVertexBuffer.Get(),
        0,
        D3D11_MAP_WRITE_DISCARD,
        0,
        &mapped
    );

    if (FAILED(hr))return;

    memcpy(
        mapped.pData,
        vertices,
        dataSize
    );

    m_context->Unmap(
        m_dynamicVertexBuffer.Get(),
        0
    );

    // 设置 Vertex Buffer
    UINT stride = vertexStride;
    UINT offset = 0;

    ID3D11Buffer* buffer = m_dynamicVertexBuffer.Get();

    m_context->IASetVertexBuffers(
        0,
        1,
        &buffer,
        &stride,
        &offset
    );

    // 设置图元类型
    m_context->IASetPrimitiveTopology(topology);

    // 绘制
    m_context->Draw(vertexCount, 0);
}


void Graphic::CreateVertexBuffer()
{
    D3D11_BUFFER_DESC desc{};

    desc.ByteWidth =
        sizeof(Vertex) * 4;   // 四个顶点


    //允许CPU频繁修改
    desc.Usage =
        D3D11_USAGE_DYNAMIC;


    //这个Buffer作为顶点Buffer
    desc.BindFlags =
        D3D11_BIND_VERTEX_BUFFER;


    //允许CPU写入
    desc.CPUAccessFlags =
        D3D11_CPU_ACCESS_WRITE;


    HRESULT hr =
        m_device->CreateBuffer(
            &desc,
            nullptr,
            &m_vertexBuffer
        );


    if(FAILED(hr))
    {
        std::cout << "m_vertexBuffer加载失败" << std::endl;
    }
}
void Graphic::CreatePixelShader()
{
    ID3DBlob* psBlob = nullptr;

    HRESULT hr =
        D3DCompileFromFile(
            L"PixelShader.hlsl",
            nullptr,
            nullptr,
            "main",
            "ps_5_0",
            0,
            0,
            &psBlob,
            nullptr
        );


    if(FAILED(hr))
    {
        std::cout << "m_pixelShader加载失败" << std::endl;
        return;
    }


    hr = m_device->CreatePixelShader(
        psBlob->GetBufferPointer(),
        psBlob->GetBufferSize(),
        nullptr,
        &m_pixelShader
    );


    psBlob->Release();
}
void Graphic::CreateVertexShader() {
    ID3DBlob* vsBlob = nullptr;

    HRESULT hr = D3DCompileFromFile(
        L"VertexShader.hlsl",
        nullptr,
        nullptr,
        "main",
        "vs_5_0",
        0,
        0,
        &vsBlob,
        nullptr
    );
    if (FAILED(hr))
    {
        std::cout << "VertexShader编译失败" << std::endl;
        return;
    }
    // 创建 Vertex Shader
    hr = m_device->CreateVertexShader(
        vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize(),
        nullptr,
        &m_vertexShader
    );
    if (FAILED(hr))
    {
        std::cout << "VertexShader创建失败" << std::endl;
        vsBlob->Release();
        return;
    }
    // 创建 Input Layout
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        {
            "POSITION",
            0,
            DXGI_FORMAT_R32G32B32_FLOAT,
            0,
            0,
            D3D11_INPUT_PER_VERTEX_DATA,
            0
        },

        {
            "TEXCOORD",
            0,
            DXGI_FORMAT_R32G32_FLOAT,
            0,
            12,  // position占3个float = 12字节
            D3D11_INPUT_PER_VERTEX_DATA,
            0
        }
    };
    hr = m_device->CreateInputLayout(
        layout,
        2,
        vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize(),
        &m_inputLayout
    );
    vsBlob->Release();
    if (FAILED(hr))
    {
        std::cout << "InputLayout创建失败" << std::endl;
        return;
    }
    std::cout << "VertexShader创建成功" << std::endl;
}
void Graphic::CreateSampler()
{
    D3D11_SAMPLER_DESC desc{};


    // 纹理缩放时使用线性过滤
    desc.Filter =
        D3D11_FILTER_MIN_MAG_MIP_LINEAR;


    // UV超过0~1时，使用边缘颜色
    desc.AddressU =
        D3D11_TEXTURE_ADDRESS_CLAMP;

    desc.AddressV =
        D3D11_TEXTURE_ADDRESS_CLAMP;

    desc.AddressW =
        D3D11_TEXTURE_ADDRESS_CLAMP;


    // 不使用比较采样
    desc.ComparisonFunc =
        D3D11_COMPARISON_NEVER;


    // 允许最大各向异性
    desc.MaxLOD = D3D11_FLOAT32_MAX;


    HRESULT hr =
        m_device->CreateSamplerState(
            &desc,
            &m_sampler
        );


    if(FAILED(hr))
    {
        std::cout
            << "Sampler创建失败"
            << std::endl;
    }
    else
    {
        std::cout
            << "Sampler创建成功"
            << std::endl;
    }
}
void Graphic::CreateBlendState()
{
    D3D11_BLEND_DESC desc = {};

    desc.RenderTarget[0].BlendEnable = TRUE;

    desc.RenderTarget[0].SrcBlend =
        D3D11_BLEND_SRC_ALPHA;

    desc.RenderTarget[0].DestBlend =
        D3D11_BLEND_INV_SRC_ALPHA;

    desc.RenderTarget[0].BlendOp =
        D3D11_BLEND_OP_ADD;


    desc.RenderTarget[0].SrcBlendAlpha =
        D3D11_BLEND_ONE;

    desc.RenderTarget[0].DestBlendAlpha =
        D3D11_BLEND_ZERO;

    desc.RenderTarget[0].BlendOpAlpha =
        D3D11_BLEND_OP_ADD;


    desc.RenderTarget[0].RenderTargetWriteMask =
        D3D11_COLOR_WRITE_ENABLE_ALL;


    HRESULT hr = m_device->CreateBlendState(
        &desc,
        &m_alphaBlendState
    );


    assert(SUCCEEDED(hr));
}

void Graphic::InitlineVertex() {
    D3D11_BUFFER_DESC desc{};

    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.ByteWidth = sizeof(LineVertex) * 2;
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    HRESULT hr = m_device->CreateBuffer(
        &desc,
        nullptr,
        &m_lineVertexBuffer
    );

    if(FAILED(hr))
    {
        MessageBox(
            nullptr,
            "创建Line VertexBuffer失败",
            "Error",
            MB_OK
        );
    }
    //m_lineVertexShader 初始化
    ID3DBlob* blob = nullptr;
    D3DCompileFromFile(
        L"LineVS.hlsl",
        nullptr,
        nullptr,
        "main",
        "vs_5_0",
        0,
        0,
        &blob,
        nullptr
    );
    m_device->CreateVertexShader(
        blob->GetBufferPointer(),
        blob->GetBufferSize(),
        nullptr,
        &m_lineVertexShader
    );
    blob->Release();
    //m_linePixelShader 初始化
    blob = nullptr;
    D3DCompileFromFile(
        L"LinePS.hlsl",
        nullptr,
        nullptr,
        "main",
        "ps_5_0",
        0,
        0,
        &blob,
        nullptr
    );
    m_device->CreatePixelShader(
        blob->GetBufferPointer(),
        blob->GetBufferSize(),
        nullptr,
        &m_linePixelShader
    );
    blob->Release();
}

void Graphic::InitVertex3()
{
    HRESULT hr;

    // =========================
    // 编译 Vertex Shader
    // =========================

    ID3DBlob* vsBlob = nullptr;
    ID3DBlob* errorBlob = nullptr;

    hr = D3DCompileFromFile(
        L"ColorVS.hlsl",
        nullptr,
        nullptr,
        "main",
        "vs_5_0",
        0,
        0,
        &vsBlob,
        &errorBlob
    );

    if (FAILED(hr))
    {
        std::cout << "ColorVS.hlsl编译失败" << std::endl;
    }

    // =========================
    // 创建 Vertex Shader
    // =========================

    hr = m_device->CreateVertexShader(
        vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize(),
        nullptr,
        &m_vertexShader3
    );

    if (FAILED(hr))
    {
        std::cout << "m_vertexShader3创建失败" << std::endl;
    }

    // =========================
    // 创建 Input Layout
    // =========================

    D3D11_INPUT_ELEMENT_DESC layout3[] =
    {
        {
            "POSITION",
            0,
            DXGI_FORMAT_R32G32B32_FLOAT,
            0,
            0,
            D3D11_INPUT_PER_VERTEX_DATA,
            0
        },

        {
            "COLOR",
            0,
            DXGI_FORMAT_R32G32B32_FLOAT,
            0,
            12,
            D3D11_INPUT_PER_VERTEX_DATA,
            0
        }
    };

    hr = m_device->CreateInputLayout(
        layout3,
        2,
        vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize(),
        &m_inputLayout3
    );

    vsBlob->Release();

    if (FAILED(hr))
    {
        std::cout << "m_inputLayout3创建失败" << std::endl;
    }

    // =========================
    // 编译 Pixel Shader
    // =========================

    ID3DBlob* psBlob = nullptr;

    hr = D3DCompileFromFile(
        L"ColorPS.hlsl",
        nullptr,
        nullptr,
        "PS",
        "ps_5_0",
        0,
        0,
        &psBlob,
        &errorBlob
    );

    if (FAILED(hr))
    {
        std::cout << "ColorPS.hlsl编译失败" << std::endl;
    }
    // =========================
    // 创建 Pixel Shader
    // =========================

    hr = m_device->CreatePixelShader(
        psBlob->GetBufferPointer(),
        psBlob->GetBufferSize(),
        nullptr,
        &m_pixelShader3
    );

    psBlob->Release();

    if (FAILED(hr))
    {
        std::cout << "m_pixelShader3创建失败" << std::endl;
    }

    std::cout << "InitVertex3结束" << std::endl;
}

void Graphic::EndFrame()
{
    m_spriteBatch->End();
    assert(m_swapChain);
    //将缓冲区显示到屏幕
    m_swapChain->Present(
        1,
        0
    );
}

DirectX::XMFLOAT2 RotatePoint(float x,float y,float centerX,float centerY,float cosA,float sinA)
{
    // 移动到旋转中心
    float dx = x - centerX;
    float dy = y - centerY;
    // 旋转
    float rx = dx * cosA - dy * sinA;
    float ry = dx * sinA + dy * cosA;
    // 移回来
    return {rx + centerX,ry + centerY};
}

void Graphic::DrawTexture2(ID3D11ShaderResourceView* texture,float x,float y,float width,float height,float rotatX,float rotatY,float angleDeg)
{
    float left   = x;
    float right  = x + width;
    float top    = y;
    float bottom = y + height;

    float angle = DirectX::XMConvertToRadians(angleDeg);


    float cosA = cos(angle);
    float sinA = sin(angle);

    // 屏幕坐标 -> NDC
    float l = left   / 800  * 2.0f - 1.0f;
    float r = right  / 800  * 2.0f - 1.0f;

    float t = 1.0f - top    / 600 * 2.0f;
    float b = 1.0f - bottom / 600 * 2.0f;

    auto p1 = RotatePoint(
        left,
        top,
        rotatX,
        rotatY,
        cosA,
        sinA
    );
    auto p2 = RotatePoint(
        right,
        top,
        rotatX,
        rotatY,
        cosA,
        sinA
    );
    auto p3 = RotatePoint(
        left,
        bottom,
        rotatX,
        rotatY,
        cosA,
        sinA
    );
    auto p4 = RotatePoint(
        right,
        bottom,
        rotatX,
        rotatY,
        cosA,
        sinA
    );
    auto ToNDC = [](float x,float y)
    {
        return DirectX::XMFLOAT2(
            x / 800.0f * 2.0f - 1.0f,
            1.0f - y / 600.0f * 2.0f
        );
    };


    auto ndc1 = ToNDC(p1.x,p1.y);
    auto ndc2 = ToNDC(p2.x,p2.y);
    auto ndc3 = ToNDC(p3.x,p3.y);
    auto ndc4 = ToNDC(p4.x,p4.y);

    Vertex vertices[] =
    {
        {
            {ndc1.x,ndc1.y,0},
            {0,0}
        },

        {
            {ndc2.x,ndc2.y,0},
            {1,0}
        },

        {
            {ndc3.x,ndc3.y,0},
            {0,1}
        },

        {
            {ndc4.x,ndc4.y,0},
            {1,1}
        }
    };


    // Vertex vertices[] =
    // {
    //     {
    //         {l, t, 0},
    //         {0,0}
    //     },
    //
    //     {
    //         {r, t, 0},
    //         {1,0}
    //     },
    //
    //     {
    //         {l, b, 0},
    //         {0,1}
    //     },
    //
    //     {
    //         {r, b, 0},
    //         {1,1}
    //     }
    // };
    // std::cout << "1" << std::endl;
    // 更新顶点数据
    D3D11_MAPPED_SUBRESOURCE mapped{};
    //让 CPU 可以修改 m_vertexBuffer 里的顶点数据
    m_context->Map(
        m_vertexBuffer,
        0,
        D3D11_MAP_WRITE_DISCARD,
        0,
        &mapped
    );
    // std::cout << "2" << std::endl;
    //把准备好的顶点数据上传给GPU
    memcpy(
        mapped.pData,
        vertices,
        sizeof(vertices)
    );
    //结束 CPU 对 GPU Buffer 的访问
    m_context->Unmap(
        m_vertexBuffer,
        0
    );
    // std::cout << "3" << std::endl;
    // 设置输入布局
    m_context->IASetInputLayout(
        m_inputLayout
    );

    // 设置顶点buffer
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    //把顶点缓冲区（Vertex Buffer）绑定到 DirectX 11 的输入装配阶段（Input Assembler），让 GPU 知道从哪里读取顶点数据。
    m_context->IASetVertexBuffers(
        0,
        1,
        &m_vertexBuffer,
        &stride,
        &offset
    );

    // 三角形列表
    m_context->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
    );

    // shader 把顶点着色器（Vertex Shader）绑定到 Direct3D 11 的渲染管线中，让 GPU 在处理顶点时使用这个 Shader
    m_context->VSSetShader(
        m_vertexShader,
        nullptr,
        0
    );
    //把像素着色器（Pixel Shader）绑定到 DirectX 11 渲染管线，让 GPU 在生成每个像素颜色时使用这个 Shader。
    m_context->PSSetShader(
        m_pixelShader,
        nullptr,
        0
    );


    // 绑定图片
    m_context->PSSetShaderResources(
        0,
        1,
        &texture
    );
    //取样器
    m_context->PSSetSamplers(
        0,
        1,
        &m_sampler
    );
    //Blend State混合状态,用于实现透明、半透明、颜色叠加效果
    float blendFactor[4] = {0,0,0,0};
    m_context->OMSetBlendState(
        m_alphaBlendState,
        blendFactor,
        0xffffffff
    );
    // 绘制
    // std::cout<<"Draw"<<std::endl;
    m_context->Draw(
        4,
        0
    );

}

void Graphic::DrawLine2(XMFLOAT2 startPos, XMFLOAT2 endPos, XMFLOAT3 color) {
    // std::cout << "画线" << std::endl;
    LineVertex vertices[2];
    // 屏幕坐标转NDC
    auto ToNDC_X = [](float x)
    {
        return x / 800.0f * 2.0f - 1.0f;
    };
    auto ToNDC_Y = [](float y)
    {
        return 1.0f - y / 600.0f * 2.0f;
    };
    vertices[0].position =
    {
        ToNDC_X(startPos.x),
        ToNDC_Y(startPos.y),
        0.0f
    };
    vertices[0].color =
    {
        color.x,
        color.y,
        color.z,
        1.0f
    };
    vertices[1].position =
    {
        ToNDC_X(endPos.x),
        ToNDC_Y(endPos.y),
        0.0f
    };
    vertices[1].color =
    {
        color.x,
        color.y,
        color.z,
        1.0f
    };
    // 更新动态VertexBuffer
    D3D11_MAPPED_SUBRESOURCE mapped{};
    HRESULT hr = m_context->Map(
        m_lineVertexBuffer,
        0,
        D3D11_MAP_WRITE_DISCARD,
        0,
        &mapped
    );
    if(SUCCEEDED(hr))
    {
        memcpy(
            mapped.pData,
            vertices,
            sizeof(vertices)
        );

        m_context->Unmap(
            m_lineVertexBuffer,
            0
        );
    }
    UINT stride = sizeof(LineVertex);
    UINT offset = 0;
    m_context->IASetVertexBuffers(
        0,
        1,
        &m_lineVertexBuffer,
        &stride,
        &offset
    );
    m_context->IASetPrimitiveTopology(
        D3D11_PRIMITIVE_TOPOLOGY_LINELIST
    );
    m_context->VSSetShader(
        m_lineVertexShader,
        nullptr,
        0
    );
    m_context->PSSetShader(
        m_linePixelShader,
        nullptr,
        0
    );
    m_context->Draw(
        2,
        0
    );
}

ID3D11ShaderResourceView*
Graphic::LoadTexture(std::string _path)
{
    std::wstring wpath = StringToWString(_path);

    ID3D11ShaderResourceView* texture = nullptr;
    HRESULT hr = DirectX::CreateWICTextureFromFile(
        m_device,
        m_context,
        wpath.c_str(),
        nullptr,
        &texture
    );
    if (FAILED(hr))
    {
        std::cout << "资源加载错误" << std::endl;
        return nullptr;
    }
    return texture;
}

