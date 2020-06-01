local function start_construction(building)
    building:setHealth(1)
    building:setAnimationProgress('build', 1 / building:getMaxHealth())
end

local function build(building, value)
    local max = building:getMaxHealth()
    local health = math.min(building:getHealth() + value, max)
    local progress = health / max
    building:setHealth(health)
    if progress >= 1 then
        return true
    end
    building:setAnimationProgress('build', progress)
    return false
end

return {
    start_construction = start_construction,
    build = build
}