modded class MissionBase
{
	void MissionBase()
	{
		// Just bad vanilla code that doesnt enable this
		if (!GetGame().IsMultiplayer()) {
			m_WorldLighting	= new WorldLighting();
		}
	}
}