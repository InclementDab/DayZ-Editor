modded class PlayerBase
{
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
	
	override void EEKilled(Object killer)
	{
		//super.EEKilled(killer);
		
		// Quick! Before he stops breathing
		if (this == PlayerBase.Cast(GetGame().GetPlayer())) {
			GetEditor().SetActive(true);
		}
	}
	
	// Yeet
	override void CheckDeath()
	{
	}
}