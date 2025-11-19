class EditorExitCommand: EditorAsyncCommand
{	
	protected override void Call(Class sender, CommandArgs args) 
	{
		DialogResult result = EditorMessageBox_DEPRECATED.Show(GetName(), "#STR_EDITOR_EXIT_MSG", MessageBoxButtons.OKCancel);
		if (result != DialogResult.OK) { 
			return;
		}
		
		// !REMOVED: m_Editor.DestroyHud() and m_Editor.Clear()
		// Cleanup is now handled by the Editor destructor called in OnMissionFinish.
		
		g_Game.ReportProgress("Exiting to main menu...");
		
        // Defer MainMenuLaunch to the CallQueue to allow the current script frame (button click event) to resolve clean before the world is destroyed.
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(g_Game.MainMenuLaunch);
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