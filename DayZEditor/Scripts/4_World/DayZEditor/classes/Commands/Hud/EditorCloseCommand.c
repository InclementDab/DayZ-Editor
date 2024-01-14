class EditorCloseCommand: EditorAsyncCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{
		DialogResult result = EditorMessageBox.Show("Close", "Are you sure?", MessageBoxButtons.OKCancel);
		
		if (result != DialogResult.OK) {
			return;
		}
		
		m_Editor.Clear();
	}
	
	override bool CanExecute()
	{
		return GetEditor().GetSaveFile() != string.Empty;
	}
			
	override string GetName() 
	{
		return "#STR_EDITOR_CLOSE";
	}
}