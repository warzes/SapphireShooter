#include "stdafx.h"
#if SE_D3D11
#include "Profiler.h"
#include "D3D11Graphics.h"
#include "D3D11VertexBuffer.h"
#include "Engine/DebugNew.h"

VertexBuffer::VertexBuffer() :
    buffer(nullptr),
    numVertices(0),
    vertexSize(0),
    elementHash(0),
    usage(USAGE_DEFAULT)
{
}

VertexBuffer::~VertexBuffer()
{
    Release();
}

void VertexBuffer::Release()
{
    if (graphics)
    {
        for (size_t i = 0; i < MAX_VERTEX_STREAMS; ++i)
        {
            if (graphics->GetVertexBuffer(i) == this)
                graphics->SetVertexBuffer(i, 0);
        }
    }

    if (buffer)
    {
        ID3D11Buffer* d3dBuffer = (ID3D11Buffer*)buffer;
        d3dBuffer->Release();
        buffer = nullptr;
    }
}

bool VertexBuffer::SetData(size_t firstVertex, size_t numVertices_, const void* data)
{
    PROFILE(UpdateVertexBuffer);

    if (!data)
    {
        SE_LOG_ERROR("Null source data for updating vertex buffer");
        return false;
    }
    if (firstVertex + numVertices_ > numVertices)
    {
        SE_LOG_ERROR("Out of bounds range for updating vertex buffer");
        return false;
    }
    if (buffer && usage == USAGE_IMMUTABLE)
    {
        SE_LOG_ERROR("Can not update immutable vertex buffer");
        return false;
    }

    if (shadowData)
        memcpy(shadowData.Get() + firstVertex * vertexSize, data, numVertices_ * vertexSize);

    if (buffer)
    {
        ID3D11DeviceContext* d3dDeviceContext = (ID3D11DeviceContext*)graphics->D3DDeviceContext();

        if (usage == USAGE_DYNAMIC)
        {
            D3D11_MAPPED_SUBRESOURCE mappedData;
            mappedData.pData = nullptr;

            d3dDeviceContext->Map((ID3D11Buffer*)buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
            if (mappedData.pData)
            {
                memcpy((unsigned char*)mappedData.pData + firstVertex * vertexSize, data, numVertices_ * vertexSize);
                d3dDeviceContext->Unmap((ID3D11Buffer*)buffer, 0);
            }
            else
            {
                SE_LOG_ERROR("Failed to map vertex buffer for update");
                return false;
            }
        }
        else
        {
            D3D11_BOX destBox;
            destBox.left = (unsigned)(firstVertex * vertexSize);
            destBox.right = destBox.left + (unsigned)(numVertices_ * vertexSize);
            destBox.top = 0;
            destBox.bottom = 1;
            destBox.front = 0;
            destBox.back = 1;

            d3dDeviceContext->UpdateSubresource((ID3D11Buffer*)buffer, 0, &destBox, data, 0, 0);
        }
    }

    return true;
}

bool VertexBuffer::Create(const void* data)
{
    if (graphics && graphics->IsInitialized())
    {
        D3D11_BUFFER_DESC bufferDesc;
        D3D11_SUBRESOURCE_DATA initialData;
        memset(&bufferDesc, 0, sizeof bufferDesc);
        memset(&initialData, 0, sizeof initialData);
        initialData.pSysMem = data;

        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bufferDesc.CPUAccessFlags = (usage == USAGE_DYNAMIC) ? D3D11_CPU_ACCESS_WRITE : 0;
        bufferDesc.Usage = (D3D11_USAGE)usage;
        bufferDesc.ByteWidth = (unsigned)(numVertices * vertexSize);

        ID3D11Device* d3dDevice = (ID3D11Device*)graphics->D3DDevice();
        d3dDevice->CreateBuffer(&bufferDesc, data ? &initialData : nullptr, (ID3D11Buffer**)&buffer);

        if (!buffer)
        {
            SE_LOG_ERROR("Failed to create vertex buffer");
            return false;
        }
        else
            LOGDEBUGF("Created vertex buffer numVertices %u vertexSize %u", (unsigned)numVertices, (unsigned)vertexSize);
    }

    return true;
}

#endif // SE_D3D11