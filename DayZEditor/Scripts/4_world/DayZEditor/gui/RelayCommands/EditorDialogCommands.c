/*class DialogCloseRoutedUICommand: RoutedUICommand
{
	override void Execute(RoutedUICommandArgs args)
	{	
		EditorLog.Trace("DialogCloseRoutedUICommand::Execute");
		delete EditorUIManager.CurrentDialog;
	}
}

class MapSelectDialogRoutedUICommand: RoutedUICommand
{
	void MapSelectDialogRoutedUICommand(ref array<KeyCode> input_gestures = null) {	
		SetCanExecute(false);
	}
	
	override void Execute(RoutedUICommandArgs args)
	{
		EditorLog.Trace("MapSelectDialogRoutedUICommand::Execute");
		delete EditorUIManager.CurrentDialog;
		
		GetGame().PlayMission(CreateEditorMission(MapSelectDialogController.Cast(m_ViewBinding.GetController()).MapHostListboxSelection));
	}
	
	override void CanExecuteChanged(bool state)
	{
		EditorLog.Trace("MapSelectDialogRoutedUICommand::CanExecuteChanged");
		Widget btn = m_ViewBinding.GetLayoutRoot();

		if (state) {
			btn.SetAlpha(1);
		}
		else btn.SetAlpha(0.3); 
		
		btn.Enable(state);
	}
}*/