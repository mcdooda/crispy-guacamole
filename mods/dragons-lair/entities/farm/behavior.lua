local states = {}

local vegetables = { 'carrot', 'pepper', 'aubergine', 'raddish'}

function states:init(farm)
	farm:click(function(farm) 			
		local position = farm:getPosition() + flat.Vector3(0, 0, 1)
		local heading = math.random() * 2 * math.pi
		local elevation = math.random() * math.pi / 8 + (math.pi / 2 - math.pi / 8)
		local vegetableType = vegetables[math.random(4)]
		local vegetable = Entity.spawn('vegetables', position, heading, elevation)
		vegetable:playAnimation(vegetableType, -1, false)
	end)
end

function states:idle(farm)	
end

return states
