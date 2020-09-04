
modded class MissionGameplay
{	
	
	override void OnInit()
	{
		super.OnInit();
				
		GetUApi().GetInputByName("UACOTModuleToggleCOT").ForceDisable(true);
		GetUApi().GetInputByName("UACOTToggleButtons").ForceDisable(true);
		GetUApi().GetInputByName("UACOTTogglePlayer").ForceDisable(true);
		GetUApi().GetInputByName("UACOTToggleCamera").ForceDisable(true);
		GetUApi().GetInputByName("UACOTToggleESP").ForceDisable(true);
		GetUApi().GetInputByName("UACOTToggleMap").ForceDisable(true);
		GetUApi().UpdateControls();
	}
	
	override void OnKeyPress(int key)
	{
		// If Editor is NOT active, just do keypress
		if (!GetEditor() || !GetEditor().IsActive()) {
			super.OnKeyPress(key);
		// If Editor IS active, and DOESNT handle the key, do keypress
		} else if (!GetEditor().OnKeyPress(key)) {
			super.OnKeyPress(key);
		}		
	}
	
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

