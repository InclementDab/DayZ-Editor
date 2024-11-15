class EditorPlaceObjectCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		m_Editor.PlaceObject();
		return true;
	}
	
	override string GetName()
	{
		return "#STR_EDITOR_PLACE_OBJECT";
	}
}


[RegisterEditorCommand(EditorTogglePlayerCommand)]
class EditorTogglePlayerCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		m_Editor.SetPlayerControlled(!m_Editor.IsPlayerControlled());
		return true;
	}

	override string GetName()
	{
		return "#STR_EDITOR_CMD_CONTROL_PLAYER";
	}
	
	override Symbols GetSymbol()
	{
		return Symbols.PERSON_WALKING;
	}
	
	override ShortcutKeys GetShortcut()
	{
		return { KeyCode.KC_LCONTROL, KeyCode.KC_HOME };
	}

	override LinearColor GetColor()
	{
		return LinearColor.REDDIT;
	}
	
	override bool IsToggled()
	{
		return GetEditor().IsPlayerControlled();
	}
}