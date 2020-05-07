local Preview = require 'data/scripts/preview'
local ModData = require 'data/editor/scripts/moddata'
local UiSettings = require 'data/scripts/ui/uisettings'

local AssetIcon = {}
AssetIcon.__index = AssetIcon
AssetIcon.selectedIcons = {}

local function makeAssetIconWidget(assetName, preview, size)
    local assetIcon = Widget.makeFixedSize(size, size)

    do
        -- preview container
        local previewContainer = Widget.makeExpand()
        previewContainer:setMargin(4)
        previewContainer:setBackground 'data/editor/interface/icons/background.png'
        previewContainer:setBackgroundRepeat(Widget.BackgroundRepeat.REPEAT)
        previewContainer:setPositionPolicy(Widget.PositionPolicy.CENTER)
        previewContainer:addChild(preview)

        do
            -- asset name
            local textContainer = Widget.makeExpand()
            textContainer:setPositionPolicy(Widget.PositionPolicy.CENTER_X + Widget.PositionPolicy.BOTTOM)
            textContainer:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.COMPRESS_Y)
            textContainer:setBackgroundColor(0x222222CC)
            textContainer:setPadding(5)
    
            do
                local text = Widget.makeText(assetName, table.unpack(UiSettings.defaultFont))
                text:setPositionPolicy(Widget.PositionPolicy.CENTER)
                textContainer:addChild(text)
            end
    
            previewContainer:addChild(textContainer)
        end

        assetIcon:addChild(previewContainer)
    end

    local isSelected = false

    assetIcon:click(function()
        if isSelected then
            return
        end
        isSelected = true
        assetIcon:setBackgroundColor(0xF39C12FF)
    end)
    
    assetIcon:mouseEnter(function()
        if not isSelected then
            assetIcon:setBackgroundColor(0xF1C40FFF)
        end
    end)

    assetIcon:mouseLeave(function()
        if not isSelected then
            assetIcon:setBackgroundColor(0)
        end
    end)

    flat.ui.addTooltip(assetIcon, assetName)

    return assetIcon
end

function AssetIcon:new(assetName, preview, size)
    return setmetatable({
        selected = false,
        container = makeAssetIconWidget(assetName, preview, size)
    }, self)
end

function AssetIcon:entity(entityTemplateName, size)
    local preview = Preview.entity(entityTemplateName, nil, false, 1)
    local width, height = preview:getSize()
    if width * 2 < size and height * 2 < size then
        preview = Preview.entity(entityTemplatePath, nil, false, 2)
    end
    preview:setPositionPolicy(Widget.PositionPolicy.CENTER)
    preview:setPosition(0, 10)
    return self:new(entityTemplateName, preview, size)
end

function AssetIcon:tile(tileTemplateName, size)
    local preview = Preview.tile(tileTemplateName, 1, false, 1)
    preview:setPositionPolicy(Widget.PositionPolicy.CENTER_X + Widget.PositionPolicy.TOP)
    preview:setPosition(0, -16)
    return self:new(tileTemplateName, preview, size)
end

function AssetIcon:prop(propTemplateName, size)
    local preview = Preview.prop(propTemplateName, ModData.props.getHighest(propTemplateName), 1)
    preview:setPositionPolicy(Widget.PositionPolicy.CENTER)
    preview:setPosition(0, 10)
    return self:new(propTemplateName, preview, size)
end

function AssetIcon:setSelected(selected, addToSelection)
    if self.selected ~= selected then
        self.selected = selected

        local selectedIcons = getmetatable(self).selectedIcons
        if selected then
            if not addToSelection then
                self:clearSelection()
            end
            flat.arrayAdd(selectedIcons, self)
            self.container:setBackgroundColor(0xF39C12FF)
        else
            flat.arrayRemoveValueCyclic(selectedIcons, self)
            self.container:setBackgroundColor(0)
        end
    end
end

function AssetIcon:isSelected()
    return self.selected
end

function AssetIcon:clearSelection()
    local selectedIcons = getmetatable(self).selectedIcons
    for i = 1, #selectedIcons do
        selectedIcons[i]:setSelected(false)
    end
end

return AssetIcon