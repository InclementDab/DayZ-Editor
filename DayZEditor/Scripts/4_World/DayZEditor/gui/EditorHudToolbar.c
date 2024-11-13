class EditorHudToolbar: ScriptView
{	
	ButtonWidget BrushToggleButton;
	
	override string GetLayoutFile() 
	{
		return "DayZEditor/gui/layouts/hud/EditorHudToolbar.layout";
	}
	
	override typename GetControllerType() 
	{
		return EditorHudToolbarController;
	}
}