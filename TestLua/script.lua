local p = 1

function print_v()
    print(p)
end

function print_name(player)
    print("print name")
    lua_call_PrintName(player)
end

function print_test(...)
    local arg={...}    --> arg 为一个表，局部变量
    for k,v in ipairs(arg) do
        print("k"..k.."--> v=="..v)
    end
end