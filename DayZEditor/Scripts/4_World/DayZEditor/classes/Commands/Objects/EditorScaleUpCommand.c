class EditorScaleUpCommand: EditorCommand
{
	override bool Execute(Class sender, CommandArgs args)
	{
		float value = m_Editor.Settings.QuickMoveStepSize * 0.1;
		if (GetGame().GetInput().LocalValue("EditorCameraSlow")) {
			value *= 0.025;
		}
		
		EditorObjectMap selected_objects = m_Editor.GetSelectedObjects();
		foreach (int id, EditorObject editor_object: selected_objects) {
			editor_object.Scale += value;
			editor_object.PropertyChanged("Scale");
		}
		
		return true;
	}
}

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
			editor_object.Scale -= value;
			editor_object.PropertyChanged("Scale");
		}
		
		return true;
	}
}