local ScriptNode = flat.require 'graph/script/scriptnode'
local PinTypes = flat.require 'graph/pintypes'

local AttackHelper = require 'data/scripts/componenthelpers/attack'

local DealDamageNode = ScriptNode:inherit 'Deal Damage'

function DealDamageNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.entityInPin = self:addInputPin(flat.types['CG.Entity'], 'Entity')
    self.amountInPin = self:addInputPin(flat.types.NUMBER, 'Amount')
    self.delayInPin = self:addInputPin(flat.types.NUMBER, 'Delay')
    self.instigatorInPin = self:addInputPin(flat.types['CG.Entity'], 'Instigator')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
end

function DealDamageNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    local entity = runtime:readPin(self.entityInPin)
    local amount = runtime:readPin(self.amountInPin)
    local delay = runtime:readPin(self.delayInPin)
    local instigator = runtime:readOptionalPin(self.instigatorInPin)

    AttackHelper.dealDamageAfterDelay(entity, amount, delay, instigator)

    runtime:impulse(self.impulseOutPin)
end

return DealDamageNode