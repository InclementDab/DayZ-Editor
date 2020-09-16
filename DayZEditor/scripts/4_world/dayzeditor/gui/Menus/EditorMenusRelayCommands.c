
class EditorMenuBarFileRelayCommand: RelayCommand
{

	
	override void Execute(RelayCommandArgs args)
	{
		EditorLog.Trace("EditorMenuBarFileRelayCommand::Execute");
		
		EditorMenu file_menu();
		file_menu.AddItem("Test1");
		ButtonWidget bw = ButtonCommandArgs.Cast(args.CommandArgs).GetButtonWidget();
		float x, y;
		bw.GetPos(x, y);
		file_menu.SetPosition(x, y);
		file_menu.Show();
		
		/*EditorMenuItem item_new();
		item_new.SetText("New");
		file_menu.AddItem(item_new);
		*/
	}
}

class EditorMenuItemRelayCommand: RelayCommand
{
	private func m_ExecuteFunc;
	void SetExecuteFunction(func execute_func) {
		m_ExecuteFunc = execute_func;
	}
	
	override void Execute(RelayCommandArgs args) {
		EditorLog.Trace("EditorMenuItemRelayCommand::Execute");
		if (m_ExecuteFunc)
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(m_ExecuteFunc, args);
	}
}

