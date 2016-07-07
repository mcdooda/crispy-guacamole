local Money = {}
local amount = 0
local harvesters = {}
local harvestTimer

function Money.setAmount(a)
	amount = a
	Money.updateWidget(amount)
end

function Money.getAmount()
	return amount
end

function Money.add(a)
	amount = amount + a
	Money.updateWidget(amount)
end

function Money.sub(s)
	amount = amount - s
	Money.updateWidget(amount)
end

function Money.enough(e)
	return amount >= e
end

function Money.updateWidget() -- replace by the real function in rightpanel.lua
	
end

function Money.clearHarvesters()
	harvesters = {}
end

function Money.addHarvester(harvester)
	harvesters[harvester] = true
	if not harvestTimer then
		harvestTimer = Timer.new(
			1,
			true,
			function()
				local sum = 0
				for harvester in pairs(harvesters) do
					harvester:animateHarvest()
					sum = sum + harvester:getHarvest()
				end
				Money.add(sum)
			end
		)
	end
end

function Money.removeHarvester(harvester)
	harvesters[harvester] = nil
	if not next(harvesters) then
		Timer.stop(harvestTimer)
		harvestTimer = nil
	end
end

return Money
