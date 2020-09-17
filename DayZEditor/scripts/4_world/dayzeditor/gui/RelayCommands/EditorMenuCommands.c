

class EditorFileMenuItemCommand: RelayCommand
{
	
	override void CanExecuteChanged(bool state)
	{
		EditorLog.Trace("EditorFileMenuItemCommand::CanExecuteChanged");

		if (state) {
			m_ViewBinding.GetLayoutRoot().FindAnyWidget("EditorMenuItemLabel").SetAlpha(1);
			m_ViewBinding.GetLayoutRoot().FindAnyWidget("EditorMenuItemIcon").SetAlpha(1);
		} else {
			m_ViewBinding.GetLayoutRoot().FindAnyWidget("EditorMenuItemLabel").SetAlpha(0.3);
			m_ViewBinding.GetLayoutRoot().FindAnyWidget("EditorMenuItemIcon").SetAlpha(0.3);	
		}	
		
		m_ViewBinding.GetLayoutRoot().Enable(state);	
	}
}


class EditorFileMenuNewCommand: EditorFileMenuItemCommand
{
	override void Execute(RelayCommandArgs args)
	{
		EditorLog.Trace("EditorFileMenuNewCommand::Execute");
	}
}

class EditorFileMenuOpenCommand: EditorFileMenuItemCommand
{	
	override void Execute(RelayCommandArgs args)
	{
		EditorLog.Trace("EditorFileMenuOpenCommand::Execute");
	}
}




