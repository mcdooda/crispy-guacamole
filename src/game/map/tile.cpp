#include "tile.h"
#include "map.h"
#include "prop.h"
#include "displaymanager.h"
#include "tiletemplate.h"

namespace game
{
namespace map
{

const flat::render::ProgramSettings* Tile::tileProgramSettings = nullptr;

Tile::Tile() :
	m_propIndex(PropIndex::INVALID_PROP)
{
	m_sprite.setColor(flat::video::Color::WHITE);
}

Tile::Tile(Tile&& tile) : MapObject(std::move(tile))
{
	m_sprite = std::move(tile.m_sprite);
	m_propIndex = tile.m_propIndex;
	tile.m_propIndex = PropIndex::INVALID_PROP;
}

Tile::~Tile()
{
	
}

Tile& Tile::operator=(Tile&& tile)
{
	MapObject::operator=(std::move(tile));
	m_sprite = std::move(tile.m_sprite);
	m_propIndex = tile.m_propIndex;
	tile.m_propIndex = PropIndex::INVALID_PROP;
	return *this;
}

void Tile::synchronizeSpriteTo(const Map& map, flat::render::SpriteSynchronizer& spriteSynchronizer)
{
	m_sprite.synchronizeTo(spriteSynchronizer);
	flat::Vector2 origin = flat::Vector2(-map.getXAxis().x, 0.f) + (map.getXAxis() + map.getYAxis()) / 2.f;
	m_sprite.setOrigin(origin);
	m_sprite.getAABB(m_spriteAABB);

	updateRenderHash();
}

void Tile::initializeMesh(const Map& map, const TileTemplate& tileTemplate, uint16_t tileTemplateVariantIndex)
{
	m_mesh.clearVertices();

	const std::shared_ptr<const flat::video::Texture>& texture = tileTemplate.getTexture();

	m_mesh.setTexture(texture);

	const float spriteWidth = texture->getSize().x;
	const float spriteHeight = texture->getSize().y;
	const float spriteTileHeight = spriteHeight / tileTemplate.getNumVariants();
	const float spriteTileWidth = spriteWidth / tileTemplate.getNumFrames();

	const float localMinU = 0.f;
	const float localMaxU = 1.f / tileTemplate.getNumFrames();
	const float localMinV = static_cast<float>(tileTemplateVariantIndex) / static_cast<float>(tileTemplate.getNumVariants());
	const float localMaxV = static_cast<float>(tileTemplateVariantIndex + 1) / static_cast<float>(tileTemplate.getNumVariants());

	const float tileHeight3d = (spriteTileHeight - map.getXAxis().y - map.getYAxis().y) / -map.getZAxis().y;
	FLAT_ASSERT(tileHeight3d >= 0.f);
	const float bottomZ = -tileHeight3d;

	/*
	
	/ 0 \
	1   2
	\ 3 /
	| | |
	4 | 5
	\ 6 /

	*/

	const flat::Vector2 uv0(localMinU - map.getXAxis().x / spriteWidth, localMinV);
	const flat::Vector2 uv1(localMinU, localMinV + map.getXAxis().y / spriteHeight);
	const flat::Vector2 uv2(localMaxU, localMinV + map.getYAxis().y / spriteHeight);
	const flat::Vector2 uv3(localMinU - map.getXAxis().x / spriteWidth, localMinV + (map.getXAxis().y + map.getYAxis().y) / spriteHeight);
	const flat::Vector2 uv4(uv1.x, localMaxV - map.getYAxis().y / spriteHeight);
	const flat::Vector2 uv5(uv2.x, localMaxV - map.getXAxis().y / spriteHeight);
	const flat::Vector2 uv6(uv3.x, localMaxV);

	// top
	m_mesh.addVertex(flat::Vector3(0.5f, 0.5f, 0.f), uv3);
	m_mesh.addVertex(flat::Vector3(0.5f, -0.5f, 0.f), uv1);
	m_mesh.addVertex(flat::Vector3(-0.5f, 0.5f, 0.f), uv2);

	m_mesh.addVertex(flat::Vector3(-0.5f, -0.5f, 0.f), uv0);
	m_mesh.addVertex(flat::Vector3(0.5f, -0.5f, 0.f), uv1);
	m_mesh.addVertex(flat::Vector3(-0.5f, 0.5f, 0.f), uv2);

	// left
	m_mesh.addVertex(flat::Vector3(0.5f, 0.5f, 0.f), uv3);
	m_mesh.addVertex(flat::Vector3(0.5f, -0.5f, 0.f), uv1);
	m_mesh.addVertex(flat::Vector3(0.5f, 0.5f, bottomZ), uv6);

	m_mesh.addVertex(flat::Vector3(0.5f, -0.5f, bottomZ), uv4);
	m_mesh.addVertex(flat::Vector3(0.5f, -0.5f, 0.f), uv1);
	m_mesh.addVertex(flat::Vector3(0.5f, 0.5f, bottomZ), uv6);

	// right
	m_mesh.addVertex(flat::Vector3(0.5f, 0.5f, 0.f), uv3);
	m_mesh.addVertex(flat::Vector3(-0.5f, 0.5f, 0.f), uv2);
	m_mesh.addVertex(flat::Vector3(0.5f, 0.5f, bottomZ), uv6);

	m_mesh.addVertex(flat::Vector3(-0.5f, 0.5f, bottomZ), uv5);
	m_mesh.addVertex(flat::Vector3(-0.5f, 0.5f, 0.f), uv2);
	m_mesh.addVertex(flat::Vector3(0.5f, 0.5f, bottomZ), uv6);
}

flat::render::BaseSprite* Tile::getSprite()
{
	FLAT_ASSERT(m_sprite.isSynchronized());
	return &m_sprite;
}

flat::render::Mesh* Tile::getMesh()
{
	return &m_mesh;
}

const flat::render::ProgramSettings& Tile::getProgramSettings() const
{
	return getTileProgramSettings();
}

void Tile::updateWorldSpaceAABB(const flat::Vector3& position)
{
	m_worldSpaceAABB.min = position + flat::Vector3(-0.5f, -0.5f, -100.f);
	m_worldSpaceAABB.max = position + flat::Vector3(0.5f, 0.5f, 0.f);
}

} // map
} // game
