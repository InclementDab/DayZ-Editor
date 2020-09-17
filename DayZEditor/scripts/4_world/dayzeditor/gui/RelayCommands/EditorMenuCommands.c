

class EditorToolbarItemCommand: RelayCommand
{
	
	override void Execute(RelayCommandArgs args)
	{
		GetEditor().GetEditorHud().CloseMenu();
	}
	
	override void CanExecuteChanged(bool state)
	{
		EditorLog.Trace("EditorToolbarItemCommand::CanExecuteChanged");

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


class EditorToolbarNewCommand: EditorToolbarItemCommand
{
	override void Execute(RelayCommandArgs args)
	{
		EditorLog.Trace("EditorToolbarNewCommand::Execute");
		
		GetEditor().New();
		
		super.Execute(args);
	}
}

class EditorToolbarOpenCommand: EditorToolbarItemCommand
{	
	override void Execute(RelayCommandArgs args)
	{
		EditorLog.Trace("EditorToolbarOpenCommand::Execute");
		
		GetEditor().Open();
		
		super.Execute(args);
	}
}

class EditorToolbarSaveCommand: EditorToolbarItemCommand
{
	override void Execute(RelayCommandArgs args)
	{
		EditorLog.Trace("EditorToolbarSaveCommand::Execute");
		
		//GetEditor().Save();
		
		super.Execute(args);
	}
}

class EditorToolbarSaveAsCommand: EditorToolbarItemCommand
{
	override void Execute(RelayCommandArgs args)
	{
		EditorLog.Trace("EditorToolbarSaveAsCommand::Execute");
		
		
		super.Execute(args);
	}
}

class EditorToolbarCloseCommand: EditorToolbarItemCommand
{
	override void Execute(RelayCommandArgs args)
	{
		EditorLog.Trace("EditorToolbarCloseCommand::Execute");
		GetEditor().Close();
		super.Execute(args);
	}
}



