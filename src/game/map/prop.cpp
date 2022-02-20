#include "prop.h"
#include "tile.h"
#include "map.h"

namespace game::map
{

flat::render::BaseSprite* Prop::getSprite()
{
	return &m_sprite;
}

flat::render::Mesh* Prop::getMesh()
{
	return &m_mesh;
}

const flat::render::ProgramSettings& Prop::getProgramSettings() const
{
	return Tile::getTileProgramSettings();
}

void Prop::updateWorldSpaceAABB(const flat::Vector3& position)
{
	m_worldSpaceAABB.min = position - flat::Vector3(0.5f, 0.5f, 0.f);
	m_worldSpaceAABB.max = position + flat::Vector3(0.5f, 0.5f, 100.f);
}

void Prop::initializeMesh(const Map& map, const std::shared_ptr<const flat::video::Texture>& texture)
{
	m_mesh.clearVertices();

	m_mesh.setTexture(texture);

	const flat::Vector2& textureSize = texture->getSize();
	flat::Vector2 origin(textureSize.x / 2.f, textureSize.y - textureSize.x / 4.f);

	const float propHeight3d = (textureSize.y - map.getXAxis().y) / -map.getZAxis().y;
	FLAT_ASSERT(propHeight3d >= 0.f);

	const float propMeshBottom = 0.01f; // put the bottom a bit over the ground to avoid z fighting

	/*
	
	0 - 1
	| / |
	2 - 3
	 \ /
	  4
	
	*/

	const flat::Vector2 uv0(0.f, 0.f);
	const flat::Vector2 uv1(1.f, 0.f);
	const flat::Vector2 uv2(0.f, 1.f - map.getYAxis().y / textureSize.y);
	const flat::Vector2 uv3(1.f, 1.f - map.getXAxis().y / textureSize.y);
	const flat::Vector2 uv4(-map.getXAxis().x / textureSize.x, 1.f);

	// vertical part
	m_mesh.addVertex(flat::Vector3(0.5f, -0.5f, propHeight3d), uv0);
	m_mesh.addVertex(flat::Vector3(-0.5f, 0.5f, propHeight3d), uv1);
	m_mesh.addVertex(flat::Vector3(0.5f, -0.5f, propMeshBottom), uv2);

	m_mesh.addVertex(flat::Vector3(-0.5f, 0.5f, propMeshBottom), uv3);
	m_mesh.addVertex(flat::Vector3(-0.5f, 0.5f, propHeight3d), uv1);
	m_mesh.addVertex(flat::Vector3(0.5f, -0.5f, propMeshBottom), uv2);

	// bottom horizontal part
	m_mesh.addVertex(flat::Vector3(-0.5f, 0.5f, propMeshBottom), uv3);
	m_mesh.addVertex(flat::Vector3( 0.5f, 0.5f, propMeshBottom), uv4);
	m_mesh.addVertex(flat::Vector3(0.5f, -0.5f, propMeshBottom), uv2);
}

} // game::map
