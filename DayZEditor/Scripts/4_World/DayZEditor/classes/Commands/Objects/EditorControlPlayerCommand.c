class EditorControlPlayerCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args)
	{
		Param1<EditorObject> data = Param1<EditorObject>.Cast(GetData());
		if (!data) {
			return;
		}
		
		// Set the player as the current active
		data.param1.Control = true;
		data.param1.PropertyChanged("Control");
		
		// Enable Player
		m_Editor.SetActive(false);
	}
	
	override string GetName()
	{
		return "Control Player";
	}
}