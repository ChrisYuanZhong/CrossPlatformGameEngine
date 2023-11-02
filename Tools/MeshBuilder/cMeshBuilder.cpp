// Includes
//=========

#include "cMeshBuilder.h"

#include <Tools/AssetBuildLibrary/Functions.h>
#include <Engine/Platform/Platform.h>
#include <iostream>
#include <fstream>

// Inherited Implementation
//=========================

// Build
//------

eae6320::cResult eae6320::Assets::cMeshBuilder::Build(const std::vector<std::string>& i_arguments)
{
	auto result = Results::Success;

	Graphics::VertexFormats::sVertex_mesh* vertexData = nullptr;
	uint16_t* indexData = nullptr;
	unsigned int vertexCount;
	unsigned int indexCount;

	// Create a new Lua state
	lua_State* luaState = nullptr;
	eae6320::cScopeGuard scopeGuard_onExit([&luaState]
		{
			if (luaState)
			{
				// If I haven't made any mistakes
				// there shouldn't be anything on the stack
				// regardless of any errors
				EAE6320_ASSERT(lua_gettop(luaState) == 0);

				lua_close(luaState);
				luaState = nullptr;
			}
		});
	{
		luaState = luaL_newstate();
		if (!luaState)
		{
			result = eae6320::Results::OutOfMemory;
			std::cerr << "Failed to create a new Lua state" << std::endl;
			return result;
		}
	}

	// Load the asset file as a "chunk",
	// meaning there will be a callable function at the top of the stack
	const auto stackTopBeforeLoad = lua_gettop(luaState);
	{
		const auto luaResult = luaL_loadfile(luaState, m_path_source);
		if (luaResult != LUA_OK)
		{
			result = eae6320::Results::Failure;
			std::cerr << lua_tostring(luaState, -1) << std::endl;
			// Pop the error message
			lua_pop(luaState, 1);
			return result;
		}
	}
	// Execute the "chunk", which should load the asset
	// into a table at the top of the stack
	{
		constexpr int argumentCount = 0;
		constexpr int returnValueCount = LUA_MULTRET;	// Return _everything_ that the file returns
		constexpr int noMessageHandler = 0;
		const auto luaResult = lua_pcall(luaState, argumentCount, returnValueCount, noMessageHandler);
		if (luaResult == LUA_OK)
		{
			// A well-behaved asset file will only return a single value
			const auto returnedValueCount = lua_gettop(luaState) - stackTopBeforeLoad;
			if (returnedValueCount == 1)
			{
				// A correct asset file _must_ return a table
				EAE6320_ASSERT(lua_istable(luaState, -1));
				if (!lua_istable(luaState, -1))
				{
					result = eae6320::Results::InvalidFile;
					std::cerr << "Asset files must return a table (instead of a " <<
						luaL_typename(luaState, -1) << ")" << std::endl;
					// Pop the returned non-table value
					lua_pop(luaState, 1);
					return result;
				}
			}
			else
			{
				result = eae6320::Results::InvalidFile;
				std::cerr << "Asset files must return a single table (instead of " <<
					returnedValueCount << " values)" << std::endl;
				// Pop every value that was returned
				lua_pop(luaState, returnedValueCount);
				return result;
			}
		}
		else
		{
			result = eae6320::Results::InvalidFile;
			std::cerr << lua_tostring(luaState, -1) << std::endl;
			// Pop the error message
			lua_pop(luaState, 1);
			return result;
		}
	}

	// If this code is reached the asset file was loaded successfully,
	// and its table is now at index -1
	eae6320::cScopeGuard scopeGuard_popAssetTable([luaState]
		{
			lua_pop(luaState, 1);
		});

	// Read vertex values
	if (!(result = LoadTableValues_vertices(*luaState, vertexData, vertexCount)))
	{
		return result;
	}

	// Read index values
	if (!(result = LoadTableValues_indices(*luaState, indexData, indexCount)))
	{
		return result;
	}

	eae6320::cScopeGuard scopeGuard_releaseArrays([&vertexData, &indexData]
		{
			if (vertexData)
			{
				delete[] vertexData;
				vertexData = nullptr;
			}
			if (indexData)
			{
				delete[] indexData;
				indexData = nullptr;
			}
		});

	// Write to binary file
	if (!(result = WriteToBinaryFile(vertexData, indexData, vertexCount, indexCount, m_path_target)))
	{
		return result;
	}

	return result;

	////Copy the source file to the target file
	//{
	//	std::string errorMessage;
	//	if (!Platform::CopyFile(m_path_source, m_path_target, false, true, &errorMessage))
	//	{
	//		OutputErrorMessageWithFileInfo(m_path_source, errorMessage.c_str());
	//		return Results::Failure;
	//	}
	//	return Results::Success;
	//}
}

eae6320::cResult eae6320::Assets::cMeshBuilder::WriteToBinaryFile(Graphics::VertexFormats::sVertex_mesh* i_vertexData, uint16_t* i_indexData, const unsigned int i_vertexCount, const unsigned int i_indexCount, const char* const i_path)
{
	auto result = Results::Success;

	std::ofstream outfile(i_path, std::ofstream::binary);

	if (outfile.is_open())
	{
		// Write vertex count
		outfile.write(reinterpret_cast<const char*>(&i_vertexCount), sizeof(uint32_t));
		// Write vertex data
		outfile.write(reinterpret_cast<const char*>(i_vertexData), sizeof(Graphics::VertexFormats::sVertex_mesh) * i_vertexCount);
		// Write index count
		outfile.write(reinterpret_cast<const char*>(&i_indexCount), sizeof(uint32_t));
		// Write index data
		outfile.write(reinterpret_cast<const char*>(i_indexData), sizeof(uint16_t) * i_indexCount);

		outfile.close();
	}
	else
	{
		result = Results::Failure;
		std::cerr << "Failed to open the target file \"" << i_path << "\" for writing" << std::endl;
		return result;
	}

	return result;
}

eae6320::cResult eae6320::Assets::cMeshBuilder::LoadTableValues_vertices(lua_State& io_luaState, Graphics::VertexFormats::sVertex_mesh*& i_vertexData, unsigned int& i_vertexCount)
{
	auto result = eae6320::Results::Success;

	// Right now the asset table is at -1.
	// After the following table operation it will be at -2
	// and the "vertices" table will be at -1:
	constexpr auto* const key = "vertices";
	lua_pushstring(&io_luaState, key);
	lua_gettable(&io_luaState, -2);
	// It can be hard to remember where the stack is at
	// and how many values to pop.
	// There are two ways that I suggest making it easier to keep track of this:
	//	1) Use scope guards to pop things automatically
	//	2) Call a different function when you are at a new level
	// Right now we know that we have an original table at -2,
	// and a new one at -1,
	// and so we _know_ that we always have to pop at least _one_
	// value before leaving this function
	// (to make the original table be back to index -1).
	// We can create a scope guard immediately as soon as the new table has been pushed
	// to guarantee that it will be popped when we are done with it:
	eae6320::cScopeGuard scopeGuard_popVertices([&io_luaState]
		{
			lua_pop(&io_luaState, 1);
		});
	// Additionally, I try not to do any further stack manipulation in this function
	// and call other functions that assume the "vertices" table is at -1
	// but don't know or care about the rest of the stack
	if (lua_istable(&io_luaState, -1))
	{
		if (!(result = LoadTableValues_vertices_vertex(io_luaState, i_vertexData, i_vertexCount)))
		{
			return result;
		}
	}
	else
	{
		result = eae6320::Results::InvalidFile;
		std::cerr << "The value at \"" << key << "\" must be a table "
			"(instead of a " << luaL_typename(&io_luaState, -1) << ")" << std::endl;
		return result;
	}

	return result;
}

eae6320::cResult eae6320::Assets::cMeshBuilder::LoadTableValues_vertices_vertex(lua_State& io_luaState, Graphics::VertexFormats::sVertex_mesh*& i_vertexData, unsigned int& i_vertexCount)
{
	// Right now the asset table is at -2
	// and the vertices table is at -1.
	// NOTE, however, that it doesn't matter to me in this function
	// that the asset table is at -2.
	// Because I've carefully called a new function for every "stack level"
	// The only thing I care about is that the vertices table that I care about
	// is at the top of the stack.
	// As long as I make sure that when I leave this function it is _still_
	// at -1 then it doesn't matter to me at all what is on the stack below it.

	auto result = eae6320::Results::Success;

	Graphics::VertexFormats::sVertex_mesh* vertexData = nullptr;

	std::cout << "Iterating through every vertex:" << std::endl;
	// Rewrite this: i_vertexCount = luaL_len(&io_luaState, -1);
	i_vertexCount = static_cast<unsigned int>(luaL_len(&io_luaState, -1));
	vertexData = new Graphics::VertexFormats::sVertex_mesh[i_vertexCount];

	for (unsigned int i = 1; i <= i_vertexCount; ++i)
	{
		lua_pushinteger(&io_luaState, i);
		lua_gettable(&io_luaState, -2);
		eae6320::cScopeGuard scopeGuard_popVertex([&io_luaState]
			{
				lua_pop(&io_luaState, 1);
			});
		LoadTableValues_vertices_vertex_coordinates(io_luaState, vertexData[i - 1]);
	}

	i_vertexData = vertexData;

	return result;
}

eae6320::cResult eae6320::Assets::cMeshBuilder::LoadTableValues_vertices_vertex_coordinates(lua_State& io_luaState, Graphics::VertexFormats::sVertex_mesh& i_vertexData)
{
	auto result = eae6320::Results::Success;

	std::cout << "Iterating through every vertex's coordinate:" << std::endl;
	for (int i = 1; i <= 3; ++i)
	{
		lua_pushinteger(&io_luaState, i);
		lua_gettable(&io_luaState, -2);
		eae6320::cScopeGuard scopeGuard_popVertex([&io_luaState]
			{
				lua_pop(&io_luaState, 1);
			});

		switch (i)
		{
		case 1:
		{
			i_vertexData.x = static_cast<float>(lua_tonumber(&io_luaState, -1));
			break;
		}
		case 2:
		{
			i_vertexData.y = static_cast<float>(lua_tonumber(&io_luaState, -1));
			break;
		}
		case 3:
		{
			i_vertexData.z = static_cast<float>(lua_tonumber(&io_luaState, -1));
			break;
		}
		}
	}

	return result;
}

eae6320::cResult eae6320::Assets::cMeshBuilder::LoadTableValues_indices(lua_State& io_luaState, uint16_t*& i_indexData, unsigned int& i_indexCount)
{
	auto result = eae6320::Results::Success;

	// Right now the asset table is at -1.
	// After the following table operation it will be at -2
	// and the "indices" table will be at -1:
	constexpr auto* const key = "indices";
	lua_pushstring(&io_luaState, key);
	lua_gettable(&io_luaState, -2);
	// It can be hard to remember where the stack is at
	// and how many values to pop.
	// There are two ways that I suggest making it easier to keep track of this:
	//	1) Use scope guards to pop things automatically
	//	2) Call a different function when you are at a new level
	// Right now we know that we have an original table at -2,
	// and a new one at -1,
	// and so we _know_ that we always have to pop at least _one_
	// value before leaving this function
	// (to make the original table be back to index -1).
	// We can create a scope guard immediately as soon as the new table has been pushed
	// to guarantee that it will be popped when we are done with it:
	eae6320::cScopeGuard scopeGuard_popIndices([&io_luaState]
		{
			lua_pop(&io_luaState, 1);
		});
	// Additionally, I try not to do any further stack manipulation in this function
	// and call other functions that assume the "indices" table is at -1
	// but don't know or care about the rest of the stack
	if (lua_istable(&io_luaState, -1))
	{
		if (!(result = LoadTableValues_indices_index(io_luaState, i_indexData, i_indexCount)))
		{
			return result;
		}
	}
	else
	{
		result = eae6320::Results::InvalidFile;
		std::cerr << "The value at \"" << key << "\" must be a table "
			"(instead of a " << luaL_typename(&io_luaState, -1) << ")" << std::endl;
		return result;
	}

	return result;
}

eae6320::cResult eae6320::Assets::cMeshBuilder::LoadTableValues_indices_index(lua_State& io_luaState, uint16_t*& i_indexData, unsigned int& i_indexCount)
{
	// Right now the asset table is at -2
	// and the indices table is at -1.
	// NOTE, however, that it doesn't matter to me in this function
	// that the asset table is at -2.
	// Because I've carefully called a new function for every "stack level"
	// The only thing I care about is that the indices table that I care about
	// is at the top of the stack.
	// As long as I make sure that when I leave this function it is _still_
	// at -1 then it doesn't matter to me at all what is on the stack below it.

	auto result = eae6320::Results::Success;

	uint16_t* indexData = nullptr;

	std::cout << "Iterating through every index:" << std::endl;
	i_indexCount = static_cast<unsigned int>(luaL_len(&io_luaState, -1));
	indexData = new uint16_t[i_indexCount];

	for (unsigned int i = 1; i <= i_indexCount; ++i)
	{
		lua_pushinteger(&io_luaState, i);
		lua_gettable(&io_luaState, -2);
		eae6320::cScopeGuard scopeGuard_popIndex([&io_luaState]
			{
				lua_pop(&io_luaState, 1);
			});
		indexData[i - 1] = static_cast<uint16_t>(lua_tointeger(&io_luaState, -1));
	}

	i_indexData = indexData;

	return result;
}