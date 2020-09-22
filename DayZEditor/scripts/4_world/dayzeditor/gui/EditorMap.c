
class EditorMapController: Controller
{
	
}

class EditorMap: MVCLayout
{
	
	protected MapWidget EditorMapWidget;
	
	// Temp hopefully
	MapWidget GetMapWidget() {
		return EditorMapWidget;
	}
	
	override string GetLayoutFile() {
		return "DayZEditor/gui/layouts/EditorMap.layout";
	}
	
	override typename GetControllerType() {
		return EditorMapController;
	}
	
	EditorMapController GetEditorMapController() { 
		return EditorMapController.Cast(GetController());
	}
}



/*
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		EditorLog.Trace("EditorMap::OnMouseButtonDown");
		
		switch (button) {
			
			case MouseState.LEFT: {
				GetEditor().GetEditorHud().GetEditorHudController().DelayedDragBoxCheck();
				return true;
			}
			
			case MouseState.MIDDLE: {
				vector pos = GetCursorPosition();
				pos[1] = GetEditor().GetCamera().GetPosition()[1];
				GetEditor().GetCamera().SetPosition(pos);
				break;
			}
			
		}
		
		return false;
	}
	
	vector GetCursorPosition()
	{
		int x, y;
		GetMousePos(x, y);
		return GetMapWidget().ScreenToMap(Vector(x, y, 0));
	}*/