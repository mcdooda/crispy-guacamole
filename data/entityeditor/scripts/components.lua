local UiSettings = require 'data/scripts/ui/uisettings'
local Path = require 'data/scripts/path'
local Icon = require 'data/scripts/ui/icon'
local ComponentsModel = require 'data/entityeditor/scripts/componentsmodel'

local Theme = {
    SELECTED_COMPONENT_BACKGROUND_COLOR = 0x666666FF,
    COMPONENT_ENABLED_COLOR             = 0xFFFFFFFF,
    COMPONENT_DISABLED_COLOR            = 0x999999FF,
    COMPONENT_ERROR_COLOR               = 0xFF5555FF
}

-- EntityState

local EntityState = {}

function EntityState:init()

end

function EntityState:getEntity()
    if self.entity and self.entity:isValid() then
        return self.entity
    end

    -- the entity might have been killed, find a new one (spawned by EntityEditorState in c++)
    local startZone = Map.getZone 'Start'
    local zoneCenter = startZone:getCenter()
    self.entity = startZone:getEntities()[1]
    self.entity:setDebug(true)
    return self.entity
end

function EntityState:getTemplatePath()
    return self:getEntity():getTemplatePath()
end

function EntityState:findFirstEnabledComponent()
    local entity = self:getEntity()
    local componentsSorted = ComponentsModel.sortedByName()
    for i, componentName in pairs(componentsSorted) do
        local componentFlag = Component[componentName]
        if entity:hasComponent(componentFlag) then
            return componentName
        end
    end
end

function EntityState:componentFileExists(componentName)
    return Path.componentFileExists(EntityState:getTemplatePath(), componentName)
end

function EntityState:componentIsGraph(componentName)
    return Path.componentFileExists(EntityState:getTemplatePath(), componentName .. '.graph')
end

function EntityState:hasComponent(componentName)
    local entity = self:getEntity()
    return entity:hasComponent(Component[componentName])
end

function EntityState:isComponentBroken(componentName)
    return self:componentFileExists(componentName) and not self:hasComponent(componentName)
end

function EntityState:isComponentEnabled(componentName)
    assert(self:hasComponent(componentName))
    local entity = self:getEntity()
    return entity:isComponentEnabled(Component[componentName])
end

function EntityState:enableComponent(componentName)
    assert(not self:isComponentEnabled(componentName))
    local entity = self:getEntity()
    entity:decComponentDisableLevel(Component[componentName])
end

function EntityState:disableComponent(componentName)
    assert(self:isComponentEnabled(componentName))
    local entity = self:getEntity()
    entity:incComponentDisableLevel(Component[componentName])
end

-- UI

local EntityEditorPanel = {}
local ComponentSelectionPanel = {}
local ComponentDetailsPanel = {}

-- EntityEditorPanel

function EntityEditorPanel:build()
    local panel = Widget.makeLineFlow()
    panel:setSizePolicy(Widget.SizePolicy.COMPRESS_X + Widget.SizePolicy.EXPAND_Y)
    self.panel = panel
    ComponentSelectionPanel:build()
    ComponentDetailsPanel:build()
    ComponentSelectionPanel:setComponentTab(EntityState:findFirstEnabledComponent())
    Widget.getRoot():addChild(panel)
end

-- ComponentSelectionPanel

function ComponentSelectionPanel:build()
    local panel = Widget.makeColumnFlow()
    panel:setBackgroundColor(0x444444FF)
    panel:setSizePolicy(Widget.SizePolicy.COMPRESS_X + Widget.SizePolicy.EXPAND_Y)

    do
        local titleLabel = Widget.makeText('Components', table.unpack(UiSettings.titleFont))
        titleLabel:setMargin(2, 5, 2, 5)
        panel:addChild(titleLabel)
    end

    self.panel = panel
    self:buildComponentTabs()
    EntityEditorPanel.panel:addChild(panel)
end

function ComponentSelectionPanel:getCurrentComponentName()
    assert(self.currentComponentName)
    return self.currentComponentName
end

function ComponentSelectionPanel:buildComponentTabs()
    local entity = EntityState:getEntity()
    local componentsSorted = ComponentsModel.sortedByName()

    self.componentNameLabels = {}
    self.componentTabs = {}

    for i, componentName in pairs(componentsSorted) do

        local componentTab = Widget.makeColumnFlow()
        componentTab:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.COMPRESS_Y)
        local componentNameLabel = Widget.makeText(Components.getVisualName(Component[componentName]), table.unpack(UiSettings.defaultFont))

        self.componentNameLabels[componentName] = componentNameLabel
        self:updateTabColor(componentName)

        componentTab:click(function()
            self:setComponentTab(componentName)
        end)

        componentTab:setPadding(1, 5, 1, 5)
        componentTab:addChild(componentNameLabel)

        self.panel:addChild(componentTab)
        self.componentTabs[componentName] = componentTab
    end
end

function ComponentSelectionPanel:updateTabColor(componentName)
    local componentNameLabel = self.componentNameLabels[componentName]

    local componentFlag = Component[componentName]
    local entity = EntityState:getEntity()
    local hasComponent = entity:hasComponent(componentFlag)
    if hasComponent then
        componentNameLabel:setTextColor(Theme.COMPONENT_ENABLED_COLOR)
    elseif EntityState:componentFileExists(componentName) then
        componentNameLabel:setTextColor(Theme.COMPONENT_ERROR_COLOR)
    else
        componentNameLabel:setTextColor(Theme.COMPONENT_DISABLED_COLOR)
    end
end

function ComponentSelectionPanel:disableTab(componentName)
    self.componentTabs[componentName]:setBackgroundColor(0x00000000)

    local entity = EntityState:getEntity()
    local componentFlag = Component[componentName]
    if entity:hasComponent(componentFlag) then
        entity:setComponentDebug(componentFlag, false)
    end
end

function ComponentSelectionPanel:enableTab(componentName)
    self.componentTabs[componentName]:setBackgroundColor(Theme.SELECTED_COMPONENT_BACKGROUND_COLOR)

    local entity = EntityState:getEntity()
    local componentFlag = Component[componentName]
    if entity:hasComponent(componentFlag) then
        entity:setComponentDebug(componentFlag, true)
    end
end

function ComponentSelectionPanel:setComponentTab(componentName)
    if componentName == self.currentComponentName then
        return
    end

    if self.currentComponentName then
        self:disableTab(self.currentComponentName)
    end

    self:enableTab(componentName)
    ComponentDetailsPanel:setComponent(componentName)

    self.currentComponentName = componentName
end

function ComponentSelectionPanel:updateCurrentTab()
    assert(self.currentComponentName)
    self:updateTabColor(self.currentComponentName)
    ComponentDetailsPanel:setComponent(self.currentComponentName)
end

-- ComponentDetailsPanel

function ComponentDetailsPanel:build()
    local panel = Widget.makeColumnFlow()
    panel:setBackgroundColor(Theme.SELECTED_COMPONENT_BACKGROUND_COLOR)
    panel:setSizePolicy(Widget.SizePolicy.FIXED_X + Widget.SizePolicy.EXPAND_Y)
    panel:setSize(200, 0)

    do
        local titleLine = Widget.makeLineFlow()

        do
            local titleLabel = Widget.makeText('<Title>', table.unpack(UiSettings.titleFont))
            titleLabel:setMargin(2, 0, 0, 5)
            titleLine:addChild(titleLabel)
            self.titleLabel = titleLabel
        end

        do
            local enabledLabel = Widget.makeText('<Enabled>', table.unpack(UiSettings.defaultFont))
            enabledLabel:setPositionPolicy(Widget.PositionPolicy.BOTTOM_LEFT)
            enabledLabel:setTextColor(0xCCCCCCFF)
            enabledLabel:setMargin(0, 0, 5, 5)
            enabledLabel:click(function()
                ComponentDetailsPanel:toggleCurrentComponent()
            end)
            titleLine:addChild(enabledLabel)
            self.enabledLabel = enabledLabel
        end

        do
            local editComponentIcon = Icon:new 'brush'
            editComponentIcon.container:setMargin(0, 0, 5, 5)
            editComponentIcon.container:click(function()
                self:editCurrentComponent()
            end)
            titleLine:addChild(editComponentIcon.container)
            self.editComponentIcon = editComponentIcon
        end

        do
            local deleteComponentIcon = Icon:new 'remove'
            deleteComponentIcon.container:setMargin(0, 0, 5, 5)
            deleteComponentIcon.container:click(flat.ui.task(function()
                if flat.ui.confirm 'Are you sure you want to delete this component?' then
                    self:removeCurrentComponent()
                end
            end))
            titleLine:addChild(deleteComponentIcon.container)
            self.deleteComponentIcon = deleteComponentIcon
        end

        panel:addChild(titleLine)
    end

    do
        local detailsPanel = Widget.makeColumnFlow()
        detailsPanel:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.COMPRESS_Y)
        detailsPanel:setPadding(5)
        panel:addChild(detailsPanel)
        self.detailsPanel = detailsPanel
    end

    EntityEditorPanel.panel:addChild(panel)
end

function ComponentDetailsPanel:toggleCurrentComponent()
    local componentName = ComponentSelectionPanel:getCurrentComponentName()
    if EntityState:isComponentEnabled(componentName) then
        EntityState:disableComponent(componentName)
        self.enabledLabel:setText('(off)')
    else
        EntityState:enableComponent(componentName)
        self.enabledLabel:setText('(on)')
    end
end

function ComponentDetailsPanel:shouldEditGraph(componentName)
    return not EntityState:hasComponent(componentName) or EntityState:componentIsGraph(componentName)
end

function ComponentDetailsPanel:editCurrentComponent()
    local entityTemplatePath = EntityState:getTemplatePath()
    local componentName = ComponentSelectionPanel:getCurrentComponentName()

    local editGraph = self:shouldEditGraph(componentName)

    if not editGraph then
        flat.textEditor.open(Path.getComponentPath(entityTemplatePath, componentName .. '.lua'))
    else
        flat.graph.editor.open(
            Widget.getRoot(),
            Path.getComponentPath(entityTemplatePath, componentName),
            'script',
            { entityTemplatePath = entityTemplatePath },
            function(isNew)
                -- kill the entity to respawn a new one with the right components
                EntityState:getEntity():delete()
                game.debug_reloadComponent(entityTemplatePath, Component[componentName], isNew or EntityState:isComponentBroken(componentName))

                -- force reload component template
                Path.requireComponentTemplate(entityTemplatePath, componentName, true)

                EntityEditor.entitySpawned(function(entity)
                    ComponentSelectionPanel:updateCurrentTab()
                    return false
                end)
            end
        )
    end
end

function ComponentDetailsPanel:removeCurrentComponent()
    local entityTemplatePath = EntityState:getTemplatePath()
    local componentName = ComponentSelectionPanel:getCurrentComponentName()

    local componentPath = Path.getComponentPath(entityTemplatePath, componentName)
    os.remove(componentPath .. '.graph.lua')
    os.remove(componentPath .. '.layout.lua')
    os.remove(componentPath .. '.lua')

    -- force reload component template
    Path.requireComponentTemplateIfExists(entityTemplatePath, componentName, true)

    -- kill the entity to respawn a new one with the right components
    EntityState:getEntity():delete()
    EntityEditor.entityDespawned(function()
        game.debug_removeComponent(entityTemplatePath, Component[componentName])
        return false
    end)
    EntityEditor.entitySpawned(function(entity)
        ComponentSelectionPanel:updateCurrentTab()
        return false
    end)
end

function ComponentDetailsPanel:setComponent(componentName)
    local title = Components.getVisualName(Component[componentName])
    self.titleLabel:setText(title)

    if EntityState:hasComponent(componentName) then
        if EntityState:isComponentEnabled(componentName) then
            self.enabledLabel:setText('(on)')
        else
            self.enabledLabel:setText('(off)')
        end
        self.enabledLabel:show()
        self.deleteComponentIcon.container:show()
    else
        self.enabledLabel:hide()
        self.deleteComponentIcon.container:hide()
    end

    if self:shouldEditGraph(componentName) then
        self.editComponentIcon:setIcon 'graph'
    else
        self.editComponentIcon:setIcon 'pencil'
    end

    self.detailsPanel:removeAllChildren()
    if EntityState:hasComponent(componentName) then
        self:showComponentDetails(componentName)
    else
        self:showComponentDoesNotExist(componentName)
    end
end

function ComponentDetailsPanel:showComponentDetails(componentName)
    local componentDetailsFilePath = 'data/entityeditor/scripts/components/' .. componentName
    local componentDetailsFile = io.open(componentDetailsFilePath .. '.lua', 'r')
    if componentDetailsFile then
        componentDetailsFile:close()
        local entityTemplatePath = EntityState:getTemplatePath()
        local componentTemplate = Path.requireComponentTemplate(entityTemplatePath, componentName)
        require(componentDetailsFilePath)(self.detailsPanel, entityTemplatePath, componentTemplate, function() return EntityState:getEntity() end)
    end
end

function ComponentDetailsPanel:showComponentDoesNotExist(componentName)
    local label
    if EntityState:componentFileExists(componentName) then
        label = Widget.makeText('This component is broken', table.unpack(UiSettings.defaultFont))
        label:setTextColor(Theme.COMPONENT_ERROR_COLOR)
    else
        label = Widget.makeText('This component does not exist', table.unpack(UiSettings.defaultFont))
    end
    self.detailsPanel:addChild(label)
end


EntityState:init()
EntityEditorPanel:build()