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

DebugDisplayLine::DebugDisplayLine(const flat::Vector2& fromPos, const flat::Vector2& toPos, const flat::video::Color& fromColor, const flat::video::Color& toColor, float lineWidth) :
	m_lineWidth(lineWidth)
{
	m_vertices[0].pos = fromPos;
	m_vertices[0].color = fromColor;
	m_vertices[1].pos = toPos;
	m_vertices[1].color = toColor;
}

void DebugDisplayLine::draw(const flat::render::RenderSettings& renderSettings) const
{
	const flat::video::Attribute positionAttribute = renderSettings.positionAttribute;
	const flat::video::Attribute colorAttribute = renderSettings.colorAttribute;

	glLineWidth(m_lineWidth);

	glEnableVertexAttribArray(positionAttribute);
	glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(DebugDisplayVertex), &m_vertices[0].pos);

	glEnableVertexAttribArray(colorAttribute);
	glVertexAttribPointer(colorAttribute, 4, GL_FLOAT, GL_FALSE, sizeof(DebugDisplayVertex), &m_vertices[0].color);

	glDrawArrays(GL_LINES, 0, NUM_VERTICES);

	glDisableVertexAttribArray(positionAttribute);
	glDisableVertexAttribArray(colorAttribute);
}

DebugDisplayText::DebugDisplayText(const flat::Vector2& pos, const std::string& text, const flat::video::Color& color, const flat::video::Color& backgroundColor, const std::shared_ptr<const flat::video::font::Font>& font) : flat::video::font::String(font),
	m_color(color),
	m_backgroundColor(backgroundColor)
{
	flat::translateBy(m_transform, pos);
	flat::scaleBy(m_transform, 0.5f);
	setText(text);
}

void DebugDisplayText::draw(const flat::render::RenderSettings& renderSettings) const
{
	renderSettings.textureUniform.set(getFont()->getAtlasId());
	renderSettings.modelMatrixUniform.set(m_transform);

	renderSettings.colorUniform.set(m_color);
	renderSettings.secondaryColorUniform.set(m_backgroundColor);

	glEnableVertexAttribArray(renderSettings.positionAttribute);
	glVertexAttribPointer(renderSettings.positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const float*>(&(getVertices()[0])));

	glEnableVertexAttribArray(renderSettings.uvAttribute);
	glVertexAttribPointer(renderSettings.uvAttribute, 2, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const float*>(&(getUv()[0])));

	glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(getUv().size()));

	glDisableVertexAttribArray(renderSettings.positionAttribute);
	glDisableVertexAttribArray(renderSettings.uvAttribute);
}

} // debug
} // game

#endif // FLAT_DEBUG

