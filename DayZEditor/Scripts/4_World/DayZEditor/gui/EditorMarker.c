
class EditorMarker: ScriptView
{
	protected bool m_Show = true;
	void EditorMarker()
	{
		EditorLog.Trace("EditorMarker");
		m_LayoutRoot.SetAlpha(MARKER_ALPHA_ON_HIDE);
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
	}
	
	void ~EditorMarker()
	{
		EditorLog.Trace("~EditorMarker");
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
	}
	
	void Update();
	
	void SetPos(float x, float y) 
	{
		// Offset to center of marker
		x -= 10; y -= 10;
		m_LayoutRoot.SetPos(x, y);
		m_LayoutRoot.Update();
	}
	
	void GetPos(out float x, out float y)
	{
		m_LayoutRoot.GetPos(x, y);
	}
	
	void Show(bool show)
	{
		m_Show = show;
		if (m_LayoutRoot)
			m_LayoutRoot.Show(m_Show);
	}
	
	protected bool IsMouseInside(int c_x, int c_y)
	{
		float x, y, w, h;
		m_LayoutRoot.GetPos(x, y);
		m_LayoutRoot.GetSize(w, h);
		return (c_x < x + h && c_x > x - h) && (c_y < y + h && c_y > y - h);
	}
	
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/EditorMarker.layout";
	}
}


class EditorCameraMapMarker: EditorMarker
{
	protected Camera m_Camera;
	protected MapWidget m_EditorMap;
	
	void EditorCameraMapMarker(Camera camera)
	{	
		m_Camera = camera;
		m_EditorMap = GetEditor().GetEditorHud().EditorMapWidget;
	}
	
	override void Update()
	{
		if (!m_EditorMap || !m_EditorMap.IsVisible()) {
			Show(false);
			return;
		}
		
		Show(true);
				
		vector position = m_EditorMap.MapToScreen(m_Camera.GetPosition());
		SetPos(position[0], position[1]);
	}
	
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/EditorCameraMarker.layout";
	}
}


class EditorObjectMarker: EditorMarker
{	
	protected Editor m_Editor;
	
	protected EditorObject m_EditorObject;
	EditorObject GetEditorObject() { 
		return m_EditorObject; 
	}
	
	protected ref DragHandler m_DragHandler;
	DragHandler GetDragHandler() { 
		return m_DragHandler; 
	}
	
	void EditorObjectMarker(EditorObject editor_object)
	{
		EditorLog.Trace("EditorObjectMarker");
		m_EditorObject = editor_object;
		m_Editor = GetEditor();
		
		m_EditorObject.OnObjectSelected.Insert(EditorObjectSelected);
		m_EditorObject.OnObjectDeselected.Insert(EditorObjectDeselected);	
	}
	
	void ~EditorObjectMarker()
	{
		delete m_DragHandler;
	}
	
	void EditorObjectSelected(EditorObject data) 
	{		
		Select();
	}
	
	void EditorObjectDeselected(EditorObject data) 
	{
		Deselect();
	}
		
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{		
		// ignores the object if you are placing
		if (m_Editor.IsPlacing()) return false;
		
		
		switch (button) {
			
			case MouseState.LEFT: {
				
				// We want to Toggle selection if you are holding control
				if (KeyState(KeyCode.KC_LCONTROL)) {
					m_Editor.ToggleSelection(m_EditorObject);
					return true;
				} 
				
				// allows multiple objects to be dragged
				if (m_EditorObject.IsSelected()) {
					thread CheckDragBounds(x, y);
					return true;
				}
				
				if (!KeyState(KeyCode.KC_LSHIFT)) {
					m_Editor.ClearSelection();
				}
				
				m_Editor.SelectObject(m_EditorObject);
				
				thread CheckDragBounds(x, y);
				return false;
			}
			
			case MouseState.MIDDLE: {
				EditorCamera camera = GetEditor().GetCamera();
				vector pos = m_EditorObject.GetPosition();
				pos[1] = camera.GetPosition()[1];
				camera.SetPosition(pos);
				return true;
			}
		}
		
		return super.OnMouseButtonDown(w, x, y, button);
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		switch (button) {
			
			case MouseState.RIGHT: {
				
				if (!m_EditorObject.IsSelected() && !KeyState(KeyCode.KC_LSHIFT)) {
					m_Editor.ClearSelection();
				}
				
				m_Editor.SelectObject(m_EditorObject);
				
				if (EditorHud.CurrentMenu) {
					delete EditorHud.CurrentMenu;
				}
				
				EditorHud.CurrentMenu = new EditorPlacedContextMenu(x, y);
				
				return true;
			}
			
		}
		
		return super.OnClick(w, x, y, button);
	}
	
	void Select() 
	{
		m_LayoutRoot.SetAlpha(MARKER_ALPHA_ON_SHOW);
	}
	
	void Highlight()
	{
		m_LayoutRoot.SetAlpha(MARKER_ALPHA_ON_SHOW);
	}
	
	void Deselect() 
	{
		m_LayoutRoot.SetAlpha(MARKER_ALPHA_ON_HIDE);
	}
	
	bool IsSelected() 
	{
		return m_EditorObject.IsSelected();
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (!IsSelected()) {
			Highlight();
			if (m_EditorObject.GetListItem()) {
				m_EditorObject.GetListItem().Highlight();
			}
		}
		
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (!IsSelected()) {
			Deselect();
			if (m_EditorObject.GetListItem()) {
				m_EditorObject.GetListItem().Deselect();
			}
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
	
	private const int DRAG_THRESHOLD = 5;
	private void CheckDragBounds(int x, int y)
	{
		while (GetMouseState(MouseState.LEFT) & MB_PRESSED_MASK) {
			int c_x, c_y;
			GetMousePos(c_x, c_y);
			
			int dist_x = Math.AbsInt(x - c_x);
			int dist_y = Math.AbsInt(y - c_y);
			
			if (dist_x + dist_y > DRAG_THRESHOLD) {
				GetEditor().SelectObject(m_EditorObject);
				m_DragHandler.OnDragStart();
				return;
			}
			
			Sleep(10);
		}
	}
	
	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		m_Editor.CommandManager.ObjectPropertiesCommand.Execute(this, null);
		
		return true;
	}

}


class EditorObjectMapMarker: EditorObjectMarker
{		
	private MapWidget m_EditorMap;
	
	void EditorObjectMapMarker(EditorObject editor_object)
	{
		m_DragHandler = new MapDragHandler(m_EditorObject);
		m_EditorMap = m_Editor.GetEditorHud().EditorMapWidget;
	}
	
	override void Update()
	{
		if (!m_EditorMap || !m_EditorMap.IsVisible()) return;
		
		vector position = m_EditorMap.MapToScreen(m_EditorObject.GetPosition());
		SetPos(position[0], position[1]);
		super.Update();
	}
}

class EditorObjectWorldMarker: EditorObjectMarker
{
	protected MapWidget m_MapWidget;
	void EditorObjectWorldMarker(EditorObject editor_object)
	{
		m_MapWidget = m_Editor.GetEditorHud().EditorMapWidget;
		m_DragHandler = new ObjectDragHandler(m_EditorObject);
		EditorEvents.OnMapToggled.Insert(OnEditorMapToggled);
	}
	
	void OnEditorMapToggled(Class context, MapWidget editor_map, bool state)
	{
		m_LayoutRoot.Show(!state);
	}
	
	override void Update()
	{
		if (m_MapWidget && m_MapWidget.IsVisible()) {
			return;
		}
				
		vector screen_pos = GetGame().GetScreenPos(GetPosition());
						
		int screen_x, screen_y;
		GetScreenSize(screen_x, screen_y);
		// Is the marker in bounds?
		if (screen_pos[0] != 0 && screen_pos[0] != screen_x && screen_pos[1] != 0 && screen_pos[1] != screen_y && screen_pos[2] > 0) {
			SetPos(screen_pos[0], screen_pos[1]);
			Show(m_Show);
		} 
		
		// Overrides the hide if the camera isnt looking at the marker
		else if (m_LayoutRoot)
			m_LayoutRoot.Show(false);		

		
		super.Update();
	}
	
	protected vector GetPosition()
	{
		vector position;
		vector object_transform[4];
		m_EditorObject.GetTransform(object_transform);
		
		// Should the position be raycasted on the ground, or locked to the object
		if (m_Editor.GroundMode) {
			set<Object> o;
			vector ground_dir; int component;
			DayZPhysics.RaycastRV(object_transform[3], object_transform[3] + object_transform[1] * -1000, position, ground_dir, component, o, NULL, m_EditorObject.GetWorldObject(), false, true); // set to ground only
			return position;
		} 
		
		return m_EditorObject.GetBottomCenter();
	}
	
	override void Show(bool show)
	{
		if (vector.Distance(m_Editor.GetCamera().GetPosition(), GetPosition()) > m_Editor.Settings.MarkerViewDistance) {
			m_LayoutRoot.Show(false);
			return;
		}
		
		super.Show(show);
	}
	
	/*
	override bool OnClick(Widget w, int x, int y, int button)
	{
		EditorLog.Trace("EditorObjectWorldMarker::OnClick: " + button);
		
		switch (button) {
			
			case MouseState.RIGHT: {
				GetEditor().SelectObject(m_EditorObject);
				EditorContextMenu context_menu = new EditorContextMenu();
				context_menu.SetPosition(x, y);
				
				return true;
			}			
		}
		
		return false;
	}
	
	/*
	override bool OnClick(Widget w, int x, int y, int button)
	{
		EditorLog.Trace("EditorObjectWorldMarker::OnClick: " + button);
		
		if (button == 0) {
			
			if (GetEditor().IsPlacing()) return false;
			
			// We want to Toggle selection if you are holding control
			if (KeyState(KeyCode.KC_LCONTROL))
				GetEditor().ToggleSelection(m_EditorObject);
			else {
				// required for multiple objects to be dragged
				if (m_EditorObject.IsSelected()) 
					return true;
				
				if (!KeyState(KeyCode.KC_LSHIFT))
					GetEditor().ClearSelection();
				
				GetEditor().SelectObject(m_EditorObject);
			}	
		}
		
		return super.OnClick(w, x, y, button);
	}

	*/

	

}











