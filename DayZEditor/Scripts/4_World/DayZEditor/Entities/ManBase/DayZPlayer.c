modded class PlayerBase
{
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