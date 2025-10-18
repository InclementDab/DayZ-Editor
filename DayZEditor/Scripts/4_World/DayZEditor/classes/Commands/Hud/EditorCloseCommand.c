class EditorCloseCommand: EditorAsyncCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{
		DialogResult result = EditorMessageBox_DEPRECATED.Show("Close", "Are you sure?", MessageBoxButtons.OKCancel);
		
		if (result != DialogResult.OK) {
			return;
		}
		
		m_Editor.Clear();
	}
			
	override string GetName() 
	{
		return "#STR_EDITOR_CLOSE";
	}
	
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_W };
	}

	override Symbols GetSymbol()
	{
		return Symbols.XMARK;
	}
	
	override bool CanExecute()
	{
		return GetEditor().GetSaveFile() != string.Empty;
	}
}