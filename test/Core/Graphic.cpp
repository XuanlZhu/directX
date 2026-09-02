#include "Graphic.h"

#include <iostream>
#include <d3dcompiler.h>

#include "../Global.h"


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
    test_texture = LoadTexture("PNG/first3.png");
    texture1 = LoadTexture("PNG/role1.png");
    texture2 = LoadTexture("PNG/line.png");
    texture3 = LoadTexture("PNG/grad.png");
    number = 0;
    return true;
}


void Graphic::BeginFrame(float r,float g,float b)
{
    // number += 1;
    // std::cout << number << std::endl;
    // assert(m_context);
    // assert(m_renderTargetView);
    //设置渲染目标视图
    m_context->OMSetRenderTargets(
        1,
        &m_renderTargetView,
        nullptr
    );


    float color[4] ={r,g,b,1.0f};
    //清理渲染目标图
    m_context->ClearRenderTargetView(
        m_renderTargetView,
        color
    );
    float time = GetTickCount() / 1000.0f;
    // 摆动速度
    float speed = 2.0f;
    // 最大角度
    float maxAngle = 45.0f;
    float angle = sin(time * speed)*maxAngle;

    DrawTexture(test_texture,0,0,800,600);
    DrawTexture(texture1,350,0,100,100);
    DrawTexture(texture2,378,77,5,50,378,77,angle);
    DrawTexture(texture3,365,120,32,19,378,77,angle);


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

void Graphic::EndFrame()
{
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

void Graphic::DrawTexture(ID3D11ShaderResourceView* texture,float x,float y,float width,float height,float rotatX,float rotatY,float angleDeg)
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