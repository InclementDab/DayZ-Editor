class EditorExitCommand: EditorAsyncCommand
{	
	protected override void Call(Class sender, CommandArgs args) 
	{
		DialogResult result = EditorMessageBox_DEPRECATED.Show(GetName(), "#STR_EDITOR_EXIT_MSG", MessageBoxButtons.OKCancel);
		if (result != DialogResult.OK) { 
			return;
		}
		
		if (m_Editor) {
			m_Editor.Clear();
		}	
		
		g_Game.ReportProgress("Exiting to main menu...");
		GetDayZGame().MainMenuLaunch();
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_EXIT";
	}

	override Symbols GetSymbol()
	{
		return Symbols.PERSON_TO_DOOR;
	}
}