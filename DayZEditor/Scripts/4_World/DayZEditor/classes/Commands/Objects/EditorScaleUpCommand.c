class EditorScaleUpCommand: EditorCommand
{
	override bool Execute(Class sender, CommandArgs args)
	{
		float value = m_Editor.Settings.QuickMoveStepSize * 0.1;
		if (GetGame().GetInput().LocalValue("EditorCameraSlow")) {
			value *= 0.025;
		}
		
		array<EditorObject> selected_objects = EditorObject.SelectedObjects
		foreach (int id, EditorObject editor_object: selected_objects) {
			editor_object.GetWorldObject().SetScale(editor_object.GetWorldObject().GetScale() + value);
		}
		
		return true;
	}
	
	override EditorShortcutKeyType GetShortcutType()
	{
		return EditorShortcutKeyType.HOLD;
	}
}