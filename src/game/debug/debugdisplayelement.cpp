#ifdef FLAT_DEBUG

#include "debugdisplayelement.h"

namespace game
{
namespace debug
{

DebugDisplayElement::DebugDisplayElement()
{
}

DebugDisplayElement::~DebugDisplayElement()
{
}

DebugDisplayLine::DebugDisplayLine(const flat::Vector2& fromPos, const flat::Vector2& toPos, const flat::video::Color& fromColor, const flat::video::Color& toColor)
{
	m_vertices[0].pos = fromPos;
	m_vertices[0].color = fromColor;
	m_vertices[1].pos = toPos;
	m_vertices[1].color = toColor;
}

void DebugDisplayLine::draw(const flat::render::RenderSettings& renderSettings, const flat::Matrix4& viewMatrix) const
{
	const flat::video::Attribute positionAttribute = renderSettings.positionAttribute;
	const flat::video::Attribute colorAttribute = renderSettings.colorAttribute;

	glLineWidth(3);

	glEnableVertexAttribArray(positionAttribute);
	glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(DebugDisplayVertex), &m_vertices[0].pos);

	glEnableVertexAttribArray(colorAttribute);
	glVertexAttribPointer(colorAttribute, 4, GL_FLOAT, GL_FALSE, sizeof(DebugDisplayVertex), &m_vertices[0].color);

	glDrawArrays(GL_LINES, 0, NUM_VERTICES);

	glDisableVertexAttribArray(positionAttribute);
	glDisableVertexAttribArray(colorAttribute);
}

} // debug
} // game

#endif // FLAT_DEBUG


