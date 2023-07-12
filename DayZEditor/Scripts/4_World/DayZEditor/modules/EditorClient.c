enum EditorClientModuleRPC
{
	INVALID = 36114,
	COUNT
};

class EditorClientModule: JMModuleBase
{
	protected Editor m_Editor;
		
	void EditorClientModule() 
	{
		EditorLog.Info("EditorClientModule");
		g_Game.ReportProgress("Loading Client Module");
	}
	
	void ~EditorClientModule() 
	{
		EditorLog.Info("~EditorClientModule");
	}
	
	// JMModuleBase Overrides
	override void OnInit()
	{
		super.OnInit();
		
		EditorLog.Trace("Editor::OnInit");
					
		RegisterBinding(new JMModuleBinding("OnEditorDeleteObject", "EditorDeleteObject"));
		
		RegisterBinding(new JMModuleBinding("OnEditorMoveObjectForward", "EditorMoveObjectForward"));
		RegisterBinding(new JMModuleBinding("OnEditorMoveObjectBackward", "EditorMoveObjectBackward"));
		RegisterBinding(new JMModuleBinding("OnEditorMoveObjectLeft", "EditorMoveObjectLeft"));
		RegisterBinding(new JMModuleBinding("OnEditorMoveObjectRight", "EditorMoveObjectRight"));
		RegisterBinding(new JMModuleBinding("OnEditorMoveObjectUp", "EditorMoveObjectUp"));
		RegisterBinding(new JMModuleBinding("OnEditorMoveObjectDown", "EditorMoveObjectDown"));
		
		RegisterBinding(new JMModuleBinding("OnEditorRotateObjectClockwise", "EditorRotateObjectClockwise"));
		RegisterBinding(new JMModuleBinding("OnEditorRotateObjectCounterClockwise", "EditorRotateObjectCounterClockwise"));
		
		RegisterBinding(new JMModuleBinding("OnEditorScaleUp", "EditorScaleUp"));
		RegisterBinding(new JMModuleBinding("OnEditorScaleDown", "EditorScaleDown"));
		
	}
	
	override bool IsServer() 
	{
		return false;
	}	
	
	// Inputs
	private bool ShouldProcessInput(UAInput input)
	{
		// Check if LocalPress, Check if LControl is pressed, Check if game is focused
		return (m_Editor && input.LocalPress() && !KeyState(KeyCode.KC_LCONTROL) && GetGame().GetInput().HasGameFocus(INPUT_DEVICE_KEYBOARD) && (!GetFocus() || !GetFocus().IsInherited(EditBoxWidget));
	}
	
	private bool ShouldProcessQuickInput(UAInput input)
	{
		return (m_Editor && input.LocalValue() && !KeyState(KeyCode.KC_LCONTROL) && GetGame().GetInput().HasGameFocus(INPUT_DEVICE_KEYBOARD) && (!GetFocus() || !GetFocus().IsInherited(EditBoxWidget));
	}
		
	private void OnEditorDeleteObject(UAInput input)
	{
		if (!ShouldProcessInput(input)) return;
		EditorLog.Trace("Editor::OnEditorDeleteObject");
		
		EditorDeleteCommand command();
		CommandArgs args();
		args.Context = m_Editor.GetEditorHud();
		command.Execute(this, args);
	}
			
	private void QuickTransformObjects(vector relative_position)
	{
		EditorObjectMap selected_objects = m_Editor.GetSelectedObjects();
		foreach (int id, EditorObject editor_object: selected_objects) {
			editor_object.Position = relative_position + editor_object.GetPosition();
			editor_object.PropertyChanged("Position");
		}
	}
	
	private void OnEditorMoveObjectForward(UAInput input)
	{
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
		
		if (!ShouldProcessQuickInput(input)) return;
		//EditorLog.Trace("Editor::OnEditorMoveObjectForward");
		
		float value = m_Editor.Settings.QuickMoveStepSize;
		if (GetGame().GetInput().LocalValue("EditorCameraSlow")) {
			value *= 0.025;
		}
		
		if (m_Editor.Settings.QuickMoveFollowsCamera) {
			QuickTransformObjects(m_Editor.GetCamera().GetDirection() * value);
		} else {
			QuickTransformObjects(Vector(0, 0, value));
		}
	}

	private void OnEditorMoveObjectBackward(UAInput input)
	{
		// nothing is selected and we are actively placing
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
		
		if (!ShouldProcessQuickInput(input)) return;
		//EditorLog.Trace("Editor::OnEditorMoveObjectBackward");
		
		float value = m_Editor.Settings.QuickMoveStepSize;
		if (GetGame().GetInput().LocalValue("EditorCameraSlow")) {
			value *= 0.025;
		}
		
		if (m_Editor.Settings.QuickMoveFollowsCamera) {
			QuickTransformObjects(m_Editor.GetCamera().GetDirection() * -value);
		} else {
			QuickTransformObjects(Vector(0, 0, -value));
		}
	}
	
	private void OnEditorMoveObjectLeft(UAInput input)
	{
		if (!ShouldProcessQuickInput(input)) return;
		//EditorLog.Trace("Editor::OnEditorMoveObjectLeft");
		
		float value = m_Editor.Settings.QuickMoveStepSize;
		if (GetGame().GetInput().LocalValue("EditorCameraSlow")) {
			value *= 0.025;
		}
		
		if (m_Editor.Settings.QuickMoveFollowsCamera) {
			QuickTransformObjects(m_Editor.GetCamera().GetDirection() * vector.Up * value);
		} else {
			QuickTransformObjects(Vector(-value, 0, 0));
		}
	}	
	
	private void OnEditorMoveObjectRight(UAInput input)
	{
		if (!ShouldProcessQuickInput(input)) return;
		//EditorLog.Trace("Editor::OnEditorMoveObjectRight");
		
		float value = m_Editor.Settings.QuickMoveStepSize;
		if (GetGame().GetInput().LocalValue("EditorCameraSlow")) {
			value *= 0.025;
		}
		
		if (m_Editor.Settings.QuickMoveFollowsCamera) {
			QuickTransformObjects(m_Editor.GetCamera().GetDirection() * vector.Up * -value);
		} else {
			QuickTransformObjects(Vector(value, 0, 0));
		}
	}
	
	private void OnEditorMoveObjectUp(UAInput input)
	{
		if (!ShouldProcessQuickInput(input)) return;
		//EditorLog.Trace("Editor::OnEditorMoveObjectUp");
		
		float value = m_Editor.Settings.QuickMoveStepSize;
		if (GetGame().GetInput().LocalValue("EditorCameraSlow")) {
			value *= 0.025;
		}
		
		if (m_Editor.Settings.QuickMoveFollowsCamera) {
			QuickTransformObjects(m_Editor.GetCamera().GetDirection() * vector.Aside * value);
		} else {
			QuickTransformObjects(Vector(0, value, 0));
		}
	}	
	
	private void OnEditorMoveObjectDown(UAInput input)
	{
		if (!ShouldProcessQuickInput(input)) return;
		//EditorLog.Trace("Editor::OnEditorMoveObjectDown");
		
		float value = m_Editor.Settings.QuickMoveStepSize;
		if (GetGame().GetInput().LocalValue("EditorCameraSlow")) {
			value *= 0.025;
		}
		
		if (m_Editor.Settings.QuickMoveFollowsCamera) {
			QuickTransformObjects(m_Editor.GetCamera().GetDirection() * vector.Aside * -value);
		} else {
			QuickTransformObjects(Vector(0, -value, 0));
		}
	}
	
	private float m_LastAngle;
	
	private void OnEditorRotateObjectClockwise(UAInput input)
	{
		//if (!ShouldProcessQuickInput(input)) return;
		
		float value = m_Editor.Settings.QuickMoveStepSize;
		if (GetGame().GetInput().LocalValue("EditorCameraSlow")) {
			value *= 0.025;
		}
		
		EditorObjectMap selected_objects = m_Editor.GetSelectedObjects();
		vector projection_position = EditorObjectDragHandler.GetAveragePosition(selected_objects);
		float angle = m_LastAngle + value;
		
		foreach (int id, EditorObject editor_object: selected_objects) {
			editor_object.SetPosition(EditorMath.RotateAroundPoint(projection_position, editor_object.GetPosition(), vector.Up, Math.Cos(angle - m_LastAngle), Math.Sin(angle - m_LastAngle)));
			vector new_ori = editor_object.GetOrientation();
			new_ori[0] = new_ori[0] + ((angle - m_LastAngle) * Math.RAD2DEG);
			editor_object.SetOrientation(new_ori);
		}
		
		m_LastAngle = angle;
	}
	
	private void OnEditorRotateObjectCounterClockwise(UAInput input)
	{
		//if (!ShouldProcessQuickInput(input)) return;
		
		float value = m_Editor.Settings.QuickMoveStepSize;
		if (GetGame().GetInput().LocalValue("EditorCameraSlow")) {
			value *= 0.025;
		}
		
		EditorObjectMap selected_objects = m_Editor.GetSelectedObjects();
		vector projection_position = EditorObjectDragHandler.GetAveragePosition(selected_objects);
		float angle = m_LastAngle - value;
		
		foreach (int id, EditorObject editor_object: selected_objects) {
			editor_object.SetPosition(EditorMath.RotateAroundPoint(projection_position, editor_object.GetPosition(), vector.Up, Math.Cos(angle - m_LastAngle), Math.Sin(angle - m_LastAngle)));
			vector new_ori = editor_object.GetOrientation();
			new_ori[0] = new_ori[0] + ((angle - m_LastAngle) * Math.RAD2DEG);
			editor_object.SetOrientation(new_ori);
		}
		
		m_LastAngle = angle;
	}
	
	private void OnEditorScaleUp(UAInput input)
	{
		//if (!ShouldProcessQuickInput(input)) return;
		
		float value = m_Editor.Settings.QuickMoveStepSize * 0.1;
		if (GetGame().GetInput().LocalValue("EditorCameraSlow")) {
			value *= 0.025;
		}
		
		EditorObjectMap selected_objects = m_Editor.GetSelectedObjects();
		foreach (int id, EditorObject editor_object: selected_objects) {
			editor_object.Scale += value;
			editor_object.PropertyChanged("Scale");
		}
	}
	
	private void OnEditorScaleDown(UAInput input)
	{
		//if (!ShouldProcessQuickInput(input)) return;
		
		float value = m_Editor.Settings.QuickMoveStepSize * 0.1;
		if (GetGame().GetInput().LocalValue("EditorCameraSlow")) {
			value *= 0.025;
		}
		
		EditorObjectMap selected_objects = m_Editor.GetSelectedObjects();
		foreach (int id, EditorObject editor_object: selected_objects) {
			editor_object.Scale -= value;
			editor_object.PropertyChanged("Scale");
		}
	}
}
	