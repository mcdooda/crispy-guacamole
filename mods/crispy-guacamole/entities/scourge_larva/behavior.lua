local BehaviorHelper = require 'data/scripts/componenthelpers/behavior'

local random = math.random

local states = {}

function states:init(larva)
    larva:enterState 'wander'
end

function states:idle(larva)
    larva:jump()
end

states.wander = BehaviorHelper.wander

return states
