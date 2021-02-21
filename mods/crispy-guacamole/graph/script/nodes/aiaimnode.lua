local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local AIAimNode = ScriptNode:inherit 'AI Aim'

function AIAimNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.instigatorInPin = self:addInputPin(flat.types['CG.Entity'], 'Instigator')
    self.positionInPin = self:addInputPin(flat.types['flat.Vector2'], 'Position')
    self.followInstigatorInPin = self:addInputPin(flat.types.BOOLEAN, 'Follow Instigator')
    self.radiusInPin = self:addInputPin(flat.types.NUMBER, 'Radius')
    self.durationInPin = self:addInputPin(flat.types.NUMBER, 'Duration')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
    self.instigatorOutPin = self:addOutputPin(flat.types['CG.Entity'], 'Instigator')
    self.positionOutPin = self:addOutputPin(flat.types['flat.Vector3'], 'Position')
    self.successOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Success')
end

function AIAimNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local instigator = runtime:readPin(self.instigatorInPin)
    local position = runtime:readPin(self.positionInPin)
    local followInstigator = runtime:readPin(self.followInstigatorInPin)
    local radius = runtime:readPin(self.radiusInPin)
    local duration = runtime:readPin(self.durationInPin)

    local areaAim = Entity.spawn('ai_aim_outer', position, nil, nil, instigator)
    areaAim:setSpriteScale(radius / 1.3)
    local position3d = areaAim:getPosition()

    local aimInstigatorRelativePosition
    if followInstigator then
        aimInstigatorRelativePosition = position3d - instigator:getPosition()
    end

    local timerAimOffset = flat.Vector3(0, 0, 0.01)
    local timerAim = Entity.spawn('ai_aim_inner', position3d + timerAimOffset, nil, nil, instigator)
    timerAim:setSpriteScale(0)

    runtime:writePin(self.instigatorOutPin, instigator)
    runtime:writePin(self.positionOutPin, position3d)
    runtime:impulse(self.impulseOutPin)

    instigator:playAnimation('attack', 1, false)
    
    local time = game.getTime()
    local startTime = time
    local elapsedTime = 0
    local endTime = startTime + duration
    while time < endTime do
        local t = elapsedTime / duration
        
        if t < 0.1 then
            areaAim:setSpriteColor(0xFFFFFF00 + math.floor(t * 10 * 0xFF))
        else
            areaAim:setSpriteColor(0xFFFFFFFF)
        end
        timerAim:setSpriteScale(t * radius / 1.3)

        if aimInstigatorRelativePosition then
            local instigatorPosition = instigator:getPosition()
            areaAim:setPosition(instigatorPosition + aimInstigatorRelativePosition)
            timerAim:setPosition(instigatorPosition + aimInstigatorRelativePosition + timerAimOffset)
        end

        coroutine.yield()
        time = game.getTime()
        elapsedTime = time - startTime
    end

    areaAim:despawn()
    timerAim:despawn()

    runtime:writePin(self.instigatorOutPin, instigator)
    runtime:writePin(self.positionOutPin, position3d)
    runtime:impulse(self.successOutPin)
end

return AIAimNode