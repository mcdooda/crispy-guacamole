local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'
local PinTypes = flat.require 'graph/pintypes'

local LifeComponentNode = FunctionalScriptNode:inherit 'Life Component'

function LifeComponentNode:buildPins()
    self.maxHealthInPin = self:addInputPin(flat.types.NUMBER, 'Max Health')

    self.componentOutPin = self:addOutputPin(flat.types.TABLE, 'Component')
    self.onSpawnOutPin = self:addOutputPin(PinTypes.IMPULSE, 'On Spawn')
    self.onDespawnOutPin = self:addOutputPin(PinTypes.IMPULSE, 'On Despawn')
    self.entityOutPin = self:addOutputPin(flat.types['CG.Entity'], 'Entity')
end

function LifeComponentNode:execute(runtime)
    local maxHealth = runtime:readPin(self.maxHealthInPin)

    local component = {
        maxHealth = maxHealth,
        spawn = function(entity)
            runtime:writePin(self.entityOutPin, entity)
            runtime:impulse(self.onSpawnOutPin)
        end,
        despawn = function(entity)
            runtime:writePin(self.entityOutPin, entity)
            runtime:impulse(self.onDespawnOutPin)
        end
    }

    runtime:writePin(self.componentOutPin, component)
end

return LifeComponentNode