#include <flat.h>
#include "tiletemplate.h"
#include "../game.h"
#include "../states/basemapstate.h"

namespace game
{
namespace map
{

TileTemplate::TileTemplate(const std::string& name, Game& game) :
	m_name(name)
{
	const mod::Mod& mod = game.getStateMachine().getState()->as<states::BaseMapState>().getMod();
	std::string path = mod.getTileTemplatePath(name);
	m_texture = game.video->getTexture(path + "atlas.png");
	loadTileConfig(game, path);
}

TileTemplate::~TileTemplate()
{

}

int TileTemplate::getRandomTileVariantIndex(Game& game) const
{
	FLAT_ASSERT(!m_tileVariantProbabilities.empty() && m_tileVariantProbabilitiesSum > 0.f);
	float random = game.random->nextFloat(0.f, m_tileVariantProbabilitiesSum);
	int randomIndex = 0;
	for (float probability : m_tileVariantProbabilities)
	{
		if (random <= probability)
		{
			break;
		}
		random -= probability;
		++randomIndex;
	}
	FLAT_ASSERT(0 <= randomIndex && randomIndex < m_tileVariantProbabilities.size());
	return randomIndex;
}

int TileTemplate::getRandomTileVariantIndex(Game & game, const std::vector<int>& tileVariantIndices) const
{
	FLAT_ASSERT(!m_tileVariantProbabilities.empty() && m_tileVariantProbabilitiesSum > 0.f);
	float tileVariantProbabilitiesSum = 0.f;
	for (int tileVariantIndex : tileVariantIndices)
	{
		tileVariantProbabilitiesSum += m_tileVariantProbabilities[tileVariantIndex];
	}
	float random = game.random->nextFloat(0.f, tileVariantProbabilitiesSum);
	int randomIndex = -1;
	for (int tileVariantIndex : tileVariantIndices)
	{
		const float probability = m_tileVariantProbabilities[tileVariantIndex];
		if (random <= probability)
		{
			randomIndex = tileVariantIndex;
			break;
		}
		random -= probability;
	}
	FLAT_ASSERT(0 <= randomIndex && randomIndex < m_tileVariantProbabilities.size());
	return randomIndex;
}

void TileTemplate::loadTileConfig(Game& game, const std::string& path)
{
	lua_State* L = game.lua->state;
	{
		FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);

		std::string tileConfigPath = path + "tile.lua";
		luaL_loadfile(L, tileConfigPath.c_str());
		lua_call(L, 0, 1);

		lua_getfield(L, -1, "probabilities");
		luaL_checktype(L, -1, LUA_TTABLE);

		size_t length = lua_rawlen(L, -1);
		m_tileVariantProbabilities.reserve(length);
		m_tileVariantProbabilitiesSum = 0.f;
		for (int i = 1; i <= length; ++i)
		{
			FLAT_LUA_EXPECT_STACK_GROWTH(L, 0);
			lua_rawgeti(L, -1, i);
			const float probability = static_cast<float>(luaL_checknumber(L, -1));
			luaL_argcheck(L, probability > 0.f, -1, "probability must be > 0");
			m_tileVariantProbabilities.push_back(probability);
			m_tileVariantProbabilitiesSum += probability;
			lua_pop(L, 1);
		}

		lua_getfield(L, -2, "frameDuration");
		m_frameDuration = static_cast<float>(luaL_checknumber(L, -1));

		lua_getfield(L, -3, "numFrames");
		m_numFrames = static_cast<int>(luaL_checkinteger(L, -1));

		lua_getfield(L, -4, "selectTile");
		m_selectTile.setIfNotNil(L, -1);

		lua_getfield(L, -5, "navigability");
		if (!lua_isnil(L, -1))
		{
			m_navigability = static_cast<Navigability>(luaL_checkinteger(L, -1));
		}
		else
		{
			m_navigability = Navigability::GROUND;
		}

		lua_pop(L, 6);
	}
}

} // map
} // game


