class EditorUIManager
{	
	// Modal Menu Control
	static ref EditorMenu CurrentMenu;
	
	// ToolTip Control
	private static ref ScriptView CurrentTooltip;
	static void SetCurrentTooltip(ScriptView current_tooltip) 
	{
		if (CurrentTooltip) {
			delete CurrentTooltip;
		}
		
		CurrentTooltip = current_tooltip;
	}
		
	// Dialog Control
	static ref DialogBase CurrentDialog;
	
	static bool IsDialogCommand(Widget w) {
		return (CurrentDialog && CurrentDialog.GetLayoutRoot() && CurrentDialog.GetLayoutRoot().FindAnyWidget(w.GetName()));
	}
	
	static float DialogLastX = -1;
	static float DialogLastY = -1;
	
	static ref EditorHudController CurrentEditorHudController;
	static ref EditorHudToolbarController CurrentEditorHudToolbarController;
}