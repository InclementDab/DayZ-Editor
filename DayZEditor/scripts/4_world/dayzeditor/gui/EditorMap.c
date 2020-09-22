
class EditorMap: ScriptView
{
	MapWidget EditorMapWidget;
		
	override string GetLayoutFile() {
		return "DayZEditor/gui/layouts/EditorMap.layout";
	}
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		EditorLog.Trace("EditorMap::OnMouseButtonDown");
		
		switch (button) {
			
			case MouseState.LEFT: {
				GetEditor().GetEditorHud().GetEditorHudController().DelayedDragBoxCheck();
				return true;
			}	
			
			// MouseState.MIDDLE is handled in the Hud layer		
		}
		
		return false;
	}
}
