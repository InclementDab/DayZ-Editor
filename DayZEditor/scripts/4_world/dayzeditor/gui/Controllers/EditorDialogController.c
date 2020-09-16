class EditorDialogController: Controller
{
	string TitleText;
	
	
	void TitleCloseExecute(ButtonCommandArgs args)
	{
		EditorLog.Trace("EditorDialogController::TitleCloseExecute");
		GetEditor().GetEditorHud().GetModal().Close();
	}	
}
