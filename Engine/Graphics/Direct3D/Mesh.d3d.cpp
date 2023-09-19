// Includes
//=========

#include "../Mesh.h"

#include "../sContext.h"

#include <Engine/Logging/Logging.h>

void eae6320::Graphics::Mesh::DrawGeometry()
{
	auto* const direct3dImmediateContext = eae6320::Graphics::sContext::g_context.direct3dImmediateContext;
	EAE6320_ASSERT(direct3dImmediateContext);

	// Draw the geometry
	{
		// Bind a specific vertex buffer to the device as a data source
		{
			EAE6320_ASSERT(vertexBuffer != nullptr);
			constexpr unsigned int startingSlot = 0;
			constexpr unsigned int vertexBufferCount = 1;
			// The "stride" defines how large a single vertex is in the stream of data
			constexpr unsigned int bufferStride = sizeof(VertexFormats::sVertex_mesh);
			// It's possible to start streaming data in the middle of a vertex buffer
			constexpr unsigned int bufferOffset = 0;
			direct3dImmediateContext->IASetVertexBuffers(startingSlot, vertexBufferCount, &vertexBuffer, &bufferStride, &bufferOffset);
		}
		// Specify what kind of data the vertex buffer holds
		{
			// Bind the vertex format (which defines how to interpret a single vertex)
			{
				EAE6320_ASSERT(s_vertexFormat != nullptr);
				s_vertexFormat->Bind();
			}
			// Set the topology (which defines how to interpret multiple vertices as a single "primitive";
			// the vertex buffer was defined as a triangle list
			// (meaning that every primitive is a triangle and will be defined by three vertices)
			direct3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}

		EAE6320_ASSERT(indexBuffer);
		constexpr DXGI_FORMAT indexFormat = DXGI_FORMAT_R16_UINT;
		// The indices start at the beginning of the buffer
		constexpr unsigned int offset = 0;
		direct3dImmediateContext->IASetIndexBuffer(indexBuffer, indexFormat, offset);

		// It's possible to start rendering primitives in the middle of the stream
		constexpr unsigned int indexOfFirstIndexToUse = 0;
		constexpr unsigned int offsetToAddToEachIndex = 0;
		D3D11_BUFFER_DESC desc;
		indexBuffer->GetDesc(&desc);
		direct3dImmediateContext->DrawIndexed(static_cast<unsigned int>(indexCount), indexOfFirstIndexToUse, offsetToAddToEachIndex);

		//// Render triangles from the currently-bound vertex buffer
		//{
		//	constexpr unsigned int triangleCount = 3;
		//	constexpr unsigned int vertexCountPerTriangle = 3;
		//	constexpr auto vertexCountToRender = triangleCount * vertexCountPerTriangle;
		//	// It's possible to start rendering primitives in the middle of the stream
		//	constexpr unsigned int indexOfFirstVertexToRender = 0;
		//	direct3dImmediateContext->Draw(vertexCountToRender, indexOfFirstVertexToRender);
		//}
	}
}

eae6320::cResult eae6320::Graphics::Mesh::InitializeGeometry(VertexFormats::sVertex_mesh* const i_vertexData, uint16_t* const i_indexData, const unsigned int i_vertexCount, const unsigned int i_indexCount)
{
	indexCount = i_indexCount;

	auto result = eae6320::Results::Success;

	auto* const direct3dDevice = eae6320::Graphics::sContext::g_context.direct3dDevice;
	EAE6320_ASSERT(direct3dDevice);

	// Vertex Format
	{
		if (!(result = eae6320::Graphics::cVertexFormat::Load(eae6320::Graphics::eVertexType::Mesh, s_vertexFormat,
			"data/Shaders/Vertex/vertexInputLayout_mesh.shader")))
		{
			EAE6320_ASSERTF(false, "Can't initialize geometry without vertex format");
			return result;
		}
	}
	// Vertex Buffer
	{
		//constexpr unsigned int triangleCount = 3;
		//constexpr auto vertexCount = triangleCount * vertexCountPerTriangle;
		//eae6320::Graphics::VertexFormats::sVertex_mesh vertexData[vertexCount];
		//{
		//	// Direct3D is left-handed

		//	// Draw a house shape using three triangles:
		//	i_vertexData[0].x = -0.5f;
		//	i_vertexData[0].y = -0.5f;
		//	i_vertexData[0].z = 0.0f;

		//	i_vertexData[1].x = -0.5f;
		//	i_vertexData[1].y = 0.5f;
		//	i_vertexData[1].z = 0.0f;

		//	i_vertexData[2].x = 0.5f;
		//	i_vertexData[2].y = 0.5f;
		//	i_vertexData[2].z = 0.0f;

		//	i_vertexData[3].x = 0.5f;
		//	i_vertexData[3].y = -0.5f;
		//	i_vertexData[3].z = 0.0f;

		//	i_vertexData[4].x = 0.0f;
		//	i_vertexData[4].y = 1.0f;
		//	i_vertexData[4].z = 0.0f;
		//}
		const auto bufferSize = sizeof(i_vertexData[0]) * i_vertexCount;
		EAE6320_ASSERT(bufferSize <= std::numeric_limits<decltype(D3D11_BUFFER_DESC::ByteWidth)>::max());
		const auto bufferDescription = [bufferSize]
		{
			D3D11_BUFFER_DESC bufferDescription{};

			bufferDescription.ByteWidth = static_cast<unsigned int>(bufferSize);
			bufferDescription.Usage = D3D11_USAGE_IMMUTABLE;	// In our class the buffer will never change after it's been created
			bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bufferDescription.CPUAccessFlags = 0;	// No CPU access is necessary
			bufferDescription.MiscFlags = 0;
			bufferDescription.StructureByteStride = 0;	// Not used

			return bufferDescription;
		}();

		const auto initialData = [i_vertexData]
		{
			D3D11_SUBRESOURCE_DATA initialData{};

			initialData.pSysMem = i_vertexData;
			// (The other data members are ignored for non-texture buffers)

			return initialData;
		}();

		const auto result_create = direct3dDevice->CreateBuffer(&bufferDescription, &initialData, &vertexBuffer);
		if (FAILED(result_create))
		{
			result = eae6320::Results::Failure;
			EAE6320_ASSERTF(false, "3D object vertex buffer creation failed (HRESULT %#010x)", result_create);
			eae6320::Logging::OutputError("Direct3D failed to create a 3D object vertex buffer (HRESULT %#010x)", result_create);
			return result;
		}
	}
	// Create Index Buffer
	{
		//constexpr auto indexCount = sizeof(i_indexData) / sizeof(i_vertexData[0]);
		//constexpr unsigned int triangleCount = 3;
		//constexpr unsigned int vertexCountPerTriangle = 3;
		//constexpr auto vertexCount = triangleCount * vertexCountPerTriangle;
		//uint16_t indexData[vertexCount];
		//{
		//	indexData[0] = 0;
		//	indexData[1] = 1;
		//	indexData[2] = 2;
		//	indexData[3] = 0;
		//	indexData[4] = 2;
		//	indexData[5] = 3;
		//	indexData[6] = 1;
		//	indexData[7] = 4;
		//	indexData[8] = 2;
		//}
		const auto bufferSize = sizeof(i_indexData[0]) * i_indexCount;
		EAE6320_ASSERT(bufferSize <= std::numeric_limits<decltype(D3D11_BUFFER_DESC::ByteWidth)>::max());
		const auto bufferDescription = [bufferSize]
		{
			D3D11_BUFFER_DESC bufferDescription{};

			bufferDescription.ByteWidth = static_cast<unsigned int>(bufferSize);
			bufferDescription.Usage = D3D11_USAGE_IMMUTABLE;	// In our class the buffer will never change after it's been created
			bufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bufferDescription.CPUAccessFlags = 0;	// No CPU access is necessary
			bufferDescription.MiscFlags = 0;
			bufferDescription.StructureByteStride = 0;	// Not used

			return bufferDescription;
		}();

		const auto initialData = [i_indexData]
		{
			D3D11_SUBRESOURCE_DATA initialData{};

			initialData.pSysMem = i_indexData;
			// (The other data members are ignored for non-texture buffers)

			return initialData;
		}();

		const auto result_create = direct3dDevice->CreateBuffer(&bufferDescription, &initialData, &indexBuffer);
		if (FAILED(result_create))
		{
			result = eae6320::Results::Failure;
			EAE6320_ASSERTF(false, "3D object index buffer creation failed (HRESULT %#010x)", result_create);
			eae6320::Logging::OutputError("Direct3D failed to create a 3D object index buffer (HRESULT %#010x)", result_create);
			return result;
		}
	}

	return result;
}

void eae6320::Graphics::Mesh::CleanUp(eae6320::cResult& result)
{
	
	if (vertexBuffer)
	{
		vertexBuffer->Release();
		vertexBuffer = nullptr;
	}
	if (s_vertexFormat)
	{
		s_vertexFormat->DecrementReferenceCount();
		s_vertexFormat = nullptr;
	}
	if (indexBuffer)
	{
		indexBuffer->Release();
		indexBuffer = nullptr;
	}
}
