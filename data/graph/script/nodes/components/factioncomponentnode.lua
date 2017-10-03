local FunctionalScriptNode = flat.require 'graph/script/functionalscriptnode'

local FactionComponentNode = FunctionalScriptNode:inherit 'Faction Component'

function FactionComponentNode:buildPins()
    self.factionInPin = self:addInputPin(flat.types.STRING, 'Faction')

    self.componentOutPin = self:addOutputPin(flat.types.TABLE, 'Component')
end

function FactionComponentNode:execute(runtime)
    local faction = runtime:readPin(self.factionInPin)

    local component = {
        faction = faction
    }

    runtime:writePin(self.componentOutPin, component)
end

return FactionComponentNode