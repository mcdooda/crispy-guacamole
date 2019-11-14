local Path = require 'data/scripts/path'

-- entities
local function getPreviewAnimation(spriteComponentTemplate, animationName)
    if animationName and spriteComponentTemplate.animations[animationName] then
        return spriteComponentTemplate.animations[animationName]
    end
    if spriteComponentTemplate.animations.move then
        return spriteComponentTemplate.animations.move
    end
    for animationName, animation in pairs(spriteComponentTemplate.animations) do
        if animation.line == 1 then
            return animation
        end
    end
end

local function setEntitySpriteInitBackgroundPosition(preview, spriteComponentTemplate, animation)
    local line = animation and animation.line or 1
    preview:setBackgroundPosition(
        0,
        (line - 1) / spriteComponentTemplate.size:y()
    )
end

local function startEntitySpriteAnimation(preview, spriteComponentTemplate, animation)
    local frameIndex = 0
    local y = (animation.line - 1) / spriteComponentTemplate.size:y()
    local timer = flat.Timer()
    timer:onEnd(function()
        frameIndex = (frameIndex + 1) % math.max(animation.numFrames, 1)
        local x = frameIndex / spriteComponentTemplate.size:x()
        preview:setBackgroundPosition(x, y)
    end)
    timer:start(animation.frameDuration, true)

    local function stopAnimation()
        timer:stop()
        setEntitySpriteInitBackgroundPosition(preview, spriteComponentTemplate, animation)
    end

    return stopAnimation
end

local function startEntitySpriteAnimationByName(preview, spriteComponentTemplate, animationName)
    local animation = getPreviewAnimation(spriteComponentTemplate, animationName)
    if animation then
        return startEntitySpriteAnimation(preview, spriteComponentTemplate, animation)
    end
end

local function entitySpritePreview(entityTemplateName, spriteComponentTemplate, animationName, loopForever, scale, showTooltip)
    local entityAtlasPath = Path.getEntityFilePath(entityTemplateName, 'atlas.png')
    local preview = Widget.makeImage(entityAtlasPath)
    preview:setBackgroundRepeat(Widget.BackgroundRepeat.REPEAT)
    local imageWidth, imageHeight = Image.getSize(entityAtlasPath)
    preview:setSize(
        imageWidth / spriteComponentTemplate.size:x(),
        imageHeight / spriteComponentTemplate.size:y()
    )
    setEntitySpriteInitBackgroundPosition(preview, spriteComponentTemplate, getPreviewAnimation(spriteComponentTemplate, animationName))
    if loopForever then
        startEntitySpriteAnimationByName(preview, spriteComponentTemplate, animationName)
    else
        local stopAnimation
        preview:mouseEnter(function()
            stopAnimation = startEntitySpriteAnimationByName(preview, spriteComponentTemplate, animationName)
        end)
        preview:mouseLeave(function()
            if stopAnimation then
                stopAnimation()
            end
        end)
    end
    if scale then
        local width, height = preview:getSize()
        preview:setSize(width * scale, height * scale)
        preview:setBackgroundSize(imageWidth * scale, imageHeight * scale)
    end
    if showTooltip then
        flat.ui.addTooltip(preview, entityTemplateName)
    end
    return preview
end

local function entityPreview(entityTemplateName, animationName, loopForever, scale, showTooltip)
    -- try sprite component first
    local spriteComponentTemplate = Path.requireComponentTemplateIfExists(entityTemplateName, 'sprite')
    if spriteComponentTemplate then
       return entitySpritePreview(entityTemplateName, spriteComponentTemplate, animationName, loopForever, scale, showTooltip)
    end

    -- none of them
    flat.ui.error('No preview for ' .. entityTemplateName)
    local unavailablePreview = Widget.makeFixedSize(10, 10)
    unavailablePreview:setBackgroundColor(0xFF00FFFF)
    return unavailablePreview
end

-- tiles
local function setTileSpriteInitBackgroundPosition(preview)
    preview:setBackgroundPosition(0, 0)
end

local function startTileSpriteAnimation(preview, tileTemplate, tileVariantIndex)
    local frameIndex = 0
    local animationWidth = tileTemplate.numFrames
    local animationHeight = #tileTemplate.probabilities
    local y = (tileVariantIndex - 1) / animationHeight
    local timer = flat.Timer()
    timer:onEnd(function()
        frameIndex = (frameIndex + 1) % math.max(animationWidth, 1)
        local x = frameIndex / animationWidth
        preview:setBackgroundPosition(x, y)
    end)
    timer:start(tileTemplate.frameDuration, true)

    local function stopAnimation()
        timer:stop()
        setTileSpriteInitBackgroundPosition(preview)
    end

    return stopAnimation
end

local function tilePreview(tileTemplateName, tileVariantIndex, loopForever, scale, showTooltip)
    local tileAtlasPath = Path.getTileFilePath(tileTemplateName, 'atlas.png')
    local preview = Widget.makeImage(tileAtlasPath)
    preview:setBackgroundRepeat(Widget.BackgroundRepeat.REPEAT)
    local imageWidth, imageHeight = Image.getSize(tileAtlasPath)
    local tileTemplate = Path.requireTileFile(tileTemplateName, 'tile')
    local animationWidth = tileTemplate.numFrames
    local animationHeight = #tileTemplate.probabilities
    preview:setSize(
        imageWidth / animationWidth,
        imageHeight / animationHeight
    )
    setTileSpriteInitBackgroundPosition(preview)
    if loopForever then
        startTileSpriteAnimation(preview, tileTemplate, tileVariantIndex)
    else
        local stopAnimation
        preview:mouseEnter(function()
            stopAnimation = startTileSpriteAnimation(preview, tileTemplate, tileVariantIndex)
        end)
        preview:mouseLeave(function()
            if stopAnimation then
                stopAnimation()
            end
        end)
    end
    if scale then
        local width, height = preview:getSize()
        preview:setSize(width * scale, height * scale)
        preview:setBackgroundSize(imageWidth * scale, imageHeight * scale)
    end
    if showTooltip then
        flat.ui.addTooltip(preview, tileTemplateName)
    end
    return preview
end

local function propPreview(propName, fileName, scale, showTooltip)
    local propTexturePath = Path.getPropFilePath(propName, fileName .. '.png')
    local preview = Widget.makeImage(propTexturePath)
    if scale then
        local w, h = preview:getSize()
        preview:setSize(w * scale, h * scale)
    end
    if showTooltip then
        flat.ui.addTooltip(preview, propName)
    end
    return preview
end

return {
    entity = entityPreview,
    sprite = entitySpritePreview,
    tile = tilePreview,
    prop = propPreview
}