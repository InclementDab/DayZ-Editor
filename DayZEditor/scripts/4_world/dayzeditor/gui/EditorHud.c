

class EditorHud: Hud
{	
	protected Widget 			m_LayoutRoot;
	
	// Misc get ridda this shit too
	protected Widget 			m_EditorMapContainer;
	protected MapWidget 		m_EditorMapWidget;
	protected ref EditorMap 	m_EditorMap;
	protected EditBoxWidget 	m_LeftbarSearchBar;	
		
	protected ref EditorHudController 	m_EditorHudController;
	ref EditorHudController GetController() { 
		return m_EditorHudController;
	}
	
	void EditorHud() {
		EditorLog.Info("EditorHud");
	}
	
	void ~EditorHud() {
		EditorLog.Info("~EditorHud");
		delete m_EditorHudController;
		delete m_EditorMap;
		m_LayoutRoot.Unlink();
	}
	
	// literally track down everything that uses these and DELETE THEM its SHIT CODE TYLER DO IT PUSSY
	EditorMap GetMap() 			{ 
		return m_EditorMap; 
	}
	MapWidget GetMapWidget() 	{ 
		return m_EditorMapWidget; 
	}
	
	bool IsMapOpen() 			{ 
		return m_EditorMapContainer.IsVisible(); 
	}
	
	Widget GetRoot() { 
		return m_LayoutRoot; 
	}
	
	override void Show(bool show) {
		EditorLog.Trace("EditorHud::Show");
		m_LayoutRoot.Show(show);
		SetCursor(show);
		
		EditorObjectSet placed_objects = GetEditor().GetPlacedObjects();
		foreach (EditorObject editor_object: placed_objects) {
			editor_object.Show(show);
		}
	}
	
	bool IsVisible() {
		return m_LayoutRoot.IsVisible();
	}
	
	void SetCursor(bool state) {
		GetGame().GetUIManager().ShowCursor(state);
	}
	
	void ToggleCursor() {
		GetGame().GetUIManager().ShowCursor(!GetGame().GetUIManager().IsCursorVisible());
	}
	
	override void ShowCursor() {
		GetGame().GetUIManager().ShowCursor(true);
	}
	
	override void HideCursor() {
		GetGame().GetUIManager().ShowCursor(false);
	}
	
	void ShowMap(bool show)	{
		m_EditorMapContainer.Show(show);
	}

	
	override void Init(Widget hud_panel_widget) 
	{
		EditorLog.Trace("EditorHud::Init");
		m_LayoutRoot 			= GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/layouts/EditorUI.layout", hud_panel_widget);		
		m_LayoutRoot.GetScript(m_EditorHudController);
		
		// Misc get ridda this shit too
		m_EditorMapContainer	= m_LayoutRoot.FindAnyWidget("MapContainer");
		m_EditorMapWidget		= MapWidget.Cast(m_LayoutRoot.FindAnyWidget("Map"));
		m_LeftbarSearchBar		= EditBoxWidget.Cast(m_LayoutRoot.FindAnyWidget("LeftbarSearchBar"));
		// why aint we usin scriptclass?
		m_EditorMapWidget.GetScript(m_EditorMap);
		m_EditorMapWidget.SetMapPos(GetGame().GetCurrentCameraPosition());
		
		EditorLog.Trace("EditorHud::Init::Finished");
	}
	
	override void Update(float timeslice) {}	
	
	override void SetPermanentCrossHair(bool show); // todo

			
	// Current "button" on UI
	private ButtonWidget m_CurrentButton;
	void SetCurrentButton(ButtonWidget button)
	{
		EditorLog.Trace("EditorHud::SetCurrentButton");
		m_CurrentButton = button;
	}
	
	void ExecuteCurrentButton()
	{
		EditorLog.Trace("EditorHud::ExecuteCurrentButton");
		
		if (!m_CurrentButton) return;
		EditorLog.Debug("Attempting to Execute RelayCommand from " + m_CurrentButton.GetName());
		
		ViewBinding view_binding;
		m_CurrentButton.GetScript(view_binding);
		if (!view_binding) {
			EditorLog.Info("View Binding not found!");
			return;
		}
		
		view_binding.InvokeCommand(new ButtonCommandArgs(m_CurrentButton, 0, true));
	}
	
	
}




