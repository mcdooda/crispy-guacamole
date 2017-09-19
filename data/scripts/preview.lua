local Path = require 'data/scripts/path'

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

local function setInitBackgroundPosition(preview, spriteComponentTemplate, animationName)
    local animation = getPreviewAnimation(spriteComponentTemplate, animationName)
    preview:setBackgroundPosition(
        0,
        (animation.line - 1) / spriteComponentTemplate.size[2]
    )
end

local function startEntitySpriteAnimation(preview, imageWidth, spriteComponentTemplate, animationName)
    local animation = getPreviewAnimation(spriteComponentTemplate, animationName)
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
            setInitBackgroundPosition(preview, spriteComponentTemplate, animationName)
        end

        return stopAnimation
    end
end

local function entitySpritePreview(entityTemplateName, spriteComponentTemplate, animationName, loopForever, scale)
    local entityAtlasPath = Path.getEntityFilePath(entityTemplateName, 'atlas.png')
    local preview = Widget.makeImage(entityAtlasPath)
    preview:setBackgroundRepeat(Widget.BackgroundRepeat.REPEAT)
    local imageWidth, imageHeight = Image.getSize(entityAtlasPath)
    preview:setSize(
        imageWidth / spriteComponentTemplate.size[1],
        imageHeight / spriteComponentTemplate.size[2]
    )
    setInitBackgroundPosition(preview, spriteComponentTemplate, animationName)
    local stopAnimation
    if loopForever then
        startEntitySpriteAnimation(preview, imageWidth, spriteComponentTemplate, animationName)
    else
        preview:mouseEnter(function()
            stopAnimation = startEntitySpriteAnimation(preview, imageWidth, spriteComponentTemplate, animationName)
        end)
        preview:mouseLeave(function()
            stopAnimation()
        end)
    end
    if scale then
        local width, height = preview:getSize()
        preview:setSize(width * scale, height * scale)
        preview:setBackgroundSize(imageWidth * scale, imageHeight * scale)
    end
    return preview
end

local function entityTexturePreview(entityTemplateName, scale)
    local entityTexturePath = Path.getEntityFilePath(entityTemplateName, 'texture.png')
    local preview = Widget.makeImage(entityTexturePath)
    if scale then
        local width, height = preview:getSize()
        preview:setSize(width * scale, height * scale)
    end
    return preview
end

local function entityPreview(entityTemplateName, animationName, loopForever, scale)
    -- try sprite component first
    local spriteComponentTemplate = Path.requireComponentTemplateIfExists(entityTemplateName, 'sprite')
    if spriteComponentTemplate then
       return entitySpritePreview(entityTemplateName, spriteComponentTemplate, animationName, loopForever, scale)
    end

    -- then texture component
    local textureComponentTemplate = Path.requireComponentTemplateIfExists(entityTemplateName, 'texture')
    if textureComponentTemplate then
        return entityTexturePreview(entityTemplateName, scale)
    end

    -- none of them
    print('No preview for ' .. entityTemplateName)
    local unavailablePreview = Widget.makeFixedSize(10, 10)
    unavailablePreview:setBackgroundColor(0xFF00FFFF)
    return unavailablePreview
end

return {
    entity = entityPreview
}