local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'
local BarkSystem = require(Mod.getFilePath 'scripts/barksystem')

local PlayBarNode = ScriptNode:inherit 'Play Bark'

function PlayBarNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')
    self.barkNameInPin = self:addInputPin(flat.types.STRING, 'Bark Name')
    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
end

function PlayBarNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local entity = runtime:readPin(self.entityInPin)
    local barkName = runtime:readPin(self.barkNameInPin)
    BarkSystem:requestBark(Mod.getFilePath('sounds/' .. barkName))
    runtime:impulse(self.impulseOutPin)
end

return PlayBarNode