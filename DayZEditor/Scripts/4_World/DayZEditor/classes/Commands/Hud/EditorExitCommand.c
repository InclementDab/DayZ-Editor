class EditorExitCommand: EditorAsyncCommand
{	
	protected override void Call(Class sender, CommandArgs args) 
	{
		DialogResult result = EditorMessageBox.Show(GetName(), "#STR_EDITOR_EXIT_MSG", MessageBoxButtons.OKCancel);
		if (result != DialogResult.OK) { 
			return;
		}
		
		if (m_Editor) {
			m_Editor.Clear();
		}
		
		GetGame().AbortMission();
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_EXIT";
	}
}