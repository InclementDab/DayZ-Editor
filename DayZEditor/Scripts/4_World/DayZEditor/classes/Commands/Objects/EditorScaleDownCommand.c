class EditorScaleDownCommand: EditorCommand
{
	override bool Execute(Class sender, CommandArgs args)
	{
		float value = m_Editor.Settings.QuickMoveStepSize * 0.1;
		if (GetGame().GetInput().LocalValue("EditorCameraSlow")) {
			value *= 0.025;
		}
		
		EditorObjectMap selected_objects = m_Editor.GetSelectedObjects();
		foreach (int id, EditorObject editor_object: selected_objects) {
			editor_object.GetWorldObject().SetScale(editor_object.GetWorldObject().GetScale() - value);
		}
		
		return true;
	}
	
	override EditorShortcutKeyType GetShortcutType()
	{
		return EditorShortcutKeyType.HOLD;
	}
}