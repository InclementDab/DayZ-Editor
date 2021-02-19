class EditorCloseCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args) 
	{
		DialogResult result = EditorMessageBox.Show("Close", "Are you sure?", MessageBoxButtons.OKCancel);
		
		if (result != DialogResult.OK) {
			return;
		}
		
		m_Editor.Clear();
	}
			
	override string GetName() 
	{
		return "Close";
	}
	
	override ShortcutKeys GetShortcut() 
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_W };
	}
}