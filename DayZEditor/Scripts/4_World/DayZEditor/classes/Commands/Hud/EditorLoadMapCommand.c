class EditorLoadMapCommand: EditorAsyncCommand
{	
	protected override void Call(Class sender, CommandArgs args)
	{
		EditorMapSelectDialog select_dialog = new EditorMapSelectDialog(GetName());
		string selected_map;
		DialogResult result = select_dialog.ShowDialog(selected_map);
		
		if (selected_map != string.Empty && result == DialogResult.OK) {
			EditorLog.Info("Loading Map %1", selected_map);
			m_Editor.Clear();
			g_Game.ReportProgress("Loading " + selected_map);
			GetGame().PlayMission(CreateEditorMission(selected_map));
		}
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_SELECT_MAP";
	}
}