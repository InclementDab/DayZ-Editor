class EditorRotateObjectClockwiseCommand: EditorCommand
{
	override bool Execute(Class sender, CommandArgs args)
	{
		// BAD CODEEEE
		/*
		float value = m_Editor.Settings.QuickMoveStepSize;
		if (GetGame().GetInput().LocalValue("EditorCameraSlow")) {
			value *= 0.025;
		}
		
		array<EditorObject> selected_objects = EditorObject.SelectedObjects
		vector projection_position = EditorObjectDragHandler.GetAveragePosition(selected_objects);
		float angle = m_LastAngle + value;
		
		foreach (int id, EditorObject editor_object: selected_objects) {
			editor_object.SetPosition(EditorMath.RotateAroundPoint(projection_position, editor_object.GetPosition(), vector.Up, Math.Cos(angle - m_LastAngle), Math.Sin(angle - m_LastAngle)));
			vector new_ori = editor_object.GetOrientation();
			new_ori[0] = new_ori[0] + ((angle - m_LastAngle) * Math.RAD2DEG);
			editor_object.SetOrientation(new_ori);
		}
		
		m_LastAngle = angle;*/
		return true;
	}
	
	override EditorShortcutKeyType GetShortcutType()
	{
		return EditorShortcutKeyType.HOLD;
	}
}

class EditorRotateObjectCounterClockwiseCommand: EditorCommand
{
	override bool Execute(Class sender, CommandArgs args)
	{
		/*float value = m_Editor.Settings.QuickMoveStepSize;
		if (GetGame().GetInput().LocalValue("EditorCameraSlow")) {
			value *= 0.025;
		}
		
		array<EditorObject> selected_objects = EditorObject.SelectedObjects
		vector projection_position = EditorObjectDragHandler.GetAveragePosition(selected_objects);
		float angle = m_LastAngle - value;
		
		foreach (int id, EditorObject editor_object: selected_objects) {
			editor_object.SetPosition(EditorMath.RotateAroundPoint(projection_position, editor_object.GetPosition(), vector.Up, Math.Cos(angle - m_LastAngle), Math.Sin(angle - m_LastAngle)));
			vector new_ori = editor_object.GetOrientation();
			new_ori[0] = new_ori[0] + ((angle - m_LastAngle) * Math.RAD2DEG);
			editor_object.SetOrientation(new_ori);
		}
		
		m_LastAngle = angle;
		*/
		return true;
	}
	
	override EditorShortcutKeyType GetShortcutType()
	{
		return EditorShortcutKeyType.HOLD;
	}
}