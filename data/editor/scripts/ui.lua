dofile 'data/editor/scripts/tests.lua'
dofile 'data/scripts/stats.lua'

local dump = require 'data/scripts/dump'
local checkbox = require 'data/scripts/ui/checkbox'

local root = Widget.getRoot()
do
    local check1 = checkbox:new(false, 10, false)
    check1.container:setPosition(300,-300)
    root:addChild(check1.container)
end