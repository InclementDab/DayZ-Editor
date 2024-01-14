class EditorSelectAllCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		array<ref EditorObject> placed_objects = m_Editor.GetPlacedObjects();
		foreach (EditorObject eo: placed_objects) {
			eo.SetSelected(true);
		}
		
		return true;
	}
		
	override string GetName() 
	{
		return "#STR_EDITOR_SELECT_ALL";
	}
}