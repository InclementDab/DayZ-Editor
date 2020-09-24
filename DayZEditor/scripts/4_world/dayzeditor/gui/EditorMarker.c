
static const float ALPHA_ON_SHOW = 1;
static const float ALPHA_ON_HIDE = 0.25;

class EditorMarker: ScriptedWidgetEventHandler
{
	protected Widget m_LayoutRoot;
	Widget GetLayoutRoot() { return m_LayoutRoot; }

	void EditorMarker()
	{
		EditorLog.Trace("EditorMarker");
		m_LayoutRoot = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/EditorMarker.layout");
		m_LayoutRoot.SetHandler(this);
		

		
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
	}
	
	void ~EditorMarker()
	{
		EditorLog.Trace("~EditorMarker");
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
		m_LayoutRoot.Unlink();
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
		m_LayoutRoot.Show(show);
	}
	
	protected bool IsMouseInside(int c_x, int c_y)
	{
		float x, y, w, h;
		m_LayoutRoot.GetPos(x, y);
		m_LayoutRoot.GetSize(w, h);
		return (c_x < x + h && c_x > x - h) && (c_y < y + h && c_y > y - h);
	}

}

class EditorObjectMarker: EditorMarker
{	
	protected EditorObject m_EditorObject;
	EditorObject GetEditorObject() { return m_EditorObject; }
	
	protected ref DragHandler m_DragHandler;
	DragHandler GetDragHandler() { return m_DragHandler; }
	
	void EditorObjectMarker(EditorObject editor_object)
	{
		EditorLog.Trace("EditorObjectMarker");
		m_EditorObject = editor_object;
	}
	
	override void Update()
	{
		int x, y;
		GetMousePos(x, y);
		if (m_EditorObject.IsSelected() || IsMouseInside(x, y)) 
			m_LayoutRoot.SetAlpha(ALPHA_ON_SHOW);
		else 
			m_LayoutRoot.SetAlpha(ALPHA_ON_HIDE);
	}
	
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{		
		// ignores the object if you are placing
		if (GetEditor().IsPlacing()) return false;
		
		// We want to Toggle selection if you are holding control
		
		switch (button) {
			
			case 0: {
				if (KeyState(KeyCode.KC_LCONTROL)) {
					GetEditor().ToggleSelection(m_EditorObject);
				} else {
					if (!KeyState(KeyCode.KC_LSHIFT))
						GetEditor().ClearSelection();
					
					GetEditor().SelectObject(m_EditorObject);
				}
				
				GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(CheckDragBounds, 0, true, x, y);
				
				break;
			}
			
			case 2: {
				EditorCamera camera = GetEditor().GetCamera();
				vector pos = m_EditorObject.GetPosition();
				pos[1] = camera.GetPosition()[1];
				camera.SetPosition(pos);
				break;
			}
		}
		
		return true;
	}
	
	private const int DRAG_THRESHOLD = 10;
	private void CheckDragBounds(int x, int y)
	{
		if (GetMouseState(MouseState.LEFT) & MB_PRESSED_MASK) {
			int c_x, c_y;
			GetMousePos(c_x, c_y);
			
			int dist_x = Math.AbsInt(x - c_x);
			int dist_y = Math.AbsInt(y - c_y);
			
			if (dist_x + dist_y > DRAG_THRESHOLD) {
				GetEditor().SelectObject(m_EditorObject);
				m_DragHandler.OnDragStart();
				GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(CheckDragBounds);
			}
			
			
		} else {
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(CheckDragBounds);
		}
	}
}


class EditorObjectMapMarker: EditorObjectMarker
{		
	
	private MapWidget m_EditorMap;
	
	void EditorObjectMapMarker(EditorObject editor_object)
	{
		m_DragHandler = new MapDragHandler(m_EditorObject);
		m_EditorMap = GetEditor().GetEditorHud().EditorMapWidget;
	}
	
	override void Update()
	{
		if (!m_EditorMap.IsVisible()) return;
		
		vector position = m_EditorMap.MapToScreen(m_EditorObject.GetPosition());
		SetPos(position[0], position[1]);
		super.Update();
	}
}

class EditorObjectWorldMarker: EditorObjectMarker
{
	void EditorObjectWorldMarker(EditorObject editor_object)
	{
		m_DragHandler = new ObjectDragHandler(m_EditorObject);
		EditorEvents.OnMapToggled.Insert(OnEditorMapToggled);
	}
	
	void OnEditorMapToggled(Class context, MapWidget editor_map, bool state)
	{
		m_LayoutRoot.Show(!state);
	}
	
	override void Update()
	{
		if (GetEditor().GetEditorHud().EditorMapWidget.IsVisible()) {
			return;
		}
		
		vector position;
		vector object_transform[4];
		m_EditorObject.GetTransform(object_transform);
		
		// Should the position be raycasted on the ground, or locked to the object
		if (GetEditor().GetEditorHud().GetEditorHudController().GroundButton) {
			set<Object> o;
			vector ground_dir; int component;
			DayZPhysics.RaycastRV(object_transform[3], object_transform[3] + object_transform[1] * -1000, position, ground_dir, component, o, NULL, m_EditorObject.GetWorldObject(), false, true); // set to ground only
			
		} else position = m_EditorObject.GetBottomCenter();
	
		vector screen_pos = GetGame().GetScreenPos(position);
		
		if (screen_pos[2] > 0)
			SetPos(screen_pos[0], screen_pos[1]);
		/*
		if (vector.Distance(m_EditorObject.GetPosition(), GetEditor().GetCamera().GetPosition()) > GetEditor().GetSettings().MarkerViewDistance) {
			Show(false);
		} else {
			cant call show on update anymore since its called in EditorObject
			Show(m_EditorObject.IsVisible());
		}*/
		
		
		super.Update();
	}
	
	
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		EditorLog.Trace("EditorObjectWorldMarker::OnClick: " + button);
		
		switch (button) {
			
			case 1: {
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
	}*/

	
	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{		
		EditorObjectPropertiesDialog dialog = new EditorObjectPropertiesDialog();
		dialog.SetEditorObject(m_EditorObject);
		dialog.ShowDialog();
		
		return true;
	}

	

}











