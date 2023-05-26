local Player = 
 { 
    camera = nil,
    transform = nil,
 } 
 
 function Player:Awake() 
     self.camera = GameManager:CreateCamera()
     self.transform = self.owner:GetTransform()
 end

 
 function Player:Start() 

 end 
 
 function Player:Update(deltaTime) 
 
 end 
 
 return Player