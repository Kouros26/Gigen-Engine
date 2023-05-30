local SpaceBox =
{
    rigidBody = nil,
    transform = nil,
}

function SpaceBox:Awake()
    self.transform = self.owner:GetTransform()
    self.transform:SetPosition(Vector3.new(10, 10, 10))
    self.transform:SetRotation(Vector3.new(0))
    self.transform:SetScale(Vector3.new(1))
        
    self.owner:CreateBoxRigidBody(Vector3.new(0.5), Vector3.new(1),1 )
    self.rigidBody = self.owner:GetRigidBody()
    self.rigidBody:SetGravity(Vector3.new(0,0,0))
end


function SpaceBox:Start()
    self.rigidBody:SetLinearVelocity(Vector3.new(0,1,0))
end

function SpaceBox:Update()
    
end

return SpaceBox

