class EditorUIManager
{
	// Modal Menu Control
	static ref EditorMenu CurrentMenu;
	
	// ToolTip Control
	static ref EditorTooltip CurrentTooltip;
		
	// Dialog Control
	static ref DialogBase CurrentDialog;
	
	static 	bool IsDialogCommand(Widget w) {
		return (CurrentDialog && CurrentDialog.GetLayoutRoot() && CurrentDialog.GetLayoutRoot().FindAnyWidget(w.GetName()));
	}
	
	static ref EditorHudController CurrentEditorHudController;
	static ref EditorHudToolbarController CurrentEditorHudToolbarController;
}