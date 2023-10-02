/*
	This file should be #included by all shaders
*/

// Version Information
//====================

#if defined( EAE6320_PLATFORM_GL )

	// GLSL shaders require the version to be #defined before anything else in the shader
	#version 420

	#define float4 vec4
	#define mul( a, b ) ( a * b )
	#define o_vertexPosition_projected gl_Position

#endif

// Constant Buffers
//=================

#if defined( EAE6320_PLATFORM_D3D )

	#define DeclareConstantBufferFrame( i_name, i_id ) cbuffer i_name : register( b##i_id )
	#define DeclareConstantBufferDrawCall( i_name, i_id ) cbuffer i_name : register( b##i_id )
	#define mat4 float4x4

#elif defined( EAE6320_PLATFORM_GL )

	#define DeclareConstantBufferFrame( i_name, i_id ) layout( std140, binding = i_id ) uniform i_name
	#define DeclareConstantBufferDrawCall( i_name, i_id ) layout( std140, binding = i_id ) uniform i_name
	#define float4x4 mat4
	#define float2 vec2

#endif

DeclareConstantBufferFrame( g_constantBuffer_frame, 0 )
{
	float4x4 g_transform_worldToCamera;
	float4x4 g_transform_cameraToProjected;

	float g_elapsedSecondCount_systemTime;
	float g_elapsedSecondCount_simulationTime;
	// For float4 alignment
	float2 g_padding;
};

DeclareConstantBufferDrawCall( g_constantBuffer_drawCall, 2 )
{
	float4x4 g_transform_localToWorld;
};
