class EditorQuickMoveCommand: EditorCommand
{
	override bool Execute(Class sender, CommandArgs args)
	{
		float value = GetEditor().Settings.QuickMoveStepSize;
		if (GetGame().GetInput().LocalValue("EditorCameraSlow")) {
			value *= 0.025;
		}
		
		vector direction = GetDirection() * value;
		if (GetEditor().Settings.QuickMoveFollowsCamera) {
			direction = GetEditor().GetCamera().GetDirection() * value;
		}
		
		EditorObjectMap selected_objects = GetEditor().GetSelectedObjects();
		if (selected_objects.Count() == 0) {
			return false;
		}
		
		foreach (int id, EditorObject editor_object: selected_objects) {
			editor_object.GetWorldObject().SetPosition(direction + editor_object.GetWorldObject().GetPosition());
		}
		
		return true;
	}
	
	vector GetDirection()
	{
		return vector.Zero;
	}
	
	override EditorShortcutKeyType GetShortcutType()
	{
		return EditorShortcutKeyType.HOLD;
	}
}

/*
// nothing is selected and we are actively placing
if (m_Editor && m_Editor.GetSelectedObjects().Count() == 0 && m_Editor.IsPlacing() && (input.LocalPress() || input.LocalHold())) {
	
	EditorHudController controller = m_Editor.GetEditorHud().GetTemplateController();
	auto placeables = Ternary<ObservableCollection<ref EditorPlaceableListItem>>.If(controller.CategoryConfig, controller.LeftbarSpacerConfig, controller.LeftbarSpacerStatic);
	for (int i = 0; i < placeables.Count(); i++) {
		if (placeables[i].IsSelected()) {
			if (!placeables[i - 1]) {
				return;
			}
			
			placeables[i].Deselect();
			m_Editor.AddInHand(placeables[i - 1].GetPlaceableItem());
			placeables[i - 1].Select();
			
			// Handle tooltip showing
			placeables[i].OnMouseLeave(null, null, 0, 0);
			placeables[i - 1].OnMouseEnter(null, 0, 0);
			
			controller.LeftbarScroll.VScrollToPos01((i - 1) /  placeables.Count());
			return;
		}
	}
}
*/
class EditorMoveObjectForwardCommand: EditorQuickMoveCommand
{
	override vector GetDirection()
	{
		return Vector(0, 0, 1);
	}
}

/*
if (m_Editor && m_Editor.GetSelectedObjects().Count() == 0 && m_Editor.IsPlacing() && (input.LocalPress() || input.LocalHold())) {
	EditorHudController controller = m_Editor.GetEditorHud().GetTemplateController();
	auto placeables = Ternary<ObservableCollection<ref EditorPlaceableListItem>>.If(controller.CategoryConfig, controller.LeftbarSpacerConfig, controller.LeftbarSpacerStatic);
	for (int i = 0; i < placeables.Count(); i++) {
		if (placeables[i].IsSelected()) {
			if (!placeables[i + 1]) {
				return;
			}
			
			placeables[i].Deselect();
			m_Editor.AddInHand(placeables[i + 1].GetPlaceableItem());
			placeables[i + 1].Select();
			
			// Handle tooltip showing
			placeables[i].OnMouseLeave(null, null, 0, 0);
			placeables[i + 1].OnMouseEnter(null, 0, 0);
			
			controller.LeftbarScroll.VScrollToPos01((i + 1) /  placeables.Count());
			return;
		}
	}
}
*/
class EditorMoveObjectBackwardCommand: EditorQuickMoveCommand
{
	override vector GetDirection()
	{
		return Vector(0, 0, -1);
	}
}

class EditorMoveObjectLeftCommand: EditorQuickMoveCommand
{
	override vector GetDirection()
	{
		return Vector(-1, 0, 0);
	}
}

class EditorMoveObjectRightCommand: EditorQuickMoveCommand
{
	override vector GetDirection()
	{
		return Vector(1, 0, 0);
	}
}

class EditorMoveObjectUpCommand: EditorQuickMoveCommand
{
	override vector GetDirection()
	{
		return Vector(0, 1, 0);
	}
}

class EditorMoveObjectDownCommand: EditorQuickMoveCommand
{
	override vector GetDirection()
	{
		return Vector(0, -1, 0);
	}
}


