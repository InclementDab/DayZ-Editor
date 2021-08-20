enum EditorClientModuleRPC
{
	INVALID = 36114,
	COUNT
};

class EditorClientModule: JMModuleBase
{
	protected Editor m_Editor;
	
	protected int m_KonamiCodeProgress;
	protected float m_KonamiCodeCooldown;
	
	static const ref array<int> KONAMI_CODE = {
		KeyCode.KC_UP,
		KeyCode.KC_UP,
		KeyCode.KC_DOWN,
		KeyCode.KC_DOWN,
		KeyCode.KC_LEFT,
		KeyCode.KC_RIGHT,
		KeyCode.KC_LEFT,
		KeyCode.KC_RIGHT,
		KeyCode.KC_B,
		KeyCode.KC_A
	};
	
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
		EditorLog.Trace("Editor::OnInit");
						
		// Keybinds
		RegisterBinding(new JMModuleBinding("OnEditorToggleActive", "EditorToggleActive"));
		RegisterBinding(new JMModuleBinding("OnEditorToggleCursor", "EditorToggleCursor"));
		RegisterBinding(new JMModuleBinding("OnEditorToggleUI", "EditorToggleUI"));
		RegisterBinding(new JMModuleBinding("OnEditorTeleportPlayerToCursor", "EditorTeleportPlayerToCursor"));
		
		RegisterBinding(new JMModuleBinding("OnEditorToggleMap", "EditorToggleMap"));
		RegisterBinding(new JMModuleBinding("OnEditorDeleteObject", "EditorDeleteObject"));
		
		RegisterBinding(new JMModuleBinding("OnEditorMoveObjectForward", "EditorMoveObjectForward"));
		RegisterBinding(new JMModuleBinding("OnEditorMoveObjectBackward", "EditorMoveObjectBackward"));
		RegisterBinding(new JMModuleBinding("OnEditorMoveObjectLeft", "EditorMoveObjectLeft"));
		RegisterBinding(new JMModuleBinding("OnEditorMoveObjectRight", "EditorMoveObjectRight"));
		RegisterBinding(new JMModuleBinding("OnEditorMoveObjectUp", "EditorMoveObjectUp"));
		RegisterBinding(new JMModuleBinding("OnEditorMoveObjectDown", "EditorMoveObjectDown"));
	}
		
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
			m_Editor.GetEditorHud().CreateNotification("Konami Code Complete!", ARGB(255, 255, 0, 255));
			m_Editor.KEgg = true;
			m_KonamiCodeProgress = -1;
		}*/
		
		/*
		if (m_Editor && m_Editor.GetCamera() && !IsMissionOffline()) {
			ScriptRPC update_rpc = new ScriptRPC();
			update_rpc.Write(m_Editor.GetCamera().GetPosition());
			update_rpc.Write(m_Editor.GetCamera().GetOrientation());
			//update_rpc.Send(null, EditorServerModuleRPC.EDITOR_CLIENT_UPDATE, true);
		}*/
	}
	
	private void CheckKonamiCode(int progress)
	{
		if (m_KonamiCodeProgress == progress) {
			m_KonamiCodeProgress = 0;
		}
	}
	
	override bool IsServer() 
	{
		return false;
	}	
		
	override void OnMissionStart()
	{
		EditorLog.Trace("Editor::OnMissionStart");
		
		g_Game.ReportProgress("Loading Mission");
		vector center_pos = Editor.GetMapCenterPosition();
		PlayerBase player = Editor.CreateDefaultCharacter(Editor.GetSafeStartPosition(center_pos[0], center_pos[2], 500));
		if (!player) {
			g_Game.ReportProgress("Failed to create player, contact InclementDab");
			Error("Player was not created, exiting");
			return;
		}
		
		GetGame().SelectPlayer(null, player);
	}
	
	override void OnMissionFinish()
	{
		EditorLog.Trace("Editor::OnMissionFinish");
		Editor.Destroy();
	}
		
	override void OnMissionLoaded()
	{
		EditorLog.Trace("Editor::OnMissionLoaded");
		
		g_Game.ReportProgress("Mission Loaded");
		
		// Check if COM is running, because that means theres crackheads afoot!
		string com_check = "CommunityOfflineClient";
		if (com_check.ToType()) {
			EditorLog.Error("The DayZ Editor cannot run with COM enabled, disable it in your Parameters file and restart the game with ONLY the DayZ Editor");
			GetGame().GetUIManager().ShowDialog("Community Offline Mode Enabled", "The DayZ Editor cannot run with COM enabled, disable it in your Parameters file and restart the game with ONLY the DayZ Editor", 76, DBT_OK, DBB_NONE, DMT_INFO, GetGame().GetUIManager().GetMenu());
			return;
		}
		
		EditorLog.Info("Loading Offline Editor...");
		m_Editor = Editor.Create(PlayerBase.Cast(GetGame().GetPlayer()));
		m_Editor.SetActive(true);
	}
	
	// Inputs
	private bool ShouldProcessInput(UAInput input)
	{
		// Check if LocalPress, Check if LControl is pressed, Check if game is focused
		return (m_Editor && input.LocalPress() && !KeyState(KeyCode.KC_LCONTROL) && GetGame().GetInput().HasGameFocus());
	}
	
	private bool ShouldProcessQuickInput(UAInput input)
	{
		return (m_Editor && input.LocalValue() && !KeyState(KeyCode.KC_LCONTROL) && GetGame().GetInput().HasGameFocus());
	}
	
	private void OnEditorToggleActive(UAInput input)
	{
		if (!ShouldProcessInput(input)) return;
		EditorLog.Trace("Editor::OnEditorToggleActive");
		
		string ban_reason;
		if (m_Editor.IsBannedClient(ban_reason)) {
			m_Editor.ShowBanDialog(ban_reason);
			return;
		}
		
		bool active = m_Editor.IsActive(); // weird syntax bug?
		m_Editor.SetActive(!active);
	}	
	
	private void OnEditorToggleCursor(UAInput input)
	{
		if (!ShouldProcessInput(input)) return;
		EditorLog.Trace("Editor::OnEditorToggleCursor");
		
		if (!m_Editor.IsActive() && !m_Editor.IsInventoryEditorActive()) {
			return;
		}
		
		// Dont want to toggle cursor on map
		if (m_Editor.GetEditorHud().EditorMapWidget.IsVisible() || (EditorHud.CurrentDialog && m_Editor.Settings.LockCameraDuringDialogs)) {
			return;
		}
		
		m_Editor.GetEditorHud().ToggleCursor();
	}	
	
	private void OnEditorToggleUI(UAInput input)
	{		
		if (!ShouldProcessInput(input)) return;
		EditorLog.Trace("Editor::OnEditorToggleUI");
		
		string ban_reason;
		if (m_Editor.IsBannedClient(ban_reason)) {
			m_Editor.ShowBanDialog(ban_reason);
			return;
		}
		
		if (m_Editor.IsInventoryEditorActive()) {
			m_Editor.GetInventoryEditorHud().GetLayoutRoot().Show(!m_Editor.GetInventoryEditorHud().GetLayoutRoot().IsVisible());
			return;
		}
		
		m_Editor.GetEditorHud().Show(!m_Editor.GetEditorHud().IsVisible());
		
		EditorObjectMap placed_objects =  m_Editor.GetPlacedObjects();
		foreach (int id, EditorObject editor_object: placed_objects) {
			EditorObjectMarker marker = editor_object.GetMarker();
			if (marker) {
				marker.Show(m_Editor.GetEditorHud().IsVisible());
			}
		}
		
		// If player is active
		if (!m_Editor.IsActive()) {
			m_Editor.GetEditorHud().ShowCursor(m_Editor.GetEditorHud().IsVisible());
			
			// A wacky way to disable motion while the UI is enabled
			if (GetGame().GetPlayer()) {
				GetGame().GetPlayer().DisableSimulation(m_Editor.GetEditorHud().IsVisible());
			}
		}
	}
	
	private void OnEditorToggleMap(UAInput input)
	{
		if (!ShouldProcessInput(input)) return;
		EditorLog.Trace("Editor::OnEditorToggleMap");
		
		if (!m_Editor.GetEditorHud().IsVisible()) return;
		
		EditorHud editor_hud = m_Editor.GetEditorHud();
		editor_hud.EditorMapWidget.Show(!editor_hud.EditorMapWidget.IsVisible());
		editor_hud.ShowCursor(true);
		
		EditorEvents.MapToggled(this, m_Editor.GetEditorHud().EditorMapWidget, m_Editor.GetEditorHud().EditorMapWidget.IsVisible());
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
	
	private void OnEditorTeleportPlayerToCursor(UAInput input)
	{		
		if (!ShouldProcessInput(input)) return;
		EditorLog.Trace("Editor::OnEditorTeleportPlayerToCursor");
				
		m_Editor.TeleportPlayerToCursor();
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
			ObservableCollection<ref EditorPlaceableListItem> placeables = m_Editor.GetEditorHud().GetTemplateController().LeftbarSpacerData;
			for (int i = 0; i < placeables.Count(); i++) {
				if (placeables[i].IsSelected()) {
					if (!placeables[i - 1]) {
						return;
					}
					
					placeables[i].Deselect();
					m_Editor.CreateInHand(placeables[i - 1].GetPlaceableItem());
					placeables[i - 1].Select();
					
					m_Editor.GetEditorHud().GetTemplateController().LeftbarScroll.VScrollToPos01((i - 1) /  placeables.Count());
					return;
				}
			}
		}
		
		if (!ShouldProcessQuickInput(input)) return;
		//EditorLog.Trace("Editor::OnEditorMoveObjectForward");
		
		float value = m_Editor.Settings.QuickMoveStepSize;
		if (GetGame().GetInput().LocalValue("EditorCameraTurbo")) {
			value *= 0.01;
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
			ObservableCollection<ref EditorPlaceableListItem> placeables = m_Editor.GetEditorHud().GetTemplateController().LeftbarSpacerData;
			for (int i = 0; i < placeables.Count(); i++) {
				if (placeables[i].IsSelected()) {
					if (!placeables[i + 1]) {
						return;
					}
					
					placeables[i].Deselect();
					m_Editor.CreateInHand(placeables[i + 1].GetPlaceableItem());
					placeables[i + 1].Select();
					
					m_Editor.GetEditorHud().GetTemplateController().LeftbarScroll.VScrollToPos01((i + 1) /  placeables.Count());
					return;
				}
			}
		}
		
		if (!ShouldProcessQuickInput(input)) return;
		//EditorLog.Trace("Editor::OnEditorMoveObjectBackward");
		
		float value = m_Editor.Settings.QuickMoveStepSize;
		if (GetGame().GetInput().LocalValue("EditorCameraTurbo")) {
			value *= 0.01;
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
		if (GetGame().GetInput().LocalValue("EditorCameraTurbo")) {
			value *= 0.01;
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
		if (GetGame().GetInput().LocalValue("EditorCameraTurbo")) {
			value *= 0.01;
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
		if (GetGame().GetInput().LocalValue("EditorCameraTurbo")) {
			value *= 0.01;
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
		if (GetGame().GetInput().LocalValue("EditorCameraTurbo")) {
			value *= 0.01;
		}
		
		if (m_Editor.Settings.QuickMoveFollowsCamera) {
			QuickTransformObjects(m_Editor.GetCamera().GetDirection() * vector.Aside * -value);
		} else {
			QuickTransformObjects(Vector(0, -value, 0));
		}
	}
	
	// RPC stuff
	override int GetRPCMin() 
	{
		return EditorClientModuleRPC.INVALID;
	}

	override int GetRPCMax()
	{
		return EditorClientModuleRPC.COUNT;
	}
	
	override void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ref ParamsReadContext ctx)
	{
		switch (rpc_type) {
			
		
		}
	}
}
	