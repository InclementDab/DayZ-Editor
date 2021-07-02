class EditorExitCommand: EditorCommand
{	
	protected override void Call(Class sender, CommandArgs args) 
	{
		DialogResult result = EditorMessageBox.Show("Exit to main menu", "Do you want to close the DayZ Editor?", MessageBoxButtons.OKCancel);
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
		return "Exit";
	}
}