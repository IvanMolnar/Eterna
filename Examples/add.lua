

function testInstance()
    print(1)
    local foo = LuaObjectTest.get()
    print(2)
    print(foo)
    print(3)
    local x = foo:add(12, 19)
    print(x)
    print(4)
    local y = foo:sub(25.0, 20)
    print(y)
end