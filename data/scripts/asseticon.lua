local Preview = require 'data/scripts/preview'
local ModData = require 'data/editor/scripts/moddata'
local UiSettings = require 'data/scripts/ui/uisettings'

local function makeAssetIconWidget(assetName, preview, size)
    local assetIcon = Widget.makeFixedSize(size, size)
    assetIcon:setBackground 'data/editor/interface/icons/background.png'
    assetIcon:setBackgroundRepeat(Widget.BackgroundRepeat.REPEAT)
    assetIcon:setPositionPolicy(Widget.PositionPolicy.CENTER)

    assetIcon:addChild(preview)

    do
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

        assetIcon:addChild(textContainer)
    end

    return assetIcon
end

local function entityAssetIcon(entityTemplateName, size)
    local preview = Preview.entity(entityTemplateName, nil, false, 1)
    local width, height = preview:getSize()
    if width * 2 < size and height * 2 < size then
        preview = Preview.entity(entityTemplateName, nil, false, 2)
    end
    preview:setPositionPolicy(Widget.PositionPolicy.CENTER)
    preview:setPosition(0, 10)
    return makeAssetIconWidget(entityTemplateName, preview, size)
end

local function tileAssetIcon(tileTemplateName, size)
    local preview = Preview.tile(tileTemplateName, 1, false, 1)
    preview:setPositionPolicy(Widget.PositionPolicy.CENTER_X + Widget.PositionPolicy.TOP)
    preview:setPosition(0, -16)
    return makeAssetIconWidget(tileTemplateName, preview, size)
end

local function propAssetIcon(propTemplateName, size)
    local preview = Preview.prop(propTemplateName, ModData.props.getHighest(propTemplateName), 1)
    preview:setPositionPolicy(Widget.PositionPolicy.CENTER)
    preview:setPosition(0, 10)
    return makeAssetIconWidget(propTemplateName, preview, size)
end

return {
    entity = entityAssetIcon,
    tile   = tileAssetIcon,
    prop   = propAssetIcon
}