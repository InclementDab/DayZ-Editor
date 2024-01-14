class EditorEscapeCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		if (EditorHud.CurrentDialog) {	
			EditorHud.CurrentDialog.CloseDialog();
			return true;
		}
		
		if (m_Editor.IsInventoryEditorActive()) {
			m_Editor.StopInventoryEditor();
			return true;
		}
		
		if (EditorHud.CurrentMenu) {
			delete EditorHud.CurrentMenu;
			return true;
		} 
		
		if (m_Editor.GetBrush()) {
			m_Editor.GetEditorHud().GetTemplateController().BrushToggleButton.SetState(false);
			m_Editor.SetBrush(null);
			return true;
		}
		
		if (m_Editor.IsPlacing()) {
			m_Editor.ClearHand();
			return true;
		}
		
		if (m_Editor.IsPromptedForObjectSelection()) {
			m_Editor.PromptForObjectSelection(null);
			return true;
		}
		
		if (m_Editor.GetSelectedObjects().Count() > 0) {
			m_Editor.ClearSelection();
			return true;
		}
		
		if (m_Editor.GetCameraTrackManager().GetSelectedTracks().Count() > 0) {
			m_Editor.GetCameraTrackManager().ClearSelection();
			return true;
		}
		
		if (m_Editor.GetEditorHud().IsMapVisible()) {
			m_Editor.GetEditorHud().EditorMapWidget.Show(false);
			return true;
		}
		
		if (m_Editor.IsLootEditActive()) {
			m_Editor.FinishEditLootSpawns();
			return true;
		} 

		if (g_Game.GetMission().IsPaused()) {
			m_Editor.GetEditorHud().Show(m_Editor.GetCurrentControl() == m_Editor.GetCamera());
			g_Game.GetMission().Continue();
			return true;
		} 
		
		m_Editor.GetEditorHud().Show(false);
		g_Game.GetMission().Pause();
		return true;
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_EXIT";
	}
}