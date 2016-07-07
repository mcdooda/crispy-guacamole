local towers = require 'data/game/scripts/towers'

for _, Tower in pairs(towers) do
	for _, building in pairs(Map.getEntities(Tower.buildingModel)) do
		local x, y = Entity.getCenter(building)
		local tower = Tower:new(x, y)
		tower:start()
	end
end

