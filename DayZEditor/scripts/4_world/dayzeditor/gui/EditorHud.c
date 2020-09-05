

class EditorHud: Hud
{	
	
	protected Widget m_LayoutRoot;
	
	// Misc get ridda this shit too
	protected Widget 			m_EditorMapContainer;
	protected MapWidget 		m_EditorMapWidget;
	protected ref EditorMap 	m_EditorMap;
	protected EditBoxWidget 	m_LeftbarSearchBar;	
	
		
	protected ref MVCEventHashMap m_MVCEventHashMap = new MVCEventHashMap();
	
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
	
	void OnMVCEventHandlerCreated(MVCEventHandler mvc_event)
	{
		EditorLog.Info("EditorHud::OnMVCEventHandlerCreated");
		m_MVCEventHashMap.InsertMVCEvent(mvc_event);
	}
	
	void EditorHud() {
		EditorLog.Info("EditorHud");
		OnMVCEventHandlerCreated.Insert(OnMVCEventHandlerCreated);
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
					OnMouseDown(target, i, x, y);
				}
			} else {
				if ((m_ActiveMouseStates & mouse_state) == mouse_state) {
					m_ActiveMouseStates &= ~mouse_state;
					OnMouseUp(target, i, x, y);				
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
	

	private ref Widget m_DragWidget;
	private void ResetDrag(Widget target, int x, int y) {
		Sleep(RESET_DRAG_THRESHOLD);
		if (!(GetMouseState(MouseState.LEFT) & MB_PRESSED_MASK)) return;
		
		EditorLog.Trace("EditorHud::ResetDrag");
		m_DragWidget = target;
		OnDrag(target, x, y);
		
		g_Game.GetUpdateQueue(CALL_CATEGORY_GUI).Insert(_DragUpdater);		
	}
	
	private void _DragUpdater()
	{
		int x, y;
		if (!(GetMouseState(MouseState.LEFT) & MB_PRESSED_MASK)) {
			g_Game.GetUpdateQueue(CALL_CATEGORY_GUI).Remove(_DragUpdater);
			
			GetMousePos(x, y);
			OnDrop(m_DragWidget, m_DragWidget, x, y);	
			return;
		} 
		
		
		GetMousePos(x, y);
		OnDragging(m_DragWidget, x, y);
	}
	

	
	

	private ref Widget m_CurrentEnterWidget;
	private void MouseUpdatePosition(int x, int y)
	{
		//EditorLog.Trace("EditorHud::MouseUpdatePosition X%1 Y%2", x.ToString(), y.ToString());
		
		Widget current_widget = GetWidgetUnderCursor();
		if (current_widget) {
			if (m_CurrentEnterWidget != current_widget) {
				OnMouseLeave(m_CurrentEnterWidget, x, y);
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
		
		if (target) {
			
			MVCEventHandler mvc_event = m_MVCEventHashMap.Get(target);
			if (mvc_event) {
				mvc_event.MVCOnMouseDown(target, button, x, y);	
			}
					
			if (button == MouseState.LEFT && (target.GetFlags() & WidgetFlags.DRAGGABLE)) {
				thread ResetDrag(target, x, y);
			}
			
			if (target.IsControlClass()) {
				OnClick(target, button, x, y);
				return;
			}
		} 
		
			
		GetEditor().OnMouseDown(target, button, x, y);
			
	}
	
	void OnMouseUp(Widget target, int button, int x, int y)
	{
		EditorLog.Trace("EditorHud::OnMouseUp %1", target.GetName());
		
		if (target) {
			MVCEventHandler mvc_event = m_MVCEventHashMap.Get(target);
			if (mvc_event) {
				mvc_event.MVCOnMouseUp(target, button, x, y);
			}
		}
		
		GetEditor().OnMouseUp(target, button, x, y);			
	}
	
	void OnMouseWheel(Widget target, int direction, int x, int y)
	{
		EditorLog.Trace("EditorHud::OnMouseWheel: %1", target.GetName());
		MVCEventHandler mvc_event = m_MVCEventHashMap.Get(target);
		if (!mvc_event) return;

		mvc_event.MVCOnMouseWheel(target, direction, x, y);
	}
	
	
	void OnClick(Widget target, int button, int x, int y)
	{
		EditorLog.Trace("EditorHud::OnClick %1", target.GetName());
		MVCEventHandler mvc_event = m_MVCEventHashMap.Get(target);
		if (!mvc_event) return;
		
		mvc_event.MVCOnClick(target, button, x, y);
		
		
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
		MVCEventHandler mvc_event = m_MVCEventHashMap.Get(target);
		if (!mvc_event) return;
		mvc_event.MVCOnDoubleClick(target, button, x, y);
			
	}
	
	void OnKeyPress(int key)
	{
		//EditorLog.Trace("EditorHud::OnKeyPress: %1", key.ToString());
		/*MVCEventHandler mvc_event = m_MVCEventHashMap.Get(target);
		if (!mvc_event) return;

		mvc_event.OnKeyPress(key);
			*/
	}
	
	void OnMouseEnter(Widget target, int x, int y)
	{
		EditorLog.Trace("EditorHud::OnMouseEnter: %1", target.GetName());
		MVCEventHandler mvc_event = m_MVCEventHashMap.Get(target);
		if (!mvc_event) return;
		
		mvc_event.OnMouseEnter(target, x, y);
		
	}
	
	void OnMouseLeave(Widget target, int x, int y)
	{
		EditorLog.Trace("EditorHud::OnMouseLeave: %1", target.GetName());
		MVCEventHandler mvc_event = m_MVCEventHashMap.Get(target);
		if (!mvc_event) return;
		mvc_event.MVCOnMouseLeave(target, x, y);
	}
	
	void OnDrag(Widget target, int x, int y)
	{
		EditorLog.Trace("EditorHud::OnDrag: %1", target.GetName());
		MVCEventHandler mvc_event = m_MVCEventHashMap.Get(target);
		if (!mvc_event) return;
		mvc_event.MVCOnDrag(target, x, y);
		
	}
	
	void OnDrop(Widget target, Widget drop_target, int x, int y)
	{
		EditorLog.Trace("EditorHud::OnDrop: %1 drop_target: %2", target.GetName(), drop_target.GetName());
		MVCEventHandler mvc_event = m_MVCEventHashMap.Get(target);
		if (!mvc_event) return;
	
		mvc_event.MVCOnDrop(target, drop_target, x, y);
		
	}
	
	void OnDragging(Widget target, int x, int y)
	{
		//EditorLog.Trace("EditorHud::OnDragging: %1 X:%2 Y:%3", target.GetName(), x.ToString(), y.ToString());
		MVCEventHandler mvc_event = m_MVCEventHashMap.Get(target);
		if (!mvc_event) return;

		mvc_event.MVCOnDragging(target, x, y);
		
	}
	
	void OnDropReceived(Widget target, Widget received_target, int x, int y)
	{
		EditorLog.Trace("EditorHud::OnDropReceived: %1 received_target: %2 X:%3 Y:%4", target.GetName(), received_target.GetName(), x.ToString(), y.ToString());
		MVCEventHandler mvc_event = m_MVCEventHashMap.Get(target);
		if (!mvc_event) return;
		mvc_event.MVCOnDropReceived(target, received_target, x, y);
		
	}
	
	
	
	override void SetPermanentCrossHair(bool show); // todo
	
	// Modal Window Control
	private ref EditorDialog m_CurrentModal = null;
	
	void SetModal(EditorDialog w) {
		m_CurrentModal = w;
	}
	
	EditorDialog GetModal() {
		return m_CurrentModal;
	}
	

	bool IsModalActive() {
		return (m_CurrentModal != null);
	}
	
	bool IsModalCommand(Widget w) {
		return (m_CurrentModal.GetLayoutRoot().FindAnyWidget(w.GetName()) != null);
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




