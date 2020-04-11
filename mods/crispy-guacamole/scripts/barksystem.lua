local random = math.random

local BarkSystem = {
    requestedBarks = {}
}

function BarkSystem:update()
    if #self.requestedBarks > 0 then
        local randomBarkIndex = random(1, #self.requestedBarks)
        local randomBark = self.requestedBarks[randomBarkIndex]
        flat.graph.sound.play(randomBark)
        self.requestedBarks = {}
    end
end

function BarkSystem:requestBark(soundName)
    self.requestedBarks[#self.requestedBarks + 1] = soundName
end

return BarkSystem