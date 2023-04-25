-- Holds data that are shared between functions of this usertype
local test =
{
    elapsedTime = 0,
    Vec2 = Vector2.new(),
    vec22 = Vector2.new(1, 2),
    vec23 = Vector2.new(1),
    
}

-- Called when the scene starts
function test:Awake()
    Debug.Log("Awake")
    Debug.Log(tostring(self.Vec2))
    Debug.Log(tostring(self.vec22))
    Debug.Log(tostring(self.vec23))

end

-- Returns the usertype so the engine has a reference to it
return test