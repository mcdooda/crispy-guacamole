local function dump(columns, rows, out)
    out = out or io.output()
    local function add(o)
        out:write(o)
    end

    local numColumns = #columns
    print('numColumns', numColumns)

    for i, column in ipairs(columns) do
        add('"')
        add(column)
        add('"')
        if i < numColumns then
            add(',')
        end
    end
    add('\n')

    for j, row in ipairs(rows) do
        for i, column in ipairs(columns) do
            local cell = row[column]
            local t = type(cell)
            if t == 'string' then
                add('"')
                add(cell)
                add('"')
            elseif t == 'number' then
                add(cell)
            else
                add('<' .. type(cell) .. '>')
            end
            if i < numColumns then
                add(',')
            end
        end
        add('\n')
    end
end

return {
    dump = dump
}