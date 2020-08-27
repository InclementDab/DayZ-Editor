

class EditorHud: Hud
{	
	
	protected Widget m_LayoutRoot;
	
	// Misc get ridda this shit too
	protected Widget 			m_EditorMapContainer;
	protected MapWidget 		m_EditorMapWidget;
	protected ref EditorMap 	m_EditorMap;
	protected EditBoxWidget 	m_LeftbarSearchBar;	
	
	protected ref EditorHudController 	m_EditorHudController;
	ref EditorHudController GetController() { 
		return m_EditorHudController;
	}
	
	protected Controller GetActiveController() {
		if (IsModalActive()) {
			return m_CurrentModal.GetController();
		} else {
			return m_EditorHudController;
		}
	}
	
	// literally track down everything that uses these and DELETE THEM its SHIT CODE TYLER DO IT PUSSY
	EditorMap GetMap() 			{ 
		return m_EditorMap; 
	}
	MapWidget GetMapWidget() 	{ 
		return m_EditorMapWidget; 
	}
	
	Widget GetRoot() { 
		return m_LayoutRoot; 
	}
	
	bool IsMapVisible() { 
		return m_EditorMapContainer.IsVisible(); 
	}
	
	override void Show(bool show) 
	{
		EditorLog.Trace("EditorHud::Show");
		m_LayoutRoot.Show(show);
	}
	
	bool IsVisible() {
		return m_LayoutRoot.IsVisible();
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
	
	void EditorHud() {
		EditorLog.Info("EditorHud");
	}
	
	void ~EditorHud() {
		EditorLog.Info("~EditorHud");
	}
	
	override void Init(Widget hud_panel_widget) 
	{
		EditorLog.Trace("EditorHud::Init");
		m_LayoutRoot 			= GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/layouts/EditorUI.layout", hud_panel_widget);		
		m_LayoutRoot.GetScript(m_EditorHudController);
		
		// Showcase UI
		//GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/layouts/DataBindingShowcase.layout", m_LayoutRoot);
		
		// Misc get ridda this shit too
		m_EditorMapContainer	= m_LayoutRoot.FindAnyWidget("MapContainer");
		m_EditorMapWidget		= MapWidget.Cast(m_LayoutRoot.FindAnyWidget("Map"));
		m_LeftbarSearchBar		= EditBoxWidget.Cast(m_LayoutRoot.FindAnyWidget("LeftbarSearchBar"));
		// why aint we usin scriptclass?
		m_EditorMapWidget.GetScript(m_EditorMap);
		m_EditorMapWidget.SetMapPos(GetGame().GetCurrentCameraPosition());
		
		EditorLog.Trace("EditorHud::Init::Finished");
	}
	
	override void Update(float timeslice)
	{
		
		typename mouse_state_type = BetterMouseState;
		
		
		for (int i = 0; i < 3; i++) {
			int mouse_state;
			mouse_state_type.GetVariableValue(null, i, mouse_state);
			if (GetMouseState(i) & MB_PRESSED_MASK) {
				if ((m_ActiveMouseStates & mouse_state) == 0) {
					m_ActiveMouseStates |= mouse_state;
					OnMouseDown(i);
					
				}
			} else { 
				if ((m_ActiveMouseStates & mouse_state) == mouse_state) {
					m_ActiveMouseStates &= ~mouse_state;
					OnMouseUp(i);
					
				}
			}
		}
		
		if (GetMouseState(MouseState.WHEEL) != 0) {
			OnMouseWheel(GetMouseState(MouseState.WHEEL));			
		}
		
		if (GetMouseState(MouseState.X) != 0 || GetMouseState(MouseState.Y) != 0) {
			int x, y;
			GetMousePos(x, y);
			MouseUpdatePosition(x, y);
		}
	}
	

	

	
	// Control Events
	private const int DOUBLE_CLICK_THRESHOLD = 250;
	
	private int m_ActiveMouseStates;
	private int m_DoubleClickButton;
	private void ResetDoubleClick()	{
		Sleep(DOUBLE_CLICK_THRESHOLD);
		m_DoubleClickButton = -1;
	}
	


	private ref Widget m_CurrentEnterWidget;
	private void MouseUpdatePosition(int x, int y)
	{
		//EditorLog.Trace("EditorHud::MouseUpdatePosition X%1 Y%2", x.ToString(), y.ToString());
		
		Widget current_widget = GetWidgetUnderCursor();
		if (current_widget) {
			if (m_CurrentEnterWidget != current_widget) {
				if (m_CurrentEnterWidget) {
					//OnMouseLeave(m_CurrentEnterWidget, current_widget, x, y);
				}
				m_CurrentEnterWidget = current_widget;
				OnMouseEnter(current_widget, x, y);
			} 
			
		} else {
			m_CurrentEnterWidget = null;
		}
		
	}
	
	void OnMouseDown(int button)
	{
		EditorLog.Trace("EditorHud::OnMouseDown %1", typename.EnumToString(MouseState, button));
		
		Widget target = GetWidgetUnderCursor();	
		if (target) {
			GetActiveController().OnMouseDown(target, button);
			if (target.IsControlClass())
				OnClick(target, button);
			
		}
		switch (button) {
			
			case MouseState.LEFT: {
#ifndef COMPONENT_SYSTEM
				if (GetEditor().IsPlacing()) {
					GetEditor().PlaceObject();
					return;
				}
				GetEditor().ClearSelection();
#endif
				break;
			}
			
			case MouseState.MIDDLE: {
				
				if (KeyState(KeyCode.KC_LCONTROL))
					EditorLog.Info(target.GetName());
				
				break;
			}
		}
	}
	
	void OnMouseUp(int button)
	{
		EditorLog.Trace("EditorHud::OnMouseUp %1", typename.EnumToString(MouseState, button));
		Widget w = GetWidgetUnderCursor();
		if (!w) return;
	
		GetActiveController().OnMouseUp(w, button);
	}
	
	void OnMouseWheel(int direction)
	{
		Widget w = GetWidgetUnderCursor();
		EditorLog.Trace("EditorHud::OnMouseWheel %1", w.GetName());
		if (!w) return;
		
		GetActiveController().OnMouseWheel(w, direction);	
	}
	
	void OnClick(Widget w, int button)
	{
		EditorLog.Trace("EditorHud::OnClick %1", w.GetName());
		GetActiveController().OnClick(w, button);
		
		if (m_DoubleClickButton == button) {
			OnDoubleClick(w, button);
			return;
		}
		
		m_DoubleClickButton = button;
		thread ResetDoubleClick();
	}	
		
	void OnDoubleClick(Widget w, int button)
	{
		EditorLog.Trace("EditorHud::OnDoubleClick: %1", w.GetName());
		GetActiveController().OnDoubleClick(w, button);
	}
	
	void OnKeyPress(int key)
	{
		EditorLog.Trace("EditorHud::OnKeyPress: %1", key.ToString());
		GetActiveController().OnKeyPress(key);
	}
	
	void OnMouseEnter(Widget w, int x, int y)
	{
		EditorLog.Trace("EditorHud::OnMouseEnter: %1", w.GetName());
		GetActiveController().OnMouseEnter(w, x, y);
	}
	
	void OnMouseLeave(Widget w, Widget enter_w, int x, int y)
	{
		EditorLog.Trace("EditorHud::OnMouseLeave %1 enter %2", w.GetName(), enter_w.GetName());
		GetActiveController().OnMouseLeave(w, enter_w, x, y);
	}
	/*
	void OnDrag(Widget target, int x, int y)
	{
		EditorLog.Trace("EditorHud::OnDrag: %1", target.GetName());
	}
	
	void OnDrop(Widget target, Widget drop_target, int x, int y)
	{
		EditorLog.Trace("EditorHud::OnDrop: %1 drop_target: %2", target.GetName(), drop_target.GetName());
	}
	
	void OnDragging(Widget target, int x, int y)
	{
		EditorLog.Trace("EditorHud::OnDragging: %1 X:%2 Y:%3", target.GetName(), x.ToString(), y.ToString());
	}
	
	void OnDropReceived(Widget target, Widget received_target, int x, int y)
	{
		EditorLog.Trace("EditorHud::OnDropReceived: %1 received_target: %2 X:%3 Y:%4", target.GetName(), received_target.GetName(), x.ToString(), y.ToString());
	}*/
	
	
	
	override void SetPermanentCrossHair(bool show); // todo
	
	// Modal Window Control
	private ref EditorDialog m_CurrentModal = null;
	void ModalSet(EditorDialog w)
	{
		EditorLog.Trace("EditorHud::ModalSet");
		m_CurrentModal = w;
		//SetModal(m_CurrentModal.GetRoot());
		ShowCursor();
	}
	
	void ModalClose()
	{
		EditorLog.Trace("EditorHud::ModalClose");
		m_CurrentModal.GetRoot().Unlink();
		m_CurrentModal = null;
		ShowCursor();
	}

	bool IsModalActive() {
		return (m_CurrentModal != null);
	}
	
	bool IsModalCommand(Widget w) {
		return (m_CurrentModal.GetRoot().FindAnyWidget(w.GetName()) != null);
	}
	
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




