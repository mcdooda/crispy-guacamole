local Path = require 'data/scripts/path'

local function getTemplate(entity)
    return Path.requireComponentTemplateIfExists(entity:getTemplateName(), 'attack')
end 

local function dealDamageAfterDelay(target, damage, delay, instigator)
	local timer = game.Timer()
	timer:onEnd(function()
        if target:isValid() then
            target:dealDamage(damage, instigator)
        end
    end)
    timer:start(delay)
end

return {
    getTemplate          = getTemplate,

    dealDamageAfterDelay = dealDamageAfterDelay
}