

class EditorHud: Hud
{	
	
	protected Widget m_LayoutRoot;
	
	// Misc get ridda this shit too
	protected Widget 			m_EditorMapContainer;
	protected MapWidget 		m_EditorMapWidget;
	protected ref EditorMap 	m_EditorMap;
	protected EditBoxWidget 	m_LeftbarSearchBar;	
	
	protected ref ViewBindingHashMap m_ViewBindingHashMap = new ViewBindingHashMap();
	
	protected ref EditorHudController 	m_EditorHudController;
	ref EditorHudController GetController() { 
		return m_EditorHudController;
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
	
	void OnViewBindingCreated(ViewBinding view_binding)
	{
		EditorLog.Info("EditorHud::OnViewBindingCreated");
		m_ViewBindingHashMap.InsertView(view_binding);
	}
	
	void EditorHud() {
		EditorLog.Info("EditorHud");
		OnViewBindingCreated.Insert(OnViewBindingCreated);
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
		int x, y;
		GetMousePos(x, y);
		typename mouse_state_type = BetterMouseState;
		Widget target = GetWidgetUnderCursor();	
		
		for (int i = 0; i < 3; i++) {
			int mouse_state;
			mouse_state_type.GetVariableValue(null, i, mouse_state);
			if (GetMouseState(i) & MB_PRESSED_MASK) {
				if ((m_ActiveMouseStates & mouse_state) == 0) {
					m_ActiveMouseStates |= mouse_state;
					GetEditor().OnMouseDown(target, i, x, y);
					if (target) {
						OnMouseDown(target, i, x, y);
					}
				}
			} else {
				if ((m_ActiveMouseStates & mouse_state) == mouse_state) {
					m_ActiveMouseStates &= ~mouse_state;
					GetEditor().OnMouseUp(target, i, x, y);
					if (target) {
						OnMouseUp(target, i, x, y);
					}					
				}
			}
		}
		
		if (GetMouseState(MouseState.WHEEL) != 0) {
			OnMouseWheel(target, GetMouseState(MouseState.WHEEL), x, y);			
		}
		
			
		
		if (GetMouseState(MouseState.X) != 0 || GetMouseState(MouseState.Y) != 0) {
			MouseUpdatePosition(x, y);
		}
	}
	
	
	// Control Events
	private const int DOUBLE_CLICK_THRESHOLD = 250;
	private const int RESET_DRAG_THRESHOLD = 30;
	
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
	
	void OnMouseDown(Widget target, int button, int x, int y)
	{
		EditorLog.Trace("EditorHud::OnMouseDown: %1", target.GetName());
		
		ViewBindingSet view_set = m_ViewBindingHashMap.Get(target);
		if (!view_set) return;
		foreach (ViewBinding view: view_set) {
			view.OnMouseDown(target, button, x, y);
		}
		
		if (target.IsControlClass()) {
			OnClick(target, button, x, y);
			return;
		}
	}
	
	void OnMouseUp(Widget target, int button, int x, int y)
	{
		EditorLog.Trace("EditorHud::OnMouseUp %1", target.GetName());
		ViewBindingSet view_set = m_ViewBindingHashMap.Get(target);
		if (!view_set) return;
		foreach (ViewBinding view: view_set) {
			view.OnMouseUp(target, button, x, y);
		}	
	}
	
	void OnMouseWheel(Widget target, int direction, int x, int y)
	{
		EditorLog.Trace("EditorHud::OnMouseWheel: %1", target.GetName());
		ViewBindingSet view_set = m_ViewBindingHashMap.Get(target);
		if (!view_set) return;
		foreach (ViewBinding view: view_set) {
			view.OnViewMouseWheel(target, direction, x, y);
		}	
	}
	
	
	void OnClick(Widget target, int button, int x, int y)
	{
		EditorLog.Trace("EditorHud::OnClick %1", target.GetName());
		ViewBindingSet view_set = m_ViewBindingHashMap.Get(target);
		if (!view_set) return;
		foreach (ViewBinding view: view_set) {
			view.OnViewClick(target, button, x, y);
		}	
		
		if (m_DoubleClickButton == button) {
			OnDoubleClick(target, button, x, y);
			return;
		}
		
		m_DoubleClickButton = button;
		thread ResetDoubleClick();
	}	
		
	void OnDoubleClick(Widget target, int button, int x, int y)
	{
		EditorLog.Trace("EditorHud::OnDoubleClick: %1", target.GetName());
		ViewBindingSet view_set = m_ViewBindingHashMap.Get(target);
		if (!view_set) return;
		foreach (ViewBinding view: view_set) {
			view.OnViewDoubleClick(target, button, x, y);
		}	
	}
	
	void OnKeyPress(int key)
	{
		//EditorLog.Trace("EditorHud::OnKeyPress: %1", key.ToString());
		//foreach (ViewBinding view: m_ViewBindingHashMap.Get(target.GetName())) {
		//	view.OnKeyPress(key);
		//}	
	}
	
	void OnMouseEnter(Widget target, int x, int y)
	{
		EditorLog.Trace("EditorHud::OnMouseEnter: %1", target.GetName());
		ViewBindingSet view_set = m_ViewBindingHashMap.Get(target);
		if (!view_set) return;
		
		foreach (ViewBinding view: view_set)
			view.OnViewMouseEnter(target, x, y);
		
	}
	
	void OnMouseLeave(Widget target, Widget enter_w, int x, int y)
	{
		EditorLog.Trace("EditorHud::OnMouseLeave: %1 Enter: %2", target.GetName(), enter_w.GetName());
		ViewBindingSet view_set = m_ViewBindingHashMap.Get(target);
		if (!view_set) return;
		
		foreach (ViewBinding view: view_set)
			view.OnViewMouseLeave(target, enter_w, x, y);
		
	}

	
	
	override void SetPermanentCrossHair(bool show); // todo
	
	// Modal Window Control
	private ref EditorDialog m_CurrentModal = null;
	void ModalSet(EditorDialog w)
	{
		EditorLog.Trace("EditorHud::ModalSet: ", w.GetTitle());
		m_CurrentModal = w;
		GetEditor().GetCamera().SetMoveEnabled(false);
		GetEditor().GetCamera().SetLookEnabled(false);
		ShowCursor();
	}
	
	void ModalClose()
	{
		EditorLog.Trace("EditorHud::ModalClose");
		m_CurrentModal.GetRoot().Unlink();
		m_CurrentModal = null;
		GetEditor().GetCamera().SetMoveEnabled(true);
		GetEditor().GetCamera().SetLookEnabled(true);
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




