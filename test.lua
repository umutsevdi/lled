-- comments begin with two dash --
print("Lua: Hello console output")
global_var = 10
local local_var = "defined in the local scope"

if global_var == 5 then
    -- something complex happens here
elseif local_var == "defined in the local scope" then
    -- something else here
else
    error("invalid operation")
end

-- add all elements of array `a'
function add(a)
    local sum = 0
    for i, v in ipairs(a) do
        sum = sum + v
    end
    return sum
end
