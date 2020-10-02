class EditorUIManager
{
	// Modal Menu Control
	static autoptr EditorMenu CurrentMenu;
	
	// ToolTip Control
	static autoptr ScriptView CurrentTooltip;
		
	// Dialog Control
	static autoptr DialogBase CurrentDialog;
	
	static bool IsDialogCommand(Widget w) {
		return (CurrentDialog && CurrentDialog.GetLayoutRoot() && CurrentDialog.GetLayoutRoot().FindAnyWidget(w.GetName()));
	}
	
	static autoptr EditorHudController CurrentEditorHudController;
	static autoptr EditorHudToolbarController CurrentEditorHudToolbarController;
}