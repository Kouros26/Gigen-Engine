-- Holds data that are shared between functions of this usertype
local test =
{
    elapsedTime = 0,
    Mat4 = Matrix4.new(1, 2, 3, 4,
                       5, 6, 7, 8,
                       9, 10, 11, 12,
                       13, 14, 15, 16),
    Mat44 = Matrix4.new(2),
    Mat45 = Matrix4.new(Vector4.new(1, 2, 3, 4), Vector4.new(5, 6, 7, 8), Vector4.new(9, 10, 11, 12), Vector4.new(13, 14, 15, 16)),
    Mat46 = Matrix4.new(),


}

-- Called when the script is loaded
function test:Awake()
    Debug.LogWarning("Test Matrix4")

        -- Constructors --
        -- Debug.LogError("Constructors")
        -- Debug.Log(tostring(self.Mat4))
        -- Debug.Log(tostring(self.Mat44))
        -- Debug.Log(tostring(self.Mat45))
        -- Debug.Log(tostring(self.Mat46))


        -- Operators --
        -- Debug.LogError("Operators")
        -- Debug.Log(tostring(self.Mat4 + self.Mat4))
        -- Debug.Log(tostring(self.Mat4 - self.Mat4))
        -- Debug.Log(tostring(self.Mat4 * self.Mat4))
        -- Debug.Log(tostring(self.Mat4 * 2))
        -- Debug.Log(tostring(self.Mat4 / 2))

        -- Data --
        -- Debug.LogError("Data")
        -- Debug.Log(tostring(Matrix4:Identity()))

        -- Functions --
        -- Debug.LogError("Functions")
        -- Debug.Log("Transpose: " .. tostring(Matrix4.Transpose(self.Mat4)))
        -- Debug.Log("Inverse: " .. tostring(Matrix4.Inverse(self.Mat4)))
        -- Debug.Log("XRotaion: " .. tostring(Matrix4.RotateX(self.Mat4, 90)))
        -- Debug.Log("YRotaion: " .. tostring(Matrix4.RotateY(self.Mat4, 90)))
        -- Debug.Log("ZRotaion: " .. tostring(Matrix4.RotateZ(self.Mat4, 90)))
        -- Debug.Log("Translation: " .. tostring(Matrix4.Translation(Vector3.new(1, 2, 3))))
        -- Debug.Log("Translation: " .. tostring(Matrix4.Translate(self.Mat4, Vector3.new(1, 2, 3))))
        -- Debug.Log("YXZRotation: " .. tostring(Matrix4.RotateYXZ(Vector3.new(1, 2, 3))))
        -- Debug.Log("RotationEuler: " .. tostring(Matrix4.RotationEuler(Vector3.new(1, 2, 3))))
        -- Debug.Log("Perspective: " .. tostring(Matrix4.CreatePerspective(90, 16 / 9, 0.1, 100)))






end

-- Returns the usertype so the engine has a reference to it
return test