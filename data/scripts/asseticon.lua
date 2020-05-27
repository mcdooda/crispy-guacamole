local UiSettings = require 'data/scripts/ui/uisettings'

local AssetIcon = {}
AssetIcon.__index = AssetIcon

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

    local isSelected = false
    if allowSelection then

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
        self.container:setBackgroundColor(selected and 0xF39C12FF or 0)
    end
end

function AssetIcon:isSelected()
    return self.selected
end

return AssetIcon