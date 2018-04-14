local Score = {
    kills = 0,
    leaks = 0,
    scoreChanged = flat.Slot:new()
}

function Score:addKill()
    self.kills = self.kills + 1
    self.scoreChanged(self.kills, self.leaks)
end

function Score:addLeak()
    self.leaks = self.leaks + 1
    self.scoreChanged(self.kills, self.leaks)
end

function Score:onScoreChanged(scoreChanged)
    self.scoreChanged:on(scoreChanged)
end

function Score:getKills()
    return self.kills
end

function Score:getLeaks()
    return self.leaks
end

return Score