modded class PlayerBase
{	
	override void EOnFrame(IEntity other, float timeSlice)
	{
#ifndef SERVER
		if (GetDayZGame().GetEditor()) {
			GetInputController().SetDisabled(GetDayZGame().GetEditor().GetCurrentControl() != this);
		}
#endif
	}
	
	override void EEKilled(Object killer)
	{
		super.EEKilled(killer);
		
		// Quick! Before he stops breathing
		if (this == GetDayZGame().GetEditor().GetCurrentControl()) {
			GetDayZGame().GetEditor().ControlCamera(GetDayZGame().GetEditor().GetCamera());
		}
	}
}