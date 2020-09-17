

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


class EditorToolbarOpenCommand: EditorToolbarItemCommand
{	
	void EditorToolbarOpenCommand()
	{
		SetCanExecute(false);
	}
	
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

class EditorToolbarUndoCommand: EditorToolbarItemCommand
{
	override void Execute(RelayCommandArgs args)
	{
		EditorLog.Trace("EditorToolbarUndoCommand::Execute");
		GetEditor().GetObjectManager().Undo();
		super.Execute(args);
	}
}

class EditorToolbarRedoCommand: EditorToolbarItemCommand
{
	override void Execute(RelayCommandArgs args)
	{
		EditorLog.Trace("EditorToolbarRedoCommand::Execute");
		GetEditor().GetObjectManager().Redo();
		super.Execute(args);
	}
}

class EditorToolbarCameraControlCommand: EditorToolbarItemCommand
{
	override void Execute(RelayCommandArgs args)
	{
		EditorLog.Trace("EditorToolbarCameraControlCommand::Execute");
		EditorCameraDialog cam_dialog = new EditorCameraDialog(GetEditor().GetCamera());
		cam_dialog.Show();
		
		super.Execute(args);
	}
}

class EditorToolbarReloadUICommand: EditorToolbarItemCommand
{
	override void Execute(RelayCommandArgs args)
	{
		EditorLog.Trace("EditorToolbarReloadUICommand::Execute");
		super.Execute(args);
		GetEditor().ReloadHud();
	}
}



