

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
	
	void SetPlayerAsEditor(PlayerIdentity player)
	{
		JsonFileLoader<ref EditorServerPermissions>.JsonLoadFile("$profile:Editor/players.json", m_ServerPermissions);
		foreach (string player_access: m_ServerPermissions.Players) {
			if (player_access == player.GetPlainId()) {
				GetRPCManager().SendRPC("Editor", "StartEditor", null, true, player); 
				EditorLog.Info(string.Format("%1 is now an Editor", player_access));
				return;
			}
		}
		
		EditorLog.Warning(string.Format("Failed to assign %1 as Editor!", player_access));
	}
	
	void GiveEditorPermission(PlayerIdentity player)
	{
		EditorLog.Info(string.Format("Adding %1 to Editor Permissions List", player.GetPlainId()));
		JsonFileLoader<ref EditorServerPermissions>.JsonLoadFile("$profile:Editor/players.json", m_ServerPermissions);
		
		m_ServerPermissions.Players.Insert(player.GetPlainId());
		JsonFileLoader<ref EditorServerPermissions>.JsonSaveFile("$profile:Editor/players.json", m_ServerPermissions);
	}
	
}
