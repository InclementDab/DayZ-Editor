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
	
	void OnControlChanged(bool state)
	{
		GetInputController().SetDisabled(!state);
		
		Hud hud = GetGame().GetMission().GetHud();
		if (hud) {
			hud.ShowHudUI(g_Game.GetProfileOption(EDayZProfilesOptions.HUD) && state);
			hud.ShowQuickbarUI(g_Game.GetProfileOption(EDayZProfilesOptions.QUICKBAR) && state);
		}
		
		SetAllowDamage(!state);
	}
	
	override void EEKilled(Object killer)
	{
		super.EEKilled(killer);
		
		// Quick! Before he stops breathing
		if (this == GetEditor().GetCurrentControl()) {
			GetEditor().ControlCamera(GetEditor().GetCamera());
		}
	}
	
	// Yeet
	override void CheckDeath()
	{
	}
	
	override void OnDebugSpawn()
	{		
		array<string> shirts = {
			"TShirt_Beige",
			"TShirt_Black",
			"TShirt_Blue",
			"TShirt_Dyed",
			"TShirt_Green",
			"TShirt_OrangeWhiteStripes",
			"TShirt_Red",
			"TShirt_RedBlackStripes",
			"TShirt_White",
			"TShirt_Grey",
		};
		
		array<string> pants = {
			"CargoPants_Beige",
			"CargoPants_Black",
			"CargoPants_Blue",
			"CargoPants_Green",
			"CargoPants_Grey",
			"Jeans_Black",
			"Jeans_Blue",
			"Jeans_Brown",
			"Jeans_Green",
			"Jeans_Grey",
			"Jeans_BlueDark",
			"ShortJeans_Black",
			"ShortJeans_Blue",
			"ShortJeans_Brown",
			"ShortJeans_Darkblue",
			"ShortJeans_Green",
			"ShortJeans_Red",
		};
		
		array<string> shoes = {
			"AthleticShoes_Blue",
			"AthleticShoes_Black",
			"AthleticShoes_Brown",
			"AthleticShoes_Green",
			"AthleticShoes_Grey",
			"JoggingShoes_Black",
			"JoggingShoes_Blue",
			"JoggingShoes_Red",
			"JoggingShoes_Violet",
			"JoggingShoes_White",
		};
		
		GetInventory().CreateAttachment(shirts.GetRandomElement());
		GetInventory().CreateAttachment(pants.GetRandomElement());
		GetInventory().CreateAttachment(shoes.GetRandomElement());
	}
	
	// yeet
	override void DepleteStamina(EStaminaModifiers modifier, float dT = -1)
	{
	}
}