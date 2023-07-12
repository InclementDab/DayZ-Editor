class EditorControlPlayerCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		Param1<EditorObject> data = Param1<EditorObject>.Cast(GetData());
		if (!data) {
			return false;
		}
		
		PlayerBase player = PlayerBase.Cast(data.param1.GetWorldObject());
		if (!player) {
			return false;
		}
		
		m_Editor.ControlPlayer(player);		
		return true;
	}
	
	override string GetName()
	{
		return "#STR_EDITOR_CMD_CONTROL_PLAYER";
	}
}