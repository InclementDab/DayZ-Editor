

class EditorHud: Hud
{	
	
	protected Widget m_LayoutRoot;

	Widget GetRoot() { return m_LayoutRoot; }
	
	// Misc get ridda this shit too
	protected Widget 			m_EditorMapContainer;
	protected MapWidget 		m_EditorMapWidget;
	protected ref EditorMap 	m_EditorMap;
	protected EditBoxWidget 	m_LeftbarSearchBar;	
	
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
	
	bool IsMapVisible() { 
		return m_EditorMapContainer.IsVisible(); 
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
		// Handles OnMouseDown and OnMouseUp events
		for (int i = 0; i < 3; i++) {
			int mouse_state;
			mouse_state_type.GetVariableValue(null, i, mouse_state);
			if (GetMouseState(i) & MB_PRESSED_MASK) {
				if ((m_ActiveMouseStates & mouse_state) == 0) {
					m_ActiveMouseStates |= mouse_state;
					OnMouseDown(mouse_state);
				}
			} else { 
				if ((m_ActiveMouseStates & mouse_state) == mouse_state) {
					m_ActiveMouseStates &= ~mouse_state;
					OnMouseUp(mouse_state);
				}
			}
		}
		
		if (GetMouseState(MouseState.X) != 0 || GetMouseState(MouseState.Y) != 0) {
			int x, y;
			GetMousePos(x, y);
			MouseUpdatePosition(x, y);
		}
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
	

	
	// Control Events
	private const int DOUBLE_CLICK_THRESHOLD = 250;
	
	private int m_ActiveMouseStates;
	private int m_DoubleClickButton;
	private void ResetDoubleClick()	{
		Sleep(DOUBLE_CLICK_THRESHOLD);
		m_DoubleClickButton = -1;
	}
	
	private Widget m_CurrentWidget;
	private void MouseUpdatePosition(int x, int y)
	{
		//EditorLog.Trace("EditorHud::MouseUpdatePosition X%1 Y%2", x.ToString(), y.ToString());
		
		Widget w = GetWidgetUnderCursor();		
		if (m_CurrentWidget != w) {
			OnMouseLeave(m_CurrentWidget, w, x, y);
			m_CurrentWidget = w;
			OnMouseEnter(m_CurrentWidget, x, y);
		}
	}
	
	void OnMouseDown(int button)
	{
		//EditorLog.Trace("EditorHud::OnMouseDown %1", typename.EnumToString(BetterMouseState, button));
		
		Widget w = GetWidgetUnderCursor();
		// Checks if Widget is Control Class
		// Also checks if Modal is active, if it is, it will only fire click events
		// within that modal window
		if (w.IsControlClass() && !IsModalActive() || (IsModalActive() && IsModalCommand(w))) {
			OnClick(w, button);
			return;
		}
		
		switch (button) {
			
			case BetterMouseState.LEFT: {
								
				break;
			}
			
			case BetterMouseState.MIDDLE: {
				
				if (KeyState(KeyCode.KC_LCONTROL))
					EditorLog.Info(GetWidgetUnderCursor().GetName());
				
				break;
			}
		}
	}
	
	void OnMouseUp(int button)
	{
		//EditorLog.Trace("EditorHud::OnMouseUp %1", typename.EnumToString(BetterMouseState, button));
	}
	
	void OnClick(Widget w, int button)
	{
		EditorLog.Trace("EditorHud::OnClick %1", w.GetName());
		if (m_DoubleClickButton == button) {
			OnDoubleClick(w, button);
			return;
		}
		

		
		m_DoubleClickButton = button;
		thread ResetDoubleClick();
	}	
		
	void OnDoubleClick(Widget w, int button)
	{
		EditorLog.Trace("EditorHud::OnDoubleClick %1", w.GetName());
	}
	
	void OnKeyPress(int key)
	{	
		EditorLog.Trace("EditorHud::OnKeyPress %1", key.ToString());
	}
	
	void OnMouseEnter(Widget w, int x, int y)
	{
		EditorLog.Trace("EditorHud::OnMouseEnter %1", w.GetName());
	}
	
	void OnMouseLeave(Widget w, Widget enter_w, int x, int y)
	{
		EditorLog.Trace("EditorHud::OnMouseLeave %1 enter: %2", w.GetName(), enter_w.GetName());
	}
	
	
	
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


