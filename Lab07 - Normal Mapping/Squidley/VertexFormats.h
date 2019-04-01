#pragma once
#include "glm/glm.hpp"

// Vertices will always have Position and optionally may have any or all of the following: 
//        a color, a texture, a normal.
//    When we read or write a vertex, these various pieces will always occur in this order: 
//        Vertex, Color, Texture, Normal.
//    We may not need all of the 8 possible vertex combinations, but it not difficult 
//    to support them all if we do so in a structured manner

struct Position
{
	glm::vec3 position;
};

struct PColor
{
	glm::vec3 position;
	glm::vec3 color;
};

struct PText
{
	glm::vec3 position;
	glm::vec2 texture;
};

struct PNorm
{
	glm::vec3 position;
	glm::vec3 normal;
};

struct PCNorm
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec3 normal;
};

struct PTNorm
{
	glm::vec3 position;
	glm::vec2 texture;
	glm::vec3 normal;
};

struct PCText
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 texture;
};

struct PCTN
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 texture;
	glm::vec3 normal;
};

struct PTNTB
{
	glm::vec3 position;
	glm::vec2 texture;
	glm::vec3 normal;
	glm::vec3 tangent;
	glm::vec3 bitanget;
};

enum VertexFormats
{
	HasPosition = 1,
	HasColor = 2,
	HasTexture = 4,
	HasNormal = 8,
	HasTangent = 16,
	HasBiTangent = 32,
	PositionOnly = HasPosition,
	PositionColor = HasPosition | HasColor,
	PositionTexture = HasPosition | HasTexture,
	PositionNormal = HasPosition | HasNormal,
	PositionColorNormal = HasPosition | HasColor | HasNormal,
	PositionTextureNormal = HasPosition | HasTexture | HasNormal,
	PositionColorTexture = HasPosition | HasColor | HasTexture,
	PositionColorTextureNormal = HasPosition | HasColor | HasTexture | HasNormal,

	PositionTextureNormalTangentBiTangent = HasPosition | HasTexture | HasNormal | HasTangent | HasBiTangent,
};