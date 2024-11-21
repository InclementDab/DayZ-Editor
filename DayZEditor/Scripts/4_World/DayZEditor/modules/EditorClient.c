class EditorClientModule: JMModuleBase
{
	protected Editor m_Editor;
				
	override void OnUpdate(float timeslice)
	{
		if (m_Editor) {
			m_Editor.Update(timeslice);
		}
	}
		
	override bool IsServer() 
	{
		return false;
	}	
		
	override void OnMissionFinish()
	{
		Editor.Destroy();
	}
		
	override void OnMissionLoaded()
	{
		g_Game.ReportProgress("Editor Mission Loaded");
		
		vector center_pos = Editor.GetMapCenterPosition();
		PlayerBase player = Editor.CreateDefaultCharacter(GetGame().CreateRandomPlayer(), Editor.GetSafeStartPosition(center_pos[0], center_pos[2], 500));
		if (!player) {
			Error("Player was not created, exiting");
			return;
		}

		// // Check if COM is running, because that means theres crackheads afoot!
		// string com_check = "CommunityOfflineClient";
		// if (com_check.ToType()) {
		// 	EditorLog.Error("The DayZ Editor cannot run with COM enabled, disable it in your Parameters file and restart the game with ONLY the DayZ Editor");
		// 	GetGame().GetUIManager().ShowDialog("Community Offline Mode Enabled", "The DayZ Editor cannot run with COM enabled, disable it in your Parameters file and restart the game with ONLY the DayZ Editor", 76, DBT_OK, DBB_NONE, DMT_INFO, GetGame().GetUIManager().GetMenu());
		// 	return;
		// }
		
		EditorLog.Info("Loading Offline Editor...");
		m_Editor = Editor.Create(player);
		m_Editor.SetActive(true);
	}
}
	