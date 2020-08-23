

class EditorHud: Hud
{	
	
	protected Widget m_LayoutRoot;

	Widget GetRoot() { return m_LayoutRoot; }
	
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
		m_LayoutRoot 			= GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/layouts/EditorUI.layout", hud_panel_widget);		

		GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/layouts/DataBindingShowcase.layout", m_LayoutRoot);
		
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
		
	}
	
	override void Show(bool show) 
	{
		EditorLog.Trace("EditorHud::Show");
		m_LayoutRoot.Show(show);
	}
	
	bool IsVisible()
	{
		return m_LayoutRoot.IsVisible();
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
	
	void ShowMap(bool show)
	{
		m_EditorMapContainer.Show(show);
	}
	
	bool IsMapVisible() { return m_EditorMapContainer.IsVisible(); }
	
	
	// literally track down everything that uses these and DELETE THEM its SHIT CODE TYLER DO IT PUSSY
	EditorMap GetMap() 			{ return m_EditorMap; }
	MapWidget GetMapWidget() 	{ return m_EditorMapWidget; }
	
	
	
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


class EditorHudHandler: ScriptedWidgetEventHandler
{
	protected Widget m_LayoutRoot;	
	protected Widget m_LeftbarFrame;
	protected Widget m_RightbarFrame;
		
	void OnWidgetScriptInit(Widget w)
	{
		EditorLog.Trace("EditorHudHandler::OnWidgetScriptInit");
		m_LayoutRoot = w;
		m_LayoutRoot.SetHandler(this);
				
		m_LeftbarFrame			= m_LayoutRoot.FindAnyWidget("LeftbarFrame");
		m_RightbarFrame			= m_LayoutRoot.FindAnyWidget("RightbarFrame");
		
		
		// Set default
		//m_Controller.UpdatePlaceableItems(PlaceableObjectCategory.BUILDING);
		

	}
	

	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		EditorLog.Trace("EditorHudHandler::OnMouseButtonDown");
		if (button == 0) {
			
#ifndef COMPONENT_SYSTEM
			
			if (GetEditor().IsPlacing()) {
				GetEditor().PlaceObject();
				return true;
			}
			
			GetEditor().ClearSelection();
#endif
		}
		
		return super.OnMouseButtonDown(w, x, y, button);
	}

/*
	override bool OnClick(Widget w, int x, int y, int button)
	{
		EditorLog.Trace("EditorHudHandler::OnClick " + w.GetName());
		if (button == 0) {
			
			
			switch (w.GetName()) {
				
				case "LeftbarHide": {
					ShowLeftBar(m_Controller.LeftbarHide);					
					break;
				}
				
				case "RightbarHide": {
					ShowRightBar(m_Controller.RightbarHide);
					break;
				}
				
				case "BuildingSelect": {
					m_Controller.UpdatePlaceableItems(PlaceableObjectCategory.BUILDING);
					break;
				}
				
				case "VehicleSelect": {
					m_Controller.UpdatePlaceableItems(PlaceableObjectCategory.VEHICLE);
					break;
				}
				
				case "EntitySelect": {
					m_Controller.UpdatePlaceableItems(PlaceableObjectCategory.ENTITY);
					break;
				}
				
				case "HumanSelect": {
					m_Controller.UpdatePlaceableItems(PlaceableObjectCategory.HUMAN);
					break;
				}
				
				
			}
		}
		
		
		return super.OnClick(w, x, y, button);
	}
*/
	
	void ShowLeftBar(bool state)
	{
		m_LeftbarFrame.SetPos(-300 * state, 32);
	}
	
	void ShowRightBar(bool state)
	{
		m_RightbarFrame.SetPos(-300 * state, 32);
	}
}
