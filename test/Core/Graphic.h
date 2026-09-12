#pragma once

// #include <Windows.h>
#include <DirectXMath.h>
#include <string>
#include <vector>
#include <WICTextureLoader.h>
#include <wrl/client.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dxgi.lib")
namespace DirectX::DX11 {
    class SpriteBatch;
    class SpriteFont;
}

using namespace DirectX;

struct Vertex
{
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT2 uv;
};
struct LineVertex
{
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT4 color;
};
struct Vertex3
{
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 color;
};
struct MatrixBuffer
{
    DirectX::XMMATRIX world;
    DirectX::XMMATRIX view;
    DirectX::XMMATRIX projection;
};
struct Vertex3fbx
{
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 normal;
    DirectX::XMFLOAT2 texCoord;
};

class Graphic
{
public:
    ~Graphic();
    bool Initialize(HWND hWnd);
    void BeginFrame();
    void EndFrame();
    void DrawTexture2(ID3D11ShaderResourceView* texture,float x,float y,float width,float height,float rotatX=0,float rotatY=0,float angleDeg=0);
    ID3D11ShaderResourceView* LoadTexture(std::string path);
    void DrawLine2(XMFLOAT2 startPos,XMFLOAT2 endPos,XMFLOAT3 color);
    void CreateVertexBuffer();
    void CreatePixelShader();
    void CreateVertexShader();
    void CreateSampler();
    void CreateBlendState();
    void InitlineVertex();
    void InitVertex3();
    void InitVertexFBX();
    void DrawPrimitiveUP(D3D11_PRIMITIVE_TOPOLOGY topology,const void* vertices,UINT vertexCount,UINT vertexStride);
    void DrawPrimitive3D(D3D11_PRIMITIVE_TOPOLOGY topology,std::vector<Vertex3> vertices,UINT vertexCount,UINT vertexStride);
    void DrawPrimitiveIndexed(D3D11_PRIMITIVE_TOPOLOGY _topology,const std::vector<Vertex3fbx>& _vertices,const std::vector<uint32_t>& _indices);
    void DrawText2(std::string _text,float _x,float _y);
    void LoadFBXTexture(std::wstring _path);//加载贴图

    Microsoft::WRL::ComPtr<ID3D11Buffer> m_dynamicVertexBuffer;
    UINT m_dynamicVertexBufferSize = 0;

    HWND m_hWnd = nullptr;
    IDXGISwapChain* m_swapChain = nullptr;//交换链
    ID3D11Device* m_device = nullptr;//设备
    ID3D11DeviceContext* m_context = nullptr;//设备上下文
    ID3D11RenderTargetView* m_renderTargetView = nullptr;//渲染目标视图

    ID3D11Buffer* m_vertexBuffer = nullptr;//顶点缓冲区
    ID3D11InputLayout* m_inputLayout = nullptr;//输入布局
    ID3D11VertexShader* m_vertexShader = nullptr;//顶点Shader
    ID3D11PixelShader* m_pixelShader = nullptr;//着色器
    ID3D11SamplerState* m_sampler = nullptr;//取样器
    ID3D11BlendState* m_alphaBlendState = nullptr;// Alpha混合状态
    ID3D11Buffer* m_lineVertexBuffer= nullptr;//直线缓冲区
    ID3D11VertexShader* m_lineVertexShader = nullptr;
    ID3D11PixelShader* m_linePixelShader = nullptr;

    ID3D11InputLayout* m_inputLayout3 = nullptr;//输入布局
    ID3D11VertexShader* m_vertexShader3 = nullptr;//顶点Shader
    ID3D11PixelShader* m_pixelShader3 = nullptr;//着色器

    DirectX::XMMATRIX m_projection = DirectX::XMMatrixIdentity();//投影矩阵
    ID3D11Buffer* m_matrixBuffer = nullptr;//矩阵buffer
    ID3D11DepthStencilView* m_depthStencilView = nullptr;//深度视图
    ID3D11DepthStencilState* m_depthStencilState = nullptr;//深度状态
    //
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_dynamicIndexBuffer;
    UINT m_dynamicIndexBufferSize = 0;

    //文字
    SpriteBatch* m_spriteBatch;
    SpriteFont* m_font;
    //新布局
    ID3D11InputLayout* m_inputLayoutFBX = nullptr;
    ID3D11VertexShader* m_vertexShaderFBX = nullptr;//顶点着色器
    ID3D11PixelShader* m_pixelShaderFBX = nullptr;//像素着色器
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureFBX;//贴图
    Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samplerStateFBX;//取样
};