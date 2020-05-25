local UiSettings = require 'data/scripts/ui/uisettings'

local AssetIcon = {}
AssetIcon.__index = AssetIcon
AssetIcon.selectedIcons = {}

local function makeAssetIconWidget(asset, preview, size, allowSelection)
    local assetIcon = Widget.makeFixedSize(size, size)

    do
        -- preview container
        local previewContainer = Widget.makeExpand()
        previewContainer:setMargin(4)
        previewContainer:setBackground 'data/editor/interface/icons/background.png'
        previewContainer:setBackgroundRepeat(Widget.BackgroundRepeat.REPEAT)
        previewContainer:setPositionPolicy(Widget.PositionPolicy.CENTER)

        if preview then
            previewContainer:addChild(preview)
        end

        do
            -- asset name
            local textContainer = Widget.makeExpand()
            textContainer:setPositionPolicy(Widget.PositionPolicy.CENTER_X + Widget.PositionPolicy.BOTTOM)
            textContainer:setSizePolicy(Widget.SizePolicy.EXPAND_X + Widget.SizePolicy.COMPRESS_Y)
            textContainer:setBackgroundColor(0x222222CC)
            textContainer:setPadding(5)
    
            do
                local text = Widget.makeText(asset:getName(), table.unpack(UiSettings.defaultFont))
                text:setPositionPolicy(Widget.PositionPolicy.CENTER)
                textContainer:addChild(text)
            end
    
            previewContainer:addChild(textContainer)
        end

        assetIcon:addChild(previewContainer)
    end

    if allowSelection then
        local isSelected = false

        assetIcon:click(function()
            if isSelected then
                return
            end
            isSelected = true
            assetIcon:setBackgroundColor(0xF39C12FF)
        end)
    end
        
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

    flat.ui.addTooltip(assetIcon, asset:getPath())

    return assetIcon
end

function AssetIcon:new(asset, preview, size, allowSelection)
    return setmetatable({
        selected = false,
        container = makeAssetIconWidget(asset, preview, size, allowSelection)
    }, self)
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