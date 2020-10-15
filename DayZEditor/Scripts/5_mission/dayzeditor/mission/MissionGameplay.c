
modded class MissionGameplay
{	
	
	override void OnInit()
	{
		super.OnInit();
				/*
		GetUApi().GetInputByName("UACOTModuleToggleCOT").ForceDisable(true);
		GetUApi().GetInputByName("UACOTToggleButtons").ForceDisable(true);
		GetUApi().GetInputByName("UACOTTogglePlayer").ForceDisable(true);
		GetUApi().GetInputByName("UACOTToggleCamera").ForceDisable(true);
		GetUApi().GetInputByName("UACOTToggleESP").ForceDisable(true);
		GetUApi().GetInputByName("UACOTToggleMap").ForceDisable(true);
		GetUApi().UpdateControls();*/
	}
	
	override void OnKeyPress(int key)
	{	
		// If Editor is NOT active, just do keypress
		if (!GetEditor() || !GetEditor().IsActive()) {
			super.OnKeyPress(key);
		} 
		
		// If Editor IS active, and DOESNT handle the key, do keypress
		else if (!GetEditor().OnKeyPress(key)) {
			super.OnKeyPress(key);
		}		
	}
	
	override void OnKeyRelease(int key)
	{
		// If Editor is NOT active, just do keypress
		if (!GetEditor() || !GetEditor().IsActive()) {			
			super.OnKeyRelease(key);
		} 
		
		// If Editor IS active, and DOESNT handle the key, do keypress
		else if (!GetEditor().OnKeyRelease(key)) {
			super.OnKeyRelease(key);
		}	
	}
	
	override void OnMouseButtonPress(int button)
	{
		// If Editor is NOT active, just do MouseDown
		if (!GetEditor() || !GetEditor().IsActive()) {			
			super.OnMouseButtonPress(button);
			// If Editor IS active, and DOESNT handle the mouse, do mousedown
		} else if (!GetEditor().OnMouseDown(button)) {
			super.OnMouseButtonPress(button);
		}
	}
	//override void OnMouseButtonRelease(int button){}

	
	override void OnUpdate(float timeslice)
	{
		if (GetEditor() && GetEditor().IsActive()) {
			GetModuleManager().OnUpdate(timeslice);
			//GetEditor().OnUpdate(timeslice);
		} else { 
			super.OnUpdate(timeslice);
		}
	}

}

