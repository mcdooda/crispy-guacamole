local Path = require 'data/scripts/path'

local function getPreviewAnimation(spriteComponentTemplate)
    if spriteComponentTemplate.animations.move then
        return spriteComponentTemplate.animations.move
    end
    for animationName, animation in pairs(spriteComponentTemplate.animations) do
        if animation.line == 1 then
            return animation
        end
    end
end

local function setInitBackgroundPosition(preview, spriteComponentTemplate)
    local animation = getPreviewAnimation(spriteComponentTemplate)
    preview:setBackgroundPosition(
        0,
        (animation.line - 1) / spriteComponentTemplate.size[2]
    )
end

local function startEntitySpriteAnimation(preview, imageWidth, spriteComponentTemplate)
    local animation = getPreviewAnimation(spriteComponentTemplate)
    if animation then
        local frameIndex = 0
        local y = (animation.line - 1) / spriteComponentTemplate.size[2]
        local timer = Timer.start(
            animation.frameDuration,
            nil,
            function()
                frameIndex = (frameIndex + 1) % animation.numFrames
                local x = frameIndex / spriteComponentTemplate.size[1]
                preview:setBackgroundPosition(x, y)
            end,
            true
        )

        local function stopAnimation()
            timer:stop()
            setInitBackgroundPosition(preview, spriteComponentTemplate)
        end

        return stopAnimation
    end
end

local function entitySpritePreview(entityTemplateName, spriteComponentTemplate)
    local entityAtlasPath = Path.getEntityFilePath(entityTemplateName, 'atlas.png')
    local preview = Widget.makeImage(entityAtlasPath)
    preview:setBackgroundRepeat(Widget.BackgroundRepeat.REPEAT)
    local imageWidth, imageHeight = Image.getSize(entityAtlasPath)
    preview:setSize(
        imageWidth / spriteComponentTemplate.size[1],
        imageHeight / spriteComponentTemplate.size[2]
    )
    setInitBackgroundPosition(preview, spriteComponentTemplate)
    local stopAnimation
    preview:mouseEnter(function()
        stopAnimation = startEntitySpriteAnimation(preview, imageWidth, spriteComponentTemplate)
    end)
    preview:mouseLeave(function()
        stopAnimation()
    end)
    return preview
end

local function entityTexturePreview(entityTemplateName)
    local entityTexturePath = Path.getEntityFilePath(entityTemplateName, 'texture.png')
    return Widget.makeImage(entityTexturePath)
end

local function entityPreview(entityTemplateName)
    -- try sprite component first
    local spriteComponentTemplate = Path.requireComponentTemplateIfExists(entityTemplateName, 'sprite')
    if spriteComponentTemplate then
       return entitySpritePreview(entityTemplateName, spriteComponentTemplate)
    end

    -- then texture component
    local textureComponentTemplate = Path.requireComponentTemplateIfExists(entityTemplateName, 'texture')
    if textureComponentTemplate then
        return entityTexturePreview(entityTemplateName)
    end

    -- none of them
    return nil
end

return {
    entity = entityPreview
}