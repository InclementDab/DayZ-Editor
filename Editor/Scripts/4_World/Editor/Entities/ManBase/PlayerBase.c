modded class PlayerBase
{	
	static PlayerBase s_LastControlledPlayer;
	
	static map<string, int> GetEmoteList()
	{
		map<string, int> emotes();
		typename type = EmoteConstants;
		for (int i = 0; i < type.GetVariableCount(); i++) {
			int value;
			type.GetVariableValue(null, i, value);
			emotes[type.GetVariableName(i)] = value;
		}
		
		return emotes;
	}
		
	override void EOnFrame(IEntity other, float timeSlice)
	{
#ifndef SERVER
		if (GetDayZGame().GetEditor()) {
			//GetInputController().SetDisabled(GetDayZGame().GetEditor().GetCurrentControl() != this);
		}
#endif
	}
		
	override void OnSelectPlayer()
	{
		super.OnSelectPlayer();
		
		s_LastControlledPlayer = this;
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