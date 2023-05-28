class EditorControlPlayerCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		Param1<EditorObject> data = Param1<EditorObject>.Cast(GetData());
		if (!data) {
			return false;
		}
		
		// Set the player as the current active
		data.param1.Control = true;
		data.param1.PropertyChanged("Control");
		
		// Enable Player
		m_Editor.SetActive(false);
		return true;
	}
	
	override string GetName()
	{
		return "#STR_EDITOR_CMD_CONTROL_PLAYER";
	}
}