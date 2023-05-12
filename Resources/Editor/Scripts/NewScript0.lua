local NewScript0 = 
 { 
 } 
 
 
 function NewScript0:Start() 
   
  
 end 
 
 function NewScript0:Update(deltaTime) 
  HitResult1 = HitResult.new()

  Debug.Log(tostring(Physics.RayCast(self.owner:GetTransform():GetPosition(), Vector3.Down() * 1000, HitResult1, RayCastDebug.Timer, 15)))
  if(Physics.RayCast(self.owner:GetTransform():GetPosition(), Vector3.Down() * 1000, HitResult1, RayCastDebug.OneFrame, 15)) 
  then
      Debug.Log("Hit")
        Debug.Log(tostring(HitResult1.hitObject))
        Debug.Log(tostring(HitResult1.hitPoint))
  else
      Debug.Log("No Hit")
  end
 end 
 
 return NewScript0