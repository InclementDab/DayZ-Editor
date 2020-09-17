class DialogCloseRelayCommand: RelayCommand
{
	override void Execute(RelayCommandArgs args)
	{	
		EditorLog.Trace("DialogCloseRelayCommand::Execute");
		GetEditor().GetEditorHud().GetModal().Close();
	}
}