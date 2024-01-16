class EditorJoinOnlineSessionViewController: ViewController
{
	bool Hide;
	int Code;
	
	PasswordEditBoxWidget PasswordEditBox;
	
	override void PropertyChanged(string property_name)
	{
		switch (property_name) {
			case "Hide": {
				PasswordEditBox.SetHideText(Hide);
				break;
			}
		}
	}
}

class EditorJoinOnlineSessionView: ScriptViewMenu
{
	protected EditorJoinOnlineSessionViewController m_TemplateController;
		
	void EditorJoinOnlineSessionView()
	{
		m_TemplateController = EditorJoinOnlineSessionViewController.Cast(m_Controller);
	}
	
	void OnJoinExecute(ButtonCommandArgs args)
	{
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(m_TemplateController.Code);
		rpc.Send(null, EditorOnlineSessionManager.RPC_REQUEST_JOIN_SESSION, true);
		
		Close();
	}
		
	void OnCloseExecute(ButtonCommandArgs args)
	{
		Close();
	}
		
	override string GetLayoutFile()
	{
		return "DayZEditor\\GUI\\layouts\\EditorJoinOnlineSession.layout";
	}
	
	override typename GetControllerType()
	{
		return EditorJoinOnlineSessionViewController;
	}
}