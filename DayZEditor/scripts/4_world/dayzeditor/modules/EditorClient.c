
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
			
			// Random cam position smile :)
			float x = Math.RandomInt(3500, 8500);
			float z = Math.RandomInt(3500, 8500);
			float y = GetGame().SurfaceY(x, z);
			Editor.Create(CreateDefaultCharacter(Vector(x, y, z)));
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
	
	private bool m_Active;
	private void OnEditorToggleActive(UAInput input)
	{
		if (!ShouldProcessInput(input)) return;
		EditorLog.Trace("Editor::OnEditorToggleActive");
		m_Active = !m_Active;
		GetEditor().SetActive(m_Active);
		
	}	
	
	private void OnEditorToggleCursor(UAInput input)
	{
		if (!ShouldProcessInput(input)) return;
		EditorLog.Trace("Editor::OnEditorToggleCursor");
		
		// Dont want to toggle cursor on map
		if (!GetEditor().IsActive() || GetEditor().GetEditorHud().IsMapVisible()) 
			return;
		
		GetEditor().GetEditorHud().ToggleCursor();
	}	
	

	private void OnEditorToggleUI(UAInput input)
	{		
		if (!ShouldProcessInput(input)) return;
		EditorLog.Trace("Editor::OnEditorToggleUI");
		
		
		GetEditor().GetEditorHud().Show(!GetEditor().GetEditorHud().IsVisible());
	}
	
	private void OnEditorToggleMap(UAInput input)
	{
		if (!ShouldProcessInput(input)) return;
		EditorLog.Trace("Editor::OnEditorToggleMap");
		
		GetEditor().GetEditorHud().ShowMap(!GetEditor().GetEditorHud().IsMapVisible());
		GetEditor().GetEditorHud().ShowCursor();
		
		// Disabling the map until we use MVCLayout with it
		//EditorEvents.MapToggled(this, GetEditor().GetEditorHud().GetMap(), GetEditor().GetEditorHud().IsMapVisible());
	}	
	
	private void OnEditorDeleteObject(UAInput input)
	{
		if (!ShouldProcessInput(input)) return;
		EditorLog.Trace("Editor::OnEditorDeleteObject");
		GetEditor().DeleteObjects(GetEditor().GetSelectedObjects());
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

