-- tab panel

local TabPanel = {}

local Widget = Widget

function TabPanel:new(background, tabsBackground, parent, anchor, position, dimensions)
	local w = Widget.new {
		image = background,
		anchor = anchor,
		parent = parent,
		position = position,
		dimensions = dimensions
	}
	
	Widget.new {
		image = tabsBackground,
		anchor = Widget.TOP + Widget.LEFT + Widget.RIGHT,
		parent = w
	}
	
	local o = { widget = w, tabsContent = {}, activeTab = nil }
	setmetatable(o, { __index = self })
	return o
end

function TabPanel:addTab(name, imagesPath, widget, onClick)
	local y = -6
	local x = 3
	local xMargin = 3
	
	for tabName, tabContent in pairs(self.tabsContent) do
		local w, h = Widget.getDimensions(tabContent.activeTabWidget)
		x = x + w + xMargin
	end
	
	local activeTabWidget = Widget.new {
		image = imagesPath..'/active.png',
		parent = self.widget,
		anchor = Widget.LEFT + Widget.TOP,
		position = {
			x = x,
			y = y
		}
	}
	Widget.hide(activeTabWidget)
	
	local passiveTabWidget = Widget.new {
		image = imagesPath..'/passive.png',
		parent = self.widget,
		anchor = Widget.LEFT + Widget.TOP,
		position = {
			x = x,
			y = y
		}
	}
	Widget.onClick(passiveTabWidget, function()
		self:openTab(name)
	end)
	
	self.tabsContent[name] = {
		widget = widget,
		activeTabWidget = activeTabWidget,
		passiveTabWidget = passiveTabWidget,
		onClick = onClick
	}
end

function TabPanel:openTab(name)
	if self.activeTab then
		local activeTabContent = self.tabsContent[self.activeTab]
		Widget.hide(activeTabContent.activeTabWidget)
		Widget.show(activeTabContent.passiveTabWidget)
		Widget.hide(activeTabContent.widget)
	end
	
	local newActiveTabContent = self.tabsContent[name]
	Widget.show(newActiveTabContent.activeTabWidget)
	Widget.hide(newActiveTabContent.passiveTabWidget)
	Widget.show(newActiveTabContent.widget)
	
	if newActiveTabContent.onClick then
		newActiveTabContent.onClick()
	end
	
	self.activeTab = name
end

return TabPanel
