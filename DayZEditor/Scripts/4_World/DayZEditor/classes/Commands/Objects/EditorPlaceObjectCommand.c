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