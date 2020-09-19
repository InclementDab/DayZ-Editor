class DialogCloseRelayCommand: RelayCommand
{
	override void Execute(RelayCommandArgs args)
	{	
		EditorLog.Trace("DialogCloseRelayCommand::Execute");
		delete GetEditorHudController().CurrentDialog;
	}
}

class MapSelectDialogRelayCommand: RelayCommand
{
	void MapSelectDialogRelayCommand() {	
		SetCanExecute(false);
	}
	
	override void Execute(RelayCommandArgs args)
	{
		EditorLog.Trace("MapSelectDialogRelayCommand::Execute");
		delete GetEditorHudController().CurrentDialog;
		
		GetGame().PlayMission(CreateEditorMission(MapSelectDialogController.Cast(m_ViewBinding.GetController()).MapHostListboxSelection));
	}
	
	override void CanExecuteChanged(bool state)
	{
		EditorLog.Trace("MapSelectDialogRelayCommand::CanExecuteChanged");
		Widget btn = m_ViewBinding.GetLayoutRoot();

		if (state) {
			btn.SetAlpha(1);
		}
		else btn.SetAlpha(0.3); 
		
		btn.Enable(state);
	}
}