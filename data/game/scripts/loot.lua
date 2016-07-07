local tileData = require 'data/game/scripts/tiledata'
local Money = require 'data/game/scripts/money'
local InterfaceConfig = require 'data/scripts/interfaceconfig'

local Tile = Tile
local random = math.random

local tiles = Map.getTiles()

Timer.new(
	15,
	true,
	function()
		local tile
		local n = 0
		repeat
			tile = tiles[random(1, #tiles)]
			n = n + 1
		until n == 20 or (Tile.isAccessible(tile) and tileData[tile].level == 0 and not tileData[tile].resource and #Tile.getEntities(tile) == 0)
		if n < 20 then
			local x, y, z = Tile.getPosition(tile)
			z = z - 0.5
			local loot = Entity.newAbstract('loot', x, y)
			tileData[tile].loot = loot
			local altitude = 10
			Entity.setZ(loot, z + altitude)
			local color = { 255, 255, 255, 0 }
			Entity.setColor(loot, color)
			Timer.new(
				0.02,
				true,
				function(timer, age)
					if age < 1 then
						color[4] = age * 255
						Entity.setColor(loot, color)
						Entity.setZ(loot, z + (1 - age) * altitude)
					else
						color[4] = 255
						Entity.setColor(loot, color)
						Entity.setZ(loot, z)
						Timer.setLoops(timer, false)
					end
				end
			)
		end
	end
)

local amount = 300
local font = InterfaceConfig.font..'@18'

local function animateLoot(lootAmount, lootAmountShadow)
	local position = { x = 0, y = 30 }
	Widget.setPosition(lootAmount, position)
	position.x = 2
	position.y = 28
	Widget.setPosition(lootAmountShadow, position)
	Widget.show(lootAmount)
	Widget.show(lootAmountShadow)
	Timer.new(
		0.02,
		true,
		function(timer, age)
			if age < 0.6 then
				position.x = 0
				position.y = 30 + age * 30
				Widget.setPosition(lootAmount, position)
				position.x = 2
				position.y = 28 + age * 30
				Widget.setPosition(lootAmountShadow, position)
				local opacity
				if age < 0.1 then
					opacity = age * 10
				else
					opacity = 1 - (age - 0.1) * 2
				end
				Widget.setOpacity(lootAmount, opacity)
				Widget.setOpacity(lootAmountShadow, opacity)
			else
				Widget.hide(lootAmount)
				Widget.hide(lootAmountShadow)
				Timer.setLoops(timer, false)
			end
		end
	)
end

for _, soldier in pairs(Map.getEntities('soldier')) do
	local overlay = Entity.enableOverlay(soldier)
	
	local lootAmountShadow = Widget.new {
		text = '+'..amount..'£',
		font = font,
		color = InterfaceConfig.black,
		parent = overlay,
		anchor = Widget.CENTERX + Widget.CENTERY,
		position = {
			x = 2,
			y = 28
		}
	}
	local lootAmount = Widget.new {
		text = '+'..amount..'£',
		font = font,
		color = InterfaceConfig.gold,
		parent = overlay,
		anchor = Widget.CENTERX + Widget.CENTERY,
		position = {
			x = 0,
			y = 30
		}
	}
	Widget.hide(lootAmount)
	Widget.hide(lootAmountShadow)
	
	Entity.onTileChange(soldier, function()
		local tile = Entity.getTile(soldier)
		if tileData[tile].lava or tileData[tile].level > 0.5 then
			Timer.new(
				0,
				false,
				function()
					local x, y, z = Entity.getPosition(soldier)
					Entity.destroy(soldier)
					Timer.new(
						0,
						false,
						function()
							local ghost = Entity.newAbstract('ghost', x, y)
							Entity.setZ(ghost, z)
							local ghostColor = { 255, 255, 255, 128 }
							Timer.new(
								0.02,
								true,
								function(timer, age)
									if age < 1 then
										ghostColor[4] = (1 - age) * 128
										Entity.setColor(ghost, ghostColor)
										Entity.setZ(ghost, z + age * 3)
									else
										Entity.destroy(ghost)
										Timer.setLoops(timer, false)
									end
								end
							)
						end
					)
				end
			)
		else
			local loot = tileData[tile].loot
			if loot then
				Sound.playSample('loot.wav')
				Money.add(amount)
				Timer.new(0, false, function() Entity.destroy(loot) end)
				tileData[tile].loot = nil
				animateLoot(lootAmount, lootAmountShadow)
			end
		end
	end)
end
