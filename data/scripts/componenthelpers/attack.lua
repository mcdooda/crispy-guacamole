local function dealDamageAfterDelay(target, damage, delay)
	local timer = game.Timer()
	timer:onEnd(function()
        if target:isValid() then
            target:dealDamage(damage)
        end
    end)
    timer:start(delay)
end

return {
    dealDamageAfterDelay = dealDamageAfterDelay
}