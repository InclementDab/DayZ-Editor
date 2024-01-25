modded class PlayerBase
{	
	override void EOnFrame(IEntity other, float timeSlice)
	{	
#ifndef SERVER
		if (GetDayZGame().GetEditor()) {
			GetInputController().SetDisabled(GetDayZGame().GetEditor().IsActive());
		}
#endif
	}
	
	override void EEKilled(Object killer)
	{
		super.EEKilled(killer);
#ifndef SERVER
		// Quick! Before he stops breathing
		if (GetGame().GetPlayer() == this) {
			GetDayZGame().GetEditor().GetHud().Show(true);
			GetDayZGame().GetEditor().GetCamera().SetActive(true);
		}
#endif
	}
}