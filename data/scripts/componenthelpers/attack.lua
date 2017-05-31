local timerStart = Timer.start

local function dealDamageAfterDelay(target, damage, delay)
    timerStart(delay, nil, function()
        if target:isValid() then
            target:dealDamage(damage)
        end
    end)
end

return {
    dealDamageAfterDelay = dealDamageAfterDelay
}