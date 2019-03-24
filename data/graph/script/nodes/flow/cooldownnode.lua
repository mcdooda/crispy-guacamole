local ScriptNode = flat.require 'graph/script/scriptnode'
local ScriptNodeRuntime = flat.require 'graph/script/scriptnoderuntime'
local PinTypes = flat.require 'graph/pintypes'

-- runtime

local CooldownNodeRuntime = ScriptNodeRuntime:inherit()

function CooldownNodeRuntime:init(node)
    self.perContextData = setmetatable({}, { __mode = 'k' })
end

function CooldownNodeRuntime:execute(node)
    local context = self:readPin(node.contextInPin)
    local contextId = flat.getUniqueObject(context)
    local contextData = self.perContextData[contextId]
    local currentTime = game.getTime()
    if not contextData then
        local cooldown = self:readPin(node.cooldownInPin)
        contextData = {
            cooldown = cooldown,
            lastImpulseTime = currentTime
        }
        self.perContextData[contextId] = contextData
        -- note: impulse on first call
        self:impulse(node.impulseOutPin)
    elseif currentTime >= contextData.lastImpulseTime + contextData.cooldown then
        contextData.lastImpulseTime = currentTime
        self:impulse(node.impulseOutPin)
    end
end

-- node

local CooldownNode = ScriptNode:inherit 'Cooldown'

function CooldownNode:buildPins()
    self.impulseInPin = self:addInputPin(PinTypes.IMPULSE, 'In')
    self.contextInPin = self:addInputPin(PinTypes.ANY, 'Context')
    self.cooldownInPin = self:addInputPin(flat.types.NUMBER, 'Cooldown')

    self.impulseOutPin = self:addOutputPin(PinTypes.IMPULSE, 'Out')
end

function CooldownNode:getRuntimeType()
    return CooldownNodeRuntime
end

function CooldownNode:execute(runtime, inputPin)
    assert(inputPin == self.impulseInPin)
    runtime:execute(self)
end

return CooldownNode