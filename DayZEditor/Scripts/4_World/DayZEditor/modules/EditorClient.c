
enum EditorClientModuleRPC
{
	INVALID = 36114,
	COUNT
};


enum BetterMouseState {
	LEFT = 1,
	RIGHT = 2,
	MIDDLE = 4
};

class EditorClientModule: JMModuleBase
{
	
	// Public Members
	void EditorClientModule() {
		EditorLog.Info("EditorClientModule");
	}
	
	void ~EditorClientModule() {
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
	}
		
	override void OnUpdate(float timeslice)
	{
		if (GetEditor()) {
			GetEditor().Update(timeslice);
		}
				
		if (GetEditor() && GetEditor().GetCamera() && !IsMissionOffline()) {
			ScriptRPC update_rpc = new ScriptRPC();
			update_rpc.Write(GetEditor().GetCamera().GetPosition());
			update_rpc.Write(GetEditor().GetCamera().GetOrientation());
			//update_rpc.Send(null, EditorServerModuleRPC.EDITOR_CLIENT_UPDATE, true);
		}
	}
	
	override bool IsServer() 
		return false;

	
		
	override void OnMissionStart()
	{
		EditorLog.Trace("Editor::OnMissionStart");		
		
		if (IsMissionOffline()) {
			EditorLog.Info("Loading Offline Editor...");
			Editor.Create(CreateDefaultCharacter(Editor.GetRandomTeleportPosition()));
		} else {
			
			EditorLog.Info("Loading Online Editor...");
			Editor.Create(GetGame().GetPlayer());
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
	}
	
	
	// Inputs
	private bool ShouldProcessInput(UAInput input)
	{
		// Check if LocalPress, Check if LControl is pressed, Check if game is focused
		return (input.LocalPress() && !KeyState(KeyCode.KC_LCONTROL) && GetGame().GetInput().HasGameFocus());
	}
	
	private bool m_IsActive;
	private void OnEditorToggleActive(UAInput input)
	{
		if (!ShouldProcessInput(input)) return;
		EditorLog.Trace("Editor::OnEditorToggleActive");
		
		string ban_reason;
		if (GetEditor().IsBannedClient(ban_reason)) {
			EditorLog.Warning("Banned Client Detected! Exiting...");
			GetGame().GetUIManager().ShowDialog("Banned from DayZ Editor", string.Format("You have been banned from using the DayZ Editor.\n%1\n If you believe this was in error, please contact InclementDab \# 0001 on Discord", ban_reason), 76, DBT_OK, DBB_NONE, DMT_INFO, GetGame().GetUIManager().GetMenu());
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
		if (!GetEditor().IsActive() || GetEditor().GetEditorHud().EditorMapWidget.IsVisible()) 
			return;
		
		GetEditor().GetEditorHud().ToggleCursor();
	}	
	

	private void OnEditorToggleUI(UAInput input)
	{		
		if (!ShouldProcessInput(input)) return;
		EditorLog.Trace("Editor::OnEditorToggleUI");
		
		string ban_reason;
		if (GetEditor().IsBannedClient(ban_reason)) {
			EditorLog.Warning("Banned Client Detected! Exiting...");
			GetGame().GetUIManager().ShowDialog("Banned from DayZ Editor", string.Format("You have been banned from using the DayZ Editor.\n%1\n If you believe this was in error, please contact InclementDab \# 0001 on Discord", ban_reason), 76, DBT_OK, DBB_NONE, DMT_INFO, GetGame().GetUIManager().GetMenu());
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
	
	
	// RPC stuff
	override int GetRPCMin() 
		return EditorClientModuleRPC.INVALID;
	

	override int GetRPCMax()
		return EditorClientModuleRPC.COUNT;
	
	
	override void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ref ParamsReadContext ctx)
	{
		switch (rpc_type) {
			
		
		}
	}
	
	
}

