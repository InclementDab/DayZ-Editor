class DialogCloseRelayCommand: RelayCommand
{
	override void Execute(RelayCommandArgs args)
	{	
		EditorLog.Trace("DialogCloseRelayCommand::Execute");
		GetEditor().GetEditorHud().GetModal().Close();
	}
}

class MapSelectDialogRelayCommand: RelayCommand
{
	void MapSelectDialogRelayCommand()
	{	
		SetCanExecute(false);
	}
	
	override void Execute(RelayCommandArgs args)
	{
		EditorLog.Trace("MapSelectDialogRelayCommand::Execute");
		
		GetEditor().GetEditorHud().GetModal().Close();
		GetGame().PlayMission(CreateEditorMission(MapSelectDialogController.Cast(m_ViewBinding.GetController()).MapHostListboxSelection));
	}
	
	override void CanExecuteChanged(bool state)
	{
		EditorLog.Trace("MapSelectDialogRelayCommand::CanExecuteChanged");
		Widget btn = m_ViewBinding.GetLayoutRoot().FindAnyWidget("DialogButton");

		if (state) {
			btn.SetAlpha(1);
		}
		else btn.SetAlpha(0.3); 
		
		btn.Enable(state);
	}
}