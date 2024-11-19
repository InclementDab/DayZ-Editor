enum EditorClientModuleRPC
{
	INVALID = 36114,
	COUNT
};

class EditorClientModule: JMModuleBase
{
	protected Editor m_Editor;
	
	// protected int m_KonamiCodeProgress;
	// protected float m_KonamiCodeCooldown;
	
	// static const ref array<int> KONAMI_CODE = {
	// 	KeyCode.KC_UP,
	// 	KeyCode.KC_UP,
	// 	KeyCode.KC_DOWN,
	// 	KeyCode.KC_DOWN,
	// 	KeyCode.KC_LEFT,
	// 	KeyCode.KC_RIGHT,
	// 	KeyCode.KC_LEFT,
	// 	KeyCode.KC_RIGHT,
	// 	KeyCode.KC_B,
	// 	KeyCode.KC_A
	// };
	
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
						
		// Keybinds
		
		RegisterBinding(new JMModuleBinding("OnEditorDeleteObject", "EditorDeleteObject"));
				
		RegisterBinding(new JMModuleBinding("OnEditorRotateObjectClockwise", "EditorRotateObjectClockwise"));
		RegisterBinding(new JMModuleBinding("OnEditorRotateObjectCounterClockwise", "EditorRotateObjectCounterClockwise"));
		
		RegisterBinding(new JMModuleBinding("OnEditorScaleUp", "EditorScaleUp"));
		RegisterBinding(new JMModuleBinding("OnEditorScaleDown", "EditorScaleDown"));
		
	}
	
	float m_LastAngle;
		
	override void OnUpdate(float timeslice)
	{
		if (m_Editor) {
			m_Editor.Update(timeslice);
		}
		
		// Konami suck
		/*if (m_KonamiCodeCooldown != 0) {
			m_KonamiCodeCooldown -= timeslice;
			m_KonamiCodeCooldown = Math.Clamp(m_KonamiCodeCooldown, 0, 100);
		}
		
		if (m_KonamiCodeProgress != -1 && KeyState(KONAMI_CODE[m_KonamiCodeProgress]) && m_KonamiCodeCooldown == 0) {
			m_KonamiCodeProgress++;
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(CheckKonamiCode, 1000, false, m_KonamiCodeProgress);
			m_KonamiCodeCooldown = 0.15;
		}
		
		if (m_KonamiCodeProgress >= KONAMI_CODE.Count()) {
			m_Editor.GetEditorHud().CreateNotification("Konami Code Complete!");
			m_Editor.KEgg = true;
			m_KonamiCodeProgress = -1;
		}*/
	}
	
	// private void CheckKonamiCode(int progress)
	// {
	// 	if (m_KonamiCodeProgress == progress) {
	// 		m_KonamiCodeProgress = 0;
	// 	}
	// }
	
	override bool IsServer() 
	{
		return false;
	}	
		
	override void OnMissionStart()
	{
		EditorLog.Trace("Editor::OnMissionStart");
		
		g_Game.ReportProgress("Loading Mission");
	}
	
	override void OnMissionFinish()
	{
		EditorLog.Trace("Editor::OnMissionFinish");
		Editor.Destroy();
	}
		
	override void OnMissionLoaded()
	{
		EditorLog.Trace("Editor::OnMissionLoaded");
		
		g_Game.ReportProgress("Editor Mission Loaded");
		
		vector center_pos = Editor.GetMapCenterPosition();
		PlayerBase player = Editor.CreateDefaultCharacter(GetGame().CreateRandomPlayer(), Editor.GetSafeStartPosition(center_pos[0], center_pos[2], 500));
		if (!player) {
			Error("Player was not created, exiting");
			return;
		}

		// // Check if COM is running, because that means theres crackheads afoot!
		// string com_check = "CommunityOfflineClient";
		// if (com_check.ToType()) {
		// 	EditorLog.Error("The DayZ Editor cannot run with COM enabled, disable it in your Parameters file and restart the game with ONLY the DayZ Editor");
		// 	GetGame().GetUIManager().ShowDialog("Community Offline Mode Enabled", "The DayZ Editor cannot run with COM enabled, disable it in your Parameters file and restart the game with ONLY the DayZ Editor", 76, DBT_OK, DBB_NONE, DMT_INFO, GetGame().GetUIManager().GetMenu());
		// 	return;
		// }
		
		EditorLog.Info("Loading Offline Editor...");
		m_Editor = Editor.Create(player);
		m_Editor.SetActive(true);
	}
	
	// Inputs
	private bool ShouldProcessInput(UAInput input)
	{
		// Check if LocalPress, Check if LControl is pressed, Check if game is focused
		return (m_Editor && input.LocalPress() && !KeyState(KeyCode.KC_LCONTROL) && GetGame().GetInput().HasGameFocus(INPUT_DEVICE_KEYBOARD) && (!GetFocus() || !GetFocus().IsInherited(EditBoxWidget)));
	}
	
	private bool ShouldProcessQuickInput(UAInput input)
	{
		return (m_Editor && input.LocalValue() && !KeyState(KeyCode.KC_LCONTROL) && GetGame().GetInput().HasGameFocus(INPUT_DEVICE_KEYBOARD) && (!GetFocus() || !GetFocus().IsInherited(EditBoxWidget)));
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
	
	private void OnEditorRotateObjectCounterClockwise(UAInput input)
	{
		//if (!ShouldProcessQuickInput(input)) return;
		
		float value = m_Editor.GetSettings().QuickMoveSpeed;
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
		
		float value = m_Editor.GetSettings().QuickMoveSpeed * 0.1;
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
		
		float value = m_Editor.GetSettings().QuickMoveSpeed * 0.1;
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
	