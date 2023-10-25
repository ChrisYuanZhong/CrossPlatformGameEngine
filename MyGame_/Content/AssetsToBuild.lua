--[[
	This file lists every asset that must be built by the AssetBuildSystem
]]

return
{
	shaders =
	{
		{ path = "Shaders/Vertex/standard.shader", arguments = { "vertex" } },
		{ path = "Shaders/Fragment/standard.shader", arguments = { "fragment" } },
		{ path = "Shaders/Fragment/animatedColor1.shader", arguments = { "fragment" } },
		{ path = "Shaders/Fragment/animatedColor2.shader", arguments = { "fragment" } },

		{ path = "Shaders/Vertex/vertexInputLayout_mesh.shader", arguments = { "vertex" } },
	},

	meshes =
	{
		"Meshes/square.mesh",
		"Meshes/triangle.mesh",
		"Meshes/Plane.mesh",
	},
}
