enum EditorClientModuleRPC
{
	INVALID = 36114,
	COUNT
};

class EditorClientModule: JMModuleBase
{
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
		if (GetEditor()) {
			GetEditor().Update(timeslice);
		}
		/*
		if (GetEditor() && GetEditor().GetCamera() && !IsMissionOffline()) {
			ScriptRPC update_rpc = new ScriptRPC();
			update_rpc.Write(GetEditor().GetCamera().GetPosition());
			update_rpc.Write(GetEditor().GetCamera().GetOrientation());
			//update_rpc.Send(null, EditorServerModuleRPC.EDITOR_CLIENT_UPDATE, true);
		}*/
	}
	
	override bool IsServer() 
	{
		return false;
	}	
		
	override void OnMissionStart()
	{
		EditorLog.Trace("Editor::OnMissionStart");		
		
		if (IsMissionOffline()) {
			EditorLog.Info("Loading Offline Editor...");
			g_Game.ReportProgress("Creating Mission");
			vector center_pos = Editor.GetMapCenterPosition();
			PlayerBase player = Editor.CreateDefaultCharacter(Editor.GetSafeStartPosition(center_pos[0], center_pos[2], 500));
			if (!player) {
				g_Game.ReportProgress("Failed to create player, contact InclementDab");
				Error("Player was not created, exiting");
				return;
			}
			
			GetGame().SelectPlayer(null, player);
			Editor.Create(PlayerBase.Cast(GetGame().GetPlayer()));
		} else {
			EditorLog.Info("Loading Online Editor...");
			Editor.Create(PlayerBase.Cast(GetGame().GetPlayer()));
		}
	}
	
	override void OnMissionFinish()
	{
		EditorLog.Trace("Editor::OnMissionFinish");
		Editor.Destroy();
	}
		
	override void OnMissionLoaded()
	{
		EditorLog.Trace("Editor::OnMissionLoaded");
		GetEditor().SetActive(true);
		
		// Just resolves like 37 different issues surrounding DayZ and DayZ related titles
		GetEditor().SetActive(false);
		GetEditor().SetActive(true);
	}
	
	// Inputs
	private bool ShouldProcessInput(UAInput input)
	{
		// Check if LocalPress, Check if LControl is pressed, Check if game is focused
		return (GetEditor() && input.LocalPress() && !KeyState(KeyCode.KC_LCONTROL) && GetGame().GetInput().HasGameFocus());
	}
	
	private bool ShouldProcessQuickInput(UAInput input)
	{
		return (GetEditor() && input.LocalValue() && !KeyState(KeyCode.KC_LCONTROL) && GetGame().GetInput().HasGameFocus());
	}
	
	private bool m_IsActive;
	private void OnEditorToggleActive(UAInput input)
	{
		if (!ShouldProcessInput(input)) return;
		EditorLog.Trace("Editor::OnEditorToggleActive");
		
		string ban_reason;
		if (GetEditor().IsBannedClient(ban_reason)) {
			GetEditor().ShowBanDialog(ban_reason);
			return;
		}
				
		GetEditor().SetActive(m_IsActive);
		m_IsActive = !m_IsActive;
	}	
	
	private void OnEditorToggleCursor(UAInput input)
	{
		if (!ShouldProcessInput(input)) return;
		EditorLog.Trace("Editor::OnEditorToggleCursor");
		
		// Dont want to toggle cursor on map
		if (!GetEditor().IsActive() || GetEditor().GetEditorHud().EditorMapWidget.IsVisible() || EditorHud.CurrentDialog) {
			return;
		}
		
		GetEditor().GetEditorHud().ToggleCursor();
	}	
	

	private void OnEditorToggleUI(UAInput input)
	{		
		if (!ShouldProcessInput(input)) return;
		EditorLog.Trace("Editor::OnEditorToggleUI");
		
		string ban_reason;
		if (GetEditor().IsBannedClient(ban_reason)) {
			GetEditor().ShowBanDialog(ban_reason);
			return;
		}
		
		GetEditor().GetEditorHud().Show(!GetEditor().GetEditorHud().IsVisible());
		
		EditorObjectMap placed_objects =  GetEditor().GetPlacedObjects();
		foreach (int id, EditorObject editor_object: placed_objects) {
			EditorObjectMarker marker = editor_object.GetMarker();
			if (marker) {
				marker.Show(GetEditor().GetEditorHud().IsVisible());
			}
		}
		
		// If player is active
		if (!GetEditor().IsActive()) {
			GetEditor().GetEditorHud().ShowCursor(GetEditor().GetEditorHud().IsVisible());
			
			// A wacky way to disable motion while the UI is enabled
			if (GetGame().GetPlayer()) {
				GetGame().GetPlayer().DisableSimulation(GetEditor().GetEditorHud().IsVisible());
			}
		}
	}
	
	private void OnEditorToggleMap(UAInput input)
	{
		if (!ShouldProcessInput(input)) return;
		EditorLog.Trace("Editor::OnEditorToggleMap");
		
		if (!GetEditor().GetEditorHud().IsVisible()) return;
		
		EditorHud editor_hud = GetEditor().GetEditorHud();
		editor_hud.EditorMapWidget.Show(!editor_hud.EditorMapWidget.IsVisible());
		editor_hud.ShowCursor(true);
		
		EditorEvents.MapToggled(this, GetEditor().GetEditorHud().EditorMapWidget, GetEditor().GetEditorHud().EditorMapWidget.IsVisible());
	}	
	
	private void OnEditorDeleteObject(UAInput input)
	{
		if (!ShouldProcessInput(input)) return;
		EditorLog.Trace("Editor::OnEditorDeleteObject");
		
		EditorDeleteCommand command();
		CommandArgs args();
		args.Context = GetEditor().GetEditorHud();
		command.Execute(this, args);
	}
	
	private void OnEditorTeleportPlayerToCursor(UAInput input)
	{		
		if (!ShouldProcessInput(input)) return;
		EditorLog.Trace("Editor::OnEditorTeleportPlayerToCursor");
				
		GetEditor().TeleportPlayerToCursor();
	}
		
	private void QuickTransformObjects(vector relative_position)
	{
		EditorObjectMap selected_objects = GetEditor().GetSelectedObjects();
		foreach (int id, EditorObject editor_object: selected_objects) {
			editor_object.Position = relative_position + editor_object.GetPosition();
			editor_object.PropertyChanged("Position");
		}
	}
	
	private void OnEditorMoveObjectForward(UAInput input)
	{
		// nothing is selected and we are actively placing
		if (GetEditor() && GetEditor().GetSelectedObjects().Count() == 0 && GetEditor().IsPlacing() && input.LocalPress()) {
			ObservableCollection<ref EditorPlaceableListItem> placeables = GetEditor().GetEditorHud().GetTemplateController().LeftbarSpacerData;
			for (int i = 0; i < placeables.Count(); i++) {
				if (placeables[i].IsSelected()) {
					if (!placeables[i - 1]) {
						return;
					}
					
					placeables[i].Deselect();
					GetEditor().CreateInHand(placeables[i - 1].GetPlaceableItem());
					placeables[i - 1].Select();
					return;
				}
			}
		}
		
		if (!ShouldProcessQuickInput(input)) return;
		EditorLog.Trace("Editor::OnEditorMoveObjectForward");
		
		float value = 0.1;
		if (GetGame().GetInput().LocalValue("EditorCameraTurbo")) {
			value = 0.001;
		}
		
		QuickTransformObjects(Vector(0, 0, value));
	}

	private void OnEditorMoveObjectBackward(UAInput input)
	{
		// nothing is selected and we are actively placing
		if (GetEditor() && GetEditor().GetSelectedObjects().Count() == 0 && GetEditor().IsPlacing() && input.LocalPress()) {
			ObservableCollection<ref EditorPlaceableListItem> placeables = GetEditor().GetEditorHud().GetTemplateController().LeftbarSpacerData;
			for (int i = 0; i < placeables.Count(); i++) {
				if (placeables[i].IsSelected()) {
					if (!placeables[i + 1]) {
						return;
					}
					
					placeables[i].Deselect();
					GetEditor().CreateInHand(placeables[i + 1].GetPlaceableItem());
					placeables[i + 1].Select();
					return;
				}
			}
		}
		
		if (!ShouldProcessQuickInput(input)) return;
		EditorLog.Trace("Editor::OnEditorMoveObjectBackward");
		
		float value = 0.1;
		if (GetGame().GetInput().LocalValue("EditorCameraTurbo")) {
			value = 0.001;
		}
		
		QuickTransformObjects(Vector(0, 0, -value));
	}
	
	private void OnEditorMoveObjectLeft(UAInput input)
	{
		if (!ShouldProcessQuickInput(input)) return;
		EditorLog.Trace("Editor::OnEditorMoveObjectLeft");
		
		float value = 0.1;
		if (GetGame().GetInput().LocalValue("EditorCameraTurbo")) {
			value = 0.001;
		}
		
		QuickTransformObjects(Vector(-value, 0, 0));
	}	
	
	private void OnEditorMoveObjectRight(UAInput input)
	{
		if (!ShouldProcessQuickInput(input)) return;
		EditorLog.Trace("Editor::OnEditorMoveObjectRight");
		
		float value = 0.1;
		if (GetGame().GetInput().LocalValue("EditorCameraTurbo")) {
			value = 0.001;
		}
		
		QuickTransformObjects(Vector(value, 0, 0));
	}
	
	private void OnEditorMoveObjectUp(UAInput input)
	{
		if (!ShouldProcessQuickInput(input)) return;
		EditorLog.Trace("Editor::OnEditorMoveObjectUp");
		
		float value = 0.1;
		if (GetGame().GetInput().LocalValue("EditorCameraTurbo")) {
			value = 0.001;
		}
		
		QuickTransformObjects(Vector(0, value, 0));
	}	
	
	private void OnEditorMoveObjectDown(UAInput input)
	{
		if (!ShouldProcessQuickInput(input)) return;
		EditorLog.Trace("Editor::OnEditorMoveObjectDown");
		
		float value = 0.1;
		if (GetGame().GetInput().LocalValue("EditorCameraTurbo")) {
			value = 0.001;
		}
		
		QuickTransformObjects(Vector(0, -value, 0));
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