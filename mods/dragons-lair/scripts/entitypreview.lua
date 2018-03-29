local Path = require 'data/scripts/path'

local EntityPreview = {}
EntityPreview.__index = EntityPreview

function EntityPreview:new(entityName, scale)
    local o = setmetatable({
        entityName = entityName,
        widget = nil,
        stopAnimation = nil,
        spriteComponentTemplate = nil
    }, self)
    o:build(scale)
    return o
end

local function setInitBackgroundPosition(preview, spriteComponentTemplate, animation)
    local line = animation and animation.line or 1
    preview:setBackgroundPosition(
        0,
        (line - 1) / spriteComponentTemplate.size:y()
    )
end

function EntityPreview:build(scale)
    self.spriteComponentTemplate = Path.requireComponentTemplateIfExists(self.entityName, 'sprite')
    if self.spriteComponentTemplate then
        local entityAtlasPath = Path.getEntityFilePath(self.entityName, 'atlas.png')
        self.widget = Widget.makeImage(entityAtlasPath)
        self.widget:setBackgroundRepeat(Widget.BackgroundRepeat.REPEAT)
        local imageWidth, imageHeight = Image.getSize(entityAtlasPath)
        self.widget:setSize(
            imageWidth / self.spriteComponentTemplate.size:x() * scale,
            imageHeight / self.spriteComponentTemplate.size:y() * scale
        )
        self.widget:setBackgroundSize(imageWidth * scale, imageHeight *scale)
    else
        print('No preview for ' .. self.entityName)
        local unavailablePreview = Widget.makeFixedSize(10, 10)
        unavailablePreview:setBackgroundColor(0xFF00FFFF)
        self.widget = unavailablePreview
    end
end

function EntityPreview:startEntitySpriteAnimation(animation, duration)
    local frameIndex = 0
    local y = (animation.line - 1) / self.spriteComponentTemplate.size:y()
    
    local function updateFrame()
        frameIndex = (frameIndex + 1) % animation.numFrames
        local x = frameIndex / self.spriteComponentTemplate.size:x()
        self.widget:setBackgroundPosition(x, y)
    end
    updateFrame()
    local timer = Timer.new()
    timer:onEnd(updateFrame)
    timer:start(animation.frameDuration, true)

    local animationTimer = nil
    local function resetPreview()
        if timer then
            timer:stop()
            timer = nil
        end
        animationTimer = nil
        self.stopAnimation = nil
        setInitBackgroundPosition(self.widget, self.spriteComponentTemplate, animation)
    end

    if duration > 0 then
        animationTimer = Timer.new()
        animationTimer:onEnd(resetPreview)
        animationTimer:start(duration)
    end

    local function stopAnimation()
        if animationTimer then
            animationTimer:stop()
            animationTimer = nil
        end
        resetPreview()
    end
    self.stopAnimation = stopAnimation
end

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

function EntityPreview:startAnimation(animationName, duration)
    duration = duration or 0
    if self.spriteComponentTemplate then
        if self.stopAnimation then
            self.stopAnimation()
        end

        local animation = getPreviewAnimation(self.spriteComponentTemplate, animationName)
        if animation then
            self:startEntitySpriteAnimation(animation, duration)
        end
   end
end

return EntityPreview