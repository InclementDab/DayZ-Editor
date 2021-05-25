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
		
		if (m_Editor.GetEditorHud().GetTemplateController().GetCameraTrackManager().GetSelectedTracks().Count() > 0) {
			m_Editor.GetEditorHud().GetTemplateController().GetCameraTrackManager().ClearSelection();
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
			return;
		} 
		
		g_Game.GetMission().Pause();
	}
	
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_ESCAPE };
	}
	
	override string GetName() 
	{
		return "Escape";
	}
}