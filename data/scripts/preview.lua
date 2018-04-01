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

local function setInitBackgroundPosition(preview, spriteComponentTemplate, animation)
    local line = animation and animation.line or 1
    preview:setBackgroundPosition(
        0,
        (line - 1) / spriteComponentTemplate.size:y()
    )
end

local function startEntitySpriteAnimation(preview, spriteComponentTemplate, animation)
    local frameIndex = 0
    local y = (animation.line - 1) / spriteComponentTemplate.size:y()
    local timer = Timer.new()
    timer:onEnd(function()
            frameIndex = (frameIndex + 1) % animation.numFrames
            local x = frameIndex / spriteComponentTemplate.size:x()
            preview:setBackgroundPosition(x, y)
        end)
    timer:start(animation.frameDuration, true)

    local function stopAnimation()
        timer:stop()
        setInitBackgroundPosition(preview, spriteComponentTemplate, animation)
    end

    return stopAnimation
end

local function startEntitySpriteAnimationByName(preview, spriteComponentTemplate, animationName)
    local animation = getPreviewAnimation(spriteComponentTemplate, animationName)
    if animation then
        return startEntitySpriteAnimation(preview, spriteComponentTemplate, animation)
    end
end

local function entitySpritePreview(entityTemplateName, spriteComponentTemplate, animationName, loopForever, scale)
    local entityAtlasPath = Path.getEntityFilePath(entityTemplateName, 'atlas.png')
    local preview = Widget.makeImage(entityAtlasPath)
    preview:setBackgroundRepeat(Widget.BackgroundRepeat.REPEAT)
    local imageWidth, imageHeight = Image.getSize(entityAtlasPath)
    preview:setSize(
        imageWidth / spriteComponentTemplate.size:x(),
        imageHeight / spriteComponentTemplate.size:y()
    )
    setInitBackgroundPosition(preview, spriteComponentTemplate, getPreviewAnimation(spriteComponentTemplate, animationName))
    local stopAnimation
    if loopForever then
        startEntitySpriteAnimationByName(preview, spriteComponentTemplate, animationName)
    else
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
    return preview
end

local function entityPreview(entityTemplateName, animationName, loopForever, scale)
    -- try sprite component first
    local spriteComponentTemplate = Path.requireComponentTemplateIfExists(entityTemplateName, 'sprite')
    if spriteComponentTemplate then
       return entitySpritePreview(entityTemplateName, spriteComponentTemplate, animationName, loopForever, scale)
    end

    -- none of them
    print('No preview for ' .. entityTemplateName)
    local unavailablePreview = Widget.makeFixedSize(10, 10)
    unavailablePreview:setBackgroundColor(0xFF00FFFF)
    return unavailablePreview
end

return {
    entity = entityPreview,
    sprite = entitySpritePreview
}