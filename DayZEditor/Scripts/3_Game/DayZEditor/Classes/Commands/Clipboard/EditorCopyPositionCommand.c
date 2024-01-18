class EditorCopyPositionCommand: EditorCommand
{
	protected override bool Execute(Class sender, CommandArgs args)
	{
		super.Execute(sender, args);
		string copy_string = string.Format("[Camera Position]: %1, %2\n", GetGame().GetCurrentCameraPosition().ToString(true), GetGame().GetCurrentCameraDirection().ToString(true));
		
		array<EditorObject> selected_objects = EditorObject.GetSelectedEditorObjects();
		if (selected_objects.Count() > 0) {
			foreach (int id, EditorObject editor_object: selected_objects) {
				copy_string += string.Format("[%1]: pos=%2, ori=%3, dir=%4 scl=%5", editor_object.GetDisplayName(), editor_object.GetWorldObject().GetPosition().ToString(true), editor_object.GetWorldObject().GetOrientation().ToString(true), editor_object.GetWorldObject().GetDirection().ToString(true), editor_object.GetWorldObject().GetScale());
				copy_string += "\n";
			}
		}
		
		GetEditor().GetEditorHud().ShowNotification(string.Format("Copied position data to clipboard!", EditorObject.GetSelectedEditorObjects().Count()), COLOR_GREEN);
		GetGame().CopyToClipboard(copy_string);
		return true;
	}
	
	override bool CanExecute()
	{
		return EditorObject.GetSelectedEditorObjects().Count() > 0;
	}

	override string GetName() 
	{
		return "#STR_EDITOR_CMD_COPY_POS";
	}
}