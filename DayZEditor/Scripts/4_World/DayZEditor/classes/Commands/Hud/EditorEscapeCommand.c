class EditorEscapeCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);		
		if (EditorHud.CurrentDialog) {	
			EditorHud.CurrentDialog.CloseDialog();
			return true;
		}
		
		if (m_Editor.GetEditorHud().GetDialog()) {
			m_Editor.GetEditorHud().GetDialog().Delete();
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
		
		if (m_Editor.IsPlacing()) {
			m_Editor.ClearHand();
			return true;
		}
		
		if (m_Editor.GetSelectedObjects().Count() > 0 || m_Editor.GetSelectedHiddenObjects().Count() > 0) {
			m_Editor.ClearSelection();
			return true;
		}
				
		if (m_Editor.GetEditorHud().IsMapVisible()) {
			m_Editor.GetEditorHud().Map.Show(false);
			return true;
		}
		
		if (m_Editor.GetControllingPlayer()) {
			m_Editor.Activate();
			return true;
		}
		
		if (m_Editor.IsPlayerControlled()) {
			m_Editor.SetPlayerControlled(false);
			return true;
		}
		
		if (m_Editor.IsLootEditActive()) {
			m_Editor.FinishEditLootSpawns();
			return true;
		} 

		if (GetGame().GetUIManager().GetMenu() && GetGame().GetUIManager().GetMenu().GetID() == MENU_INGAME) {
			GetGame().GetUIManager().Back();
			return true;
		} 
		
		//GetEditor().GetEditorHud().EnterChildMenu(MENU_INGAME);
		GetGame().GetUIManager().EnterScriptedMenu(MENU_INGAME, null);
		return true;
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