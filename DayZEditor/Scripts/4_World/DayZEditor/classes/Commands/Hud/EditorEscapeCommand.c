class EditorEscapeCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args)
	{
		if (EditorHud.CurrentDialog) {	
			EditorHud.CurrentDialog.CloseDialog();
			return;
		}
		
		if (EditorHud.CurrentMenu) {
			delete EditorHud.CurrentMenu;
			return;
		} 
		
		if (m_Editor.IsPlacing()) {
			m_Editor.StopPlacing();
			return;
		}
		
		if (m_Editor.GetSelectedObjects().Count() > 0) {
			m_Editor.ClearSelection();
			return;
		}
		
		if (m_Editor.GetCameraTrackManager().GetSelectedTracks().Count() > 0) {
			m_Editor.GetCameraTrackManager().ClearSelection();
			return;
		}
		
		if (m_Editor.GetEditorHud().IsMapVisible()) {
			m_Editor.GetEditorHud().EditorMapWidget.Show(false);
			return;
		}
		
		if (m_Editor.IsLootEditActive()) {
			m_Editor.FinishEditLootSpawns();
			return;
		} 

		if (g_Game.GetMission().IsPaused()) {
			g_Game.GetMission().Continue();
			GetEditor().GetEditorHud().Show(true);
			return;
		} 
		
		g_Game.GetMission().Pause();
		GetEditor().GetEditorHud().Show(false);
	}
	
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_ESCAPE };
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_EXIT";
	}
}