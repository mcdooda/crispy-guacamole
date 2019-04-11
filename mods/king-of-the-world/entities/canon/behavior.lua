local BehaviorHelper = require 'data/scripts/componenthelpers/behavior'

local sqrt = math.sqrt
local random = math.random

local movement = require 'mods/crispy-guacamole/entities/sheep/movement'

local states = {}

function states:init(sheep)
    sheep:setCycleAnimation 'test'
end


return states