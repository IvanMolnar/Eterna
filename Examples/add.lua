
function strTest(s)
    return s .. " from lua script"
end

function CreateCInstance()
    local Instance = {
        CData = CreateInstanceOfT();
        SomeFunc = CallSomeFuncOnT;
    }
    return Instance;
end

function testInstance()
    print(1)
    instance = CreateCInstance()
    print(2)
    local i = instance:SomeFunc()
    print(3)
    print(i)
end