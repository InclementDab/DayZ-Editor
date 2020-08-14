

class EditorServerPermissions
{
	ref set<string> Players = new set<string>();
}


modded class MissionServer
{
	private ref EditorServerPermissions m_ServerPermissions;
	
	void MissionServer()
	{
		m_ServerPermissions = new EditorServerPermissions();
		JsonFileLoader<ref EditorServerPermissions>.JsonLoadFile("$profile:Editor/players.json", m_ServerPermissions);
	}
		
	
	override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
	{
		super.InvokeOnConnect(player, identity);
		
		foreach (string player_access: m_ServerPermissions.Players) {
			if (player_access == identity.GetPlainId()) {
				GetRPCManager().SendRPC("Editor", "StartEditor", null, true, identity); 
				EditorLog.Info(string.Format("%1 is now an Editor", player_access));
			}
		}
	}
}
