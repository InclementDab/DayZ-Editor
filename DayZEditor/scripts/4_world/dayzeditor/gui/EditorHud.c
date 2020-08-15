

class EditorHudHandler: ScriptedWidgetEventHandler
{
	protected Widget m_LayoutRoot;
	
	void OnWidgetScriptInit(Widget w)
	{
		EditorLog.Trace("EditorHudHandler::OnWidgetScriptInit");
		m_LayoutRoot = w;
		m_LayoutRoot.SetHandler(this);
	}
	
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		EditorLog.Trace("EditorHudHandler::OnMouseButtonDown");
		if (button == 0) {
			if (GetEditor().IsPlacing()) {
				GetEditor().PlaceObject();
				return true;
			}
			
		}
		
		return super.OnMouseButtonDown(w, x, y, button);
	}
	
}




class EditorHud: Hud
{	
	
	protected Widget m_LayoutRoot;
	
	
	// Misc get ridda this shit too
	protected Widget 			m_EditorMapContainer;
	protected MapWidget 		m_EditorMapWidget;
	protected ref EditorMap 	m_EditorMap;
	protected EditBoxWidget 	m_LeftbarSearchBar;	
	
	void EditorHud()
	{
		EditorLog.Info("EditorHud");
	}
	
	void ~EditorHud()
	{
		EditorLog.Info("~EditorHud");
	}
	
	override void Init(Widget hud_panel_widget) 
	{
		EditorLog.Trace("EditorHud::Init");
		m_LayoutRoot = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/layouts/EditorUI.layout", hud_panel_widget);
		
		
		// Misc get ridda this shit too
		m_EditorMapContainer	= m_LayoutRoot.FindAnyWidget("MapContainer");
		m_EditorMapWidget		= MapWidget.Cast(m_LayoutRoot.FindAnyWidget("Map"));
		m_LeftbarSearchBar		= EditBoxWidget.Cast(m_LayoutRoot.FindAnyWidget("LeftbarSearchBar"));
		// why aint we usin scriptclass?
		m_EditorMapWidget.GetScript(m_EditorMap);
		m_EditorMapWidget.SetMapPos(GetGame().GetCurrentCameraPosition());
	}
	
	override void Update(float timeslice)
	{
		Input input = GetGame().GetInput();
		
		
		if (input.LocalPress("UAFire")) {
			Print("Click");
		}
		
		
	}
	
	override void Show(bool show) 
	{
		EditorLog.Trace("EditorHud::Show");
		m_LayoutRoot.Show(show);
	}
	
	void ToggleCursor()
	{
		GetGame().GetUIManager().ShowCursor(!GetGame().GetUIManager().IsCursorVisible());
	}
	
	override void ShowCursor() 
	{
		GetGame().GetUIManager().ShowCursor(true);
	}
	
	override void HideCursor() 
	{
		GetGame().GetUIManager().ShowCursor(false);
	}
	
	
	// literally track down everything that uses these and DELETE THEM its SHIT CODE TYLER DO IT PUSSY
	EditorMap GetMap() 			{ return m_EditorMap; }
	MapWidget GetMapWidget() 	{ return m_EditorMapWidget; }
	bool IsMapOpen() 			{ return m_EditorMapContainer.IsVisible(); }
	
	
	override void SetPermanentCrossHair(bool show) 
	{
		// todo 
	}
	
	// Modal Window Control
	private ref EditorDialog m_CurrentModal;
	void ModalSet(EditorDialog w)
	{
		Print("ModalSet");
		m_CurrentModal = w;
		SetModal(m_CurrentModal.GetRoot());
		ShowCursor();
	}
	
	void ModalClose()
	{
		Print("ModalClose");
		m_CurrentModal.GetRoot().Unlink();
		m_CurrentModal = null;
		ShowCursor();
	}

	bool IsModalActive()
	{
		return GetGame().GetUIManager().IsModalVisible();
	}
}

