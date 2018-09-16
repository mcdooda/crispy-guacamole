local ScriptRuntime = flat.require 'graph/script/scriptruntime'
local SpriteComponentNode = require 'data/graph/script/nodes/components/spritecomponentnode'
local Preview = require 'data/scripts/preview'

local SpriteAnimationNode = {}

function SpriteAnimationNode.build(spriteAnimationNode, nodeWidget, pinsWidget)
    local previewContainer = Widget.makeColumnFlow()
    pinsWidget:addChild(previewContainer)
    nodeWidget.previewContainer = previewContainer
    return true
end

local function getSpriteComponentNode(script)
    local nodes = script:getNodes()
    for i = 1, #nodes do
        local node = nodes[i]
        if getmetatable(node) == SpriteComponentNode then
            return node
        end
    end
end

function SpriteAnimationNode.update(spriteAnimationNode, nodeWidget, pinsWidget)
    nodeWidget.previewContainer:removeAllChildren()

    -- do not crash if an error occured while running the graph
    pcall(function()
        local mainWindow = nodeWidget.mainWindow
        local script = mainWindow:getCurrentGraph()
        local scriptRuntime = ScriptRuntime:new(script)
        local spriteComponentNode = getSpriteComponentNode(script)
        if spriteComponentNode then
            local spriteComponentNodeRuntime = scriptRuntime:getNodeRuntime(spriteComponentNode)
            local spriteComponentTemplate = spriteComponentNodeRuntime:tryReadFromOutputPin(spriteComponentNode.componentOutPin)
            if spriteComponentTemplate then
                local spriteAnimationNodeRuntime = scriptRuntime:getNodeRuntime(spriteAnimationNode)
                local spriteAnimationName = spriteAnimationNodeRuntime:readOptionalPin(spriteAnimationNode.nameInPin)
                if spriteAnimationName then
                    local entityTemplateName = mainWindow.metadata.entityTemplateName
                    local animationPreview = Preview.sprite(entityTemplateName, spriteComponentTemplate, spriteAnimationName, true)
                    -- we did it!
                    nodeWidget.previewContainer:addChild(animationPreview)
                end
            end
        end
    end)
    return true
end

return SpriteAnimationNode