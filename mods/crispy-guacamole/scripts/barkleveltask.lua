local BarkSystem = require(Mod.getFilePath 'scripts/barksystem')
local yield = coroutine.yield

while true do
    BarkSystem:update()
    yield()
end