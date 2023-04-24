-- Holds data that are shared between functions of this usertype
local test =
{
   elapsedTime = 0
}

-- Called when the scene starts
function test:Awake()
    Debug.Log("Awake")
end

-- Called when the scene is initialized
function test:Update(pDeltaTime)
    if Inputs.GetKey(Keys.A) then Debug.Log("a") end

end

-- Returns the usertype so the engine has a reference to it
return test