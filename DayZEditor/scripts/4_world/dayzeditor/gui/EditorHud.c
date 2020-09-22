

/*
class EditorHud: MVCLayout
{
	
	protected Widget NotificationFrame;
	protected Widget NotificationPanel;
	protected TextWidget NotificationText;
	
	Widget MapContainer;
	MapWidget Map;
			
	void EditorHud(Widget parent = null)
	{	
		EditorLog.Trace("EditorHud");
	}
	
	void Update(float timeslice);
	
	void Show(bool show) 
	{
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
	
	void ShowCursor() {
		GetGame().GetUIManager().ShowCursor(true);
	}
	
	void HideCursor() {
		GetGame().GetUIManager().ShowCursor(false);
	}
	
	void ShowMap(bool show)	{
		MapContainer.Show(show);
	}
	
	bool IsMapVisible() { 
		return MapContainer.IsVisible(); 
	}
	
	
	void CreateNotification(string text, int color = -4301218, float duration = 4)
	{
		EditorLog.Trace("EditorHud::CreateNotification");
		
		NotificationPanel.SetColor(color);
		NotificationText.SetText(text);
		NotificationFrame.Show(true);
		
		thread _CreateNotification(text);
	}
	
	private void _CreateNotification(string text)
	{
		
		Sleep(1000);
		NotificationFrame.SetPos(0, 6);
	}
	
	override string GetLayoutFile() {
		return "DayZEditor/gui/layouts/EditorUI.layout";
	}
	
	override typename GetControllerType() {
		return EditorHudController;
	}
	
	EditorHudController GetEditorHudController() { 
		return EditorHudController.Cast(GetController());
	}
}*/


class EditorHud: Hud
{	
	protected Widget 			m_LayoutRoot;
	protected Widget			ToolbarFrame;

	protected ref EditorHudController 	m_EditorHudController;
	ref EditorHudController GetEditorHudController() { 
		return m_EditorHudController;
	}
		
	void EditorHud() {
		EditorLog.Info("EditorHud");
	}
	
	void ~EditorHud() {
		EditorLog.Info("~EditorHud");
		delete m_EditorHudController;
		m_LayoutRoot.Unlink();
	}
	
	
	Widget GetLayoutRoot() { 
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
	
	void ShowCursor() {
		GetGame().GetUIManager().ShowCursor(true);
	}
	
	void HideCursor() {
		GetGame().GetUIManager().ShowCursor(false);
	}
	
	void ShowMap(bool show)	{
		m_EditorHudController.MapContainer.Show(show);
	}
	
	bool IsMapVisible() { 
		return m_EditorHudController.MapContainer.IsVisible(); 
	}
	
	MapWidget GetMap() {
		return m_EditorHudController.Map;
	}
	
	override void Init(Widget hud_panel_widget) 
	{
		EditorLog.Trace("EditorHud::Init");
		m_LayoutRoot 			= GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/layouts/EditorUI.layout", hud_panel_widget);		
		m_LayoutRoot.GetScript(m_EditorHudController);

		
		EditorLog.Trace("EditorHud::Init::Finished");
	}
	

	
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




