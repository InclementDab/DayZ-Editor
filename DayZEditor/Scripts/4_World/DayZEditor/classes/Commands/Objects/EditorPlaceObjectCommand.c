class EditorPlaceObjectCommand: EditorCommand
{
	protected override void Call(Class sender, CommandArgs args)
	{
		m_Editor.PlaceObject();
	}
}