

class EditorFileMenuItemCommand: RelayCommand
{
	
	override void Execute(RelayCommandArgs args)
	{
		GetEditor().GetEditorHud().CloseMenu();
	}
	
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
		
		GetEditor().New();
		
		super.Execute(args);
	}
}

class EditorFileMenuOpenCommand: EditorFileMenuItemCommand
{	
	override void Execute(RelayCommandArgs args)
	{
		EditorLog.Trace("EditorFileMenuOpenCommand::Execute");
		
		GetEditor().Open();
		
		super.Execute(args);
	}
}

class EditorFileMenuSaveCommand: EditorFileMenuItemCommand
{
	override void Execute(RelayCommandArgs args)
	{
		EditorLog.Trace("EditorFileMenuSaveCommand::Execute");
		
		//GetEditor().Save();
		
		super.Execute(args);
	}
}

class EditorFileMenuSaveAsCommand: EditorFileMenuItemCommand
{
	override void Execute(RelayCommandArgs args)
	{
		EditorLog.Trace("EditorFileMenuSaveAsCommand::Execute");
		
		
		super.Execute(args);
	}
}

class EditorFileMenuCloseCommand: EditorFileMenuItemCommand
{
	override void Execute(RelayCommandArgs args)
	{
		EditorLog.Trace("EditorFileMenuCloseCommand::Execute");
		GetEditor().Close();
		super.Execute(args);
	}
}



