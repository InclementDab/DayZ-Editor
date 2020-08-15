class EditorMap: EditorWidgetEventHandler
{

	void EditorMap()
	{
		Print("EditorMap");
		//EditorEvents.OnObjectCreated.Insert(OnObjectCreated);
	}
	
	void ~EditorMap()
	{
		Print("~EditorMap");
	}
	
	void OnObjectCreated(Class context, EditorObject obj)
	{
		Print("EditorMap::OnObjectCreated");
		//apWidget map_widget = GetMapWidget();
		//map_widget.AddChild(obj.GetMapMarker());
	}
	
	
	private int start_x, start_y;
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		Input input = GetGame().GetInput();	
		MapWidget map_widget = GetMapWidget();
		
		if (button == 0) {
			if (GetEditor().IsPlacing()) {
				GetEditor().PlaceObject();
				return true;
			} else {
				GetEditor().ClearSelection();
				EditorUI ui = GetEditor().GetUIManager().GetEditorUI();
				ui.GetCanvas().Clear();
				GetCursorPos(ui.start_x, ui.start_y);
				GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(ui.DelayedDragBoxCheck, 40);
				return true;
			}
			
		} else if (button == 2) {
			vector teleport_dest = map_widget.ScreenToMap(Vector(x, y, 0));
			vector current_pos = GetEditor().GetUIManager().GetEditorCamera().GetPosition();
			teleport_dest[1] = current_pos[1] - GetGame().SurfaceY(current_pos[0], current_pos[2]) + GetGame().SurfaceY(teleport_dest[0], teleport_dest[2]);
			GetEditor().GetUIManager().GetEditorCamera().SetPosition(teleport_dest);			
			
			return true;
			
		}
		
		return false;
	}
	
	MapWidget GetMapWidget() { return MapWidget.Cast(m_Root); }
}