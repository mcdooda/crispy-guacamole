local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local PlaySoundNode = ScriptNode:inherit 'Play Sound'

function PlaySoundNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')
    self.soundNameInPin = self:addInputPin(flat.types.STRING, 'Sound Name')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
end

function PlaySoundNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local entity = runtime:readPin(self.entityInPin)
    local soundName = runtime:readPin(self.soundNameInPin)
    flat.graph.sound.play(Mod.getFilePath('sounds/' .. soundName))
    runtime:impulse(self.impulseOutPin)
end

return PlaySoundNode