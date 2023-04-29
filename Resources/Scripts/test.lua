-- Holds data that are shared between functions of this usertype
local test =
{
    elapsedTime = 0,
    Quat4 = Quaternion.new(),
    Quat = Quaternion.new(1, 2, 3, 4),
    Quat3 = Quaternion.new(1,2,3),
    Quat2 = Quaternion.new(Vector3.new(1, 2, 3), 4),
    Quat5 = Quaternion.new(Matrix3.new(1)),

    


}

-- Called when the script is loaded
function test:Awake()
    Debug.LogWarning("Test Matrix4")

        -- Constructors --
        -- Debug.LogError("Constructors")
        -- Debug.Log(tostring(self.Quat4))
        -- Debug.Log(tostring(self.Quat))
        -- Debug.Log(tostring(self.Quat3))
        -- Debug.Log(tostring(self.Quat2))
        -- Debug.Log(tostring(self.Quat5))



        -- Operators --
        -- Debug.LogError("Operators")
        -- Debug.Log("Addition: " .. tostring(self.Quat + self.Quat))
        -- Debug.Log("Subtraction: " .. tostring(self.Quat - self.Quat))
        -- Debug.Log("Multiplication Quat: " .. tostring(self.Quat * self.Quat))
        -- Debug.Log("Multiplication Float: " .. tostring(self.Quat * 2))
        -- Debug.Log("Multiplication Vec3: " .. tostring(self.Quat * Vector3.new(1, 2, 3)))
        -- Debug.Log("Multiplication Vec4: " .. tostring(self.Quat * Vector4.new(1, 2, 3)))
        -- Debug.Log("Division Float: " .. tostring(self.Quat / 2))


        -- Data --
        -- Debug.LogError("Data")
        -- Debug.Log(tostring(Quaternion:Identity()))

        -- Functions --
        -- Debug.LogError("Functions")
        -- Debug.Log("Length: " .. tostring(Quaternion.Length(self.Quat)))
        -- Debug.Log("Normalize: " .. tostring(Quaternion.Normalize(self.Quat)))
        -- Debug.Log("Conjugate: " .. tostring(Quaternion.Conjugate(self.Quat)))
        -- Debug.Log("Inverse: " .. tostring(Quaternion.Inverse(self.Quat)))
        -- Debug.Log("Dot: " .. tostring(Quaternion.Dot(self.Quat, self.Quat2)))
        -- Debug.Log("Slerp: " .. tostring(Quaternion.Slerp(self.Quat, self.Quat2 ,0.5)))
        -- Debug.Log("Cross: " .. tostring(Quaternion.Cross(self.Quat, self.Quat2)))
        -- Debug.Log("NLerp: " .. tostring(Quaternion.NLerp(self.Quat, self.Quat2, 0.5)))
        -- Debug.Log("isUnit: " .. tostring(self.Quat4:IsUnit()))
        -- Debug.Log("GetAngle: " .. tostring(self.Quat3:GetAngle()))
        -- Debug.Log("GetAngle with quat: " .. tostring(Quaternion.GetAngle(self.Quat3)))
        -- Debug.Log("Rotate Quat & Vector3: " .. tostring(Quaternion.Rotate(self.Quat, Vector3.new(1, 2, 3))))
        -- Debug.Log("Rotate Quat & Quat: " .. tostring(Quaternion.Rotate(self.Quat, self.Quat)))
        -- Debug.Log("Rotate Quat & Vector3 & Float: " .. tostring(Quaternion.Rotate(self.Quat, Vector3.new(1, 2, 3), 0.5)))
        -- Debug.Log("FromEuler Float & Float & Float: " .. tostring(Quaternion.FromEuler(1, 2, 3)))
        -- Debug.Log("FromEuler Vector3: " .. tostring(Quaternion.FromEuler(Vector3.new(1, 2, 3))))    
        -- Debug.Log("ToMat3: " .. tostring(Quaternion.ToMat3(self.Quat)))
        -- Debug.Log("FromMat3: " .. tostring(Quaternion.FromMat3(Matrix3.new(1))))
        -- Debug.Log("Lerp: " .. tostring(Quaternion.Lerp(self.Quat, self.Quat2, 0.5)))

end

-- Returns the usertype so the engine has a reference to it
return test