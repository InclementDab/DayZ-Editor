class EditorControlPlayerCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		Param1<EditorObject> data = Param1<EditorObject>.Cast(GetData());
		if (!data) {
			return false;
		}
		
		PlayerBase control_player = PlayerBase.Cast(data.param1.GetWorldObject());
		if (!control_player) {
			return false;
		}
		
		m_Editor.ClearSelection();
		// Enable Player
		m_Editor.ControlPlayer(control_player);
		return true;
	}
	
	override string GetName()
	{
		return "#STR_EDITOR_CMD_CONTROL_PLAYER";
	}

	override Symbols GetSymbol()
	{
		return Symbols.CHILD_REACHING;
	}
}