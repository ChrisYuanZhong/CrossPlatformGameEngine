// Includes
//=========

#include "../Mesh.h"

#include <Engine/Logging/Logging.h>

void eae6320::Graphics::Mesh::DrawGeometry()
{
	// Draw the geometry
	{
		// Bind a specific vertex buffer to the device as a data source
		{
			EAE6320_ASSERT(vertexArrayId != 0);
			glBindVertexArray(vertexArrayId);
			EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
		}
		// The mode defines how to interpret multiple vertices as a single "primitive";
		// a triangle list is defined
		// (meaning that every primitive is a triangle and will be defined by three vertices)
		constexpr GLenum mode = GL_TRIANGLES;
		// It's possible to start rendering primitives in the middle of the stream
		const GLvoid* const offset = 0;
		glDrawElements(mode, static_cast<GLsizei>(indexCount), GL_UNSIGNED_SHORT, offset);
		EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
		//// Render triangles from the currently-bound vertex buffer
		//{
		//	// The mode defines how to interpret multiple vertices as a single "primitive";
		//	// a triangle list is defined
		//	// (meaning that every primitive is a triangle and will be defined by three vertices)
		//	constexpr GLenum mode = GL_TRIANGLES;
		//	// As of this comment only a single triangle is drawn
		//	// (you will have to update this code in future assignments!)
		//	constexpr unsigned int triangleCount = 3;
		//	constexpr unsigned int vertexCountPerTriangle = 3;
		//	constexpr auto vertexCountToRender = triangleCount * vertexCountPerTriangle;
		//	// It's possible to start rendering primitives in the middle of the stream
		//	constexpr unsigned int indexOfFirstVertexToRender = 0;
		//	glDrawArrays(mode, indexOfFirstVertexToRender, vertexCountToRender);
		//	EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
		//}
	}
}

void eae6320::Graphics::Mesh::CleanUp(eae6320::cResult& result)
{
	//if (vertexArrayId != 0)
	//{
	//	// Make sure that the vertex array isn't bound
	//	{
	//		// Unbind the vertex array
	//		glBindVertexArray(0);
	//		const auto errorCode = glGetError();
	//		if (errorCode != GL_NO_ERROR)
	//		{
	//			if (result)
	//			{
	//				result = Results::Failure;
	//			}
	//			EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
	//			Logging::OutputError("OpenGL failed to unbind all vertex arrays before cleaning up geometry: %s",
	//				reinterpret_cast<const char*>(gluErrorString(errorCode)));
	//		}
	//	}
	//	constexpr GLsizei arrayCount = 1;
	//	glDeleteVertexArrays(arrayCount, &vertexArrayId);
	//	const auto errorCode = glGetError();
	//	if (errorCode != GL_NO_ERROR)
	//	{
	//		if (result)
	//		{
	//			result = Results::Failure;
	//		}
	//		EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
	//		Logging::OutputError("OpenGL failed to delete the vertex array: %s",
	//			reinterpret_cast<const char*>(gluErrorString(errorCode)));
	//	}
	//	vertexArrayId = 0;
	//}
	//if (vertexBufferId != 0)
	//{
	//	constexpr GLsizei bufferCount = 1;
	//	glDeleteBuffers(bufferCount, &vertexBufferId);
	//	const auto errorCode = glGetError();
	//	if (errorCode != GL_NO_ERROR)
	//	{
	//		if (result)
	//		{
	//			result = Results::Failure;
	//		}
	//		EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
	//		Logging::OutputError("OpenGL failed to delete the vertex buffer: %s",
	//			reinterpret_cast<const char*>(gluErrorString(errorCode)));
	//	}
	//	vertexBufferId = 0;
	//}
	//if (indexBufferId != 0)
	//{
	//	constexpr GLsizei bufferCount = 1;
	//	glDeleteBuffers(bufferCount, &indexBufferId);
	//	const auto errorCode = glGetError();
	//	if (errorCode != GL_NO_ERROR)
	//	{
	//		if (result)
	//		{
	//			result = Results::Failure;
	//		}
	//		EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
	//		Logging::OutputError("OpenGL failed to delete the index buffer: %s",
	//							reinterpret_cast<const char*>(gluErrorString(errorCode)));
	//	}
	//	indexBufferId = 0;
	//}
}

eae6320::cResult eae6320::Graphics::Mesh::InitializeGeometry(VertexFormats::sVertex_mesh* i_vertexData, uint16_t* i_indexData, const unsigned int i_vertexCount, const unsigned int i_indexCount)
{
	indexCount = i_indexCount;

	auto result = eae6320::Results::Success;

	// Create a vertex array object and make it active
	{
		constexpr GLsizei arrayCount = 1;
		glGenVertexArrays(arrayCount, &vertexArrayId);
		const auto errorCode = glGetError();
		if (errorCode == GL_NO_ERROR)
		{
			glBindVertexArray(vertexArrayId);
			const auto errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				result = eae6320::Results::Failure;
				EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				eae6320::Logging::OutputError("OpenGL failed to bind a new vertex array: %s",
					reinterpret_cast<const char*>(gluErrorString(errorCode)));
				return result;
			}
		}
		else
		{
			result = eae6320::Results::Failure;
			EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			eae6320::Logging::OutputError("OpenGL failed to get an unused vertex array ID: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
			return result;
		}
	}
	// Create a vertex buffer object and make it active
	{
		constexpr GLsizei bufferCount = 1;
		glGenBuffers(bufferCount, &vertexBufferId);
		const auto errorCode = glGetError();
		if (errorCode == GL_NO_ERROR)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
			const auto errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				result = eae6320::Results::Failure;
				EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				eae6320::Logging::OutputError("OpenGL failed to bind a new vertex buffer: %s",
					reinterpret_cast<const char*>(gluErrorString(errorCode)));
				return result;
			}
		}
		else
		{
			result = eae6320::Results::Failure;
			EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			eae6320::Logging::OutputError("OpenGL failed to get an unused vertex buffer ID: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
			return result;
		}
	}
	// Assign the data to the buffer
	{
		const unsigned int bufferSize = sizeof(i_vertexData[0]) * i_vertexCount;
		EAE6320_ASSERT(bufferSize <= std::numeric_limits<unsigned int>::max());
		glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(bufferSize), reinterpret_cast<GLvoid*>(i_vertexData),
			// In our class we won't ever read from the buffer
			GL_STATIC_DRAW);
		const auto errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			result = eae6320::Results::Failure;
			EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			eae6320::Logging::OutputError("OpenGL failed to allocate the vertex buffer: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
			return result;
		}
	}
	// Create an index buffer
	/*An index buffer's ID is a GLuint, just like a vertex buffer
	The way you create it is almost identical to a vertex buffer : It must be created when the vertex array object is bound(I would recommend putting the index buffer creation code immediately after the vertex buffer creation code).You generate a new buffer, bind it, and allocate / assign initial data.You can copy / paste the code to create the vertex buffer and then make the following changes :
	When you call glGenBuffers() to create the buffer use the index buffer ID instead of the vertex buffer ID
		When you call glBindBuffer() pass in the index buffer ID and use GL_ELEMENT_ARRAY_BUFFER
		When you call glBufferData() use GL_ELEMENT_ARRAY_BUFFER and pass in the size of the index buffer(how do you calculate this ? ) and the index data
		(Remember that each index should be a uint16_t)*/
	{
		constexpr GLsizei bufferCount = 1;
		glGenBuffers(bufferCount, &indexBufferId);
		const auto errorCode = glGetError();
		if (errorCode == GL_NO_ERROR)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
			const auto errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				result = eae6320::Results::Failure;
				EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				eae6320::Logging::OutputError("OpenGL failed to bind a new index buffer: %s",
					reinterpret_cast<const char*>(gluErrorString(errorCode)));
				return result;
			}
		}
		else
		{
			result = eae6320::Results::Failure;
			EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			eae6320::Logging::OutputError("OpenGL failed to get an unused index buffer ID: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
			return result;
		}

		//ConvertLeftHandedToRight(i_indexData);

		const unsigned int bufferSize = sizeof(i_indexData[0]) * indexCount;
		EAE6320_ASSERT(bufferSize <= std::numeric_limits<unsigned int>::max());
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(bufferSize), reinterpret_cast<GLvoid*>(i_indexData),
						// In our class we won't ever read from the buffer
						GL_STATIC_DRAW);
		const auto errorCode1 = glGetError();
		if (errorCode1 != GL_NO_ERROR)
		{
			result = eae6320::Results::Failure;
			EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode1)));
			eae6320::Logging::OutputError("OpenGL failed to allocate the index buffer: %s",
														 reinterpret_cast<const char*>(gluErrorString(errorCode1)));
			return result;
		}
	}
	// Initialize vertex format
	{
		// The "stride" defines how large a single vertex is in the stream of data
		// (or, said another way, how far apart each position element is)
		constexpr auto stride = static_cast<GLsizei>(sizeof(eae6320::Graphics::VertexFormats::sVertex_mesh));

		// Position (0)
		// 3 floats == 12 bytes
		// Offset = 0
		{
			constexpr GLuint vertexElementLocation = 0;
			constexpr GLint elementCount = 3;
			constexpr GLboolean notNormalized = GL_FALSE;	// The given floats should be used as-is
			glVertexAttribPointer(vertexElementLocation, elementCount, GL_FLOAT, notNormalized, stride,
				reinterpret_cast<GLvoid*>(offsetof(eae6320::Graphics::VertexFormats::sVertex_mesh, x)));
			const auto errorCode = glGetError();
			if (errorCode == GL_NO_ERROR)
			{
				glEnableVertexAttribArray(vertexElementLocation);
				const GLenum errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					result = eae6320::Results::Failure;
					EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					eae6320::Logging::OutputError("OpenGL failed to enable the POSITION vertex attribute at location %u: %s",
						vertexElementLocation, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					return result;
				}
			}
			else
			{
				result = eae6320::Results::Failure;
				EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				eae6320::Logging::OutputError("OpenGL failed to set the POSITION vertex attribute at location %u: %s",
					vertexElementLocation, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				return result;
			}
		}
	}

	return result;
}

void eae6320::Graphics::Mesh::ConvertLeftHandedToRight(uint16_t* const& i_indexData)
{
	for (unsigned int i = 0; i < indexCount; i += 3)
	{
		uint16_t temp = i_indexData[i];
		i_indexData[i] = i_indexData[i + 2];
		i_indexData[i + 2] = temp;
	}
}

//{
//	// Convert left-handed to right-handed
//	for (unsigned int i = 0; i + 2 < indexCount; i += 3)
//	{
//		std::swap(i_indexData[i + 1], i_indexData[i + 2]);
//	}
//}
