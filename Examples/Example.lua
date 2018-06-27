

function testInstance(testNumber)
    local foo = LuaObjectTest.get()

    print("Got LuaObjectTest at adress " .. tostring(foo))

    local x = foo:add(12, 19)
    print("add(12, 19) is " .. x)

    local y = foo:sub(25.2, 20)
    print("sub(25.2, 20) is " .. y)

    print("testNumber is " .. testNumber)

    return testNumber + 6
end