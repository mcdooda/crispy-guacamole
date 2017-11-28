local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local LifeComponentNode = FunctionalScriptNode:inherit 'Life Component'

function LifeComponentNode:buildPins()
    self.maxHealthInPin = self:addInputPin(flat.types.NUMBER, 'Max Health')
    self.spawnInPin = self:addInputPin(flat.types.FUNCTION, 'On Spawn')
    self.despawnInPin = self:addInputPin(flat.types.FUNCTION, 'On Despawn')

    self.componentOutPin = self:addOutputPin(flat.types.TABLE, 'Component')
end

function LifeComponentNode:execute(runtime)
    local maxHealth = runtime:readPin(self.maxHealthInPin)
    local spawn = runtime:readPin(self.spawnInPin)
    local despawn = runtime:readPin(self.despawnInPin)

    local component = {
        maxHealth = maxHealth,
        spawn = spawn,
        despawn = despawn
    }

    runtime:writePin(self.componentOutPin, component)
end

return LifeComponentNode