class EditorCopyPositionCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		string copy_string = string.Format("[Camera Position]: %1, %2\n", GetGame().GetCurrentCameraPosition().ToString(true), GetGame().GetCurrentCameraDirection().ToString(true));
		
		EditorObjectMap selected_objects = GetEditor().GetSelectedObjects();
		if (selected_objects.Count() > 0) {
			foreach (int id, EditorObject editor_object: selected_objects) {
				copy_string += string.Format("[%1]: pos=%2, ori=%3, dir=%4 scl=%5", editor_object.Name, editor_object.Position.ToString(true), editor_object.Orientation.ToString(true), editor_object.GetWorldObject().GetDirection().ToString(true), editor_object.Scale);
				copy_string += "\n";
			}
		}
		
		GetEditor().GetEditorHud().CreateNotification(string.Format("Copied position data to clipboard!", GetEditor().GetSelectedObjects().Count()), COLOR_GREEN);
		GetGame().CopyToClipboard(copy_string);
		return true;
	}

	override string GetName() 
	{
		return "#STR_EDITOR_CMD_COPY_POS";
	}
}