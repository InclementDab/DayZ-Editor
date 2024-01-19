class EditorJoinOnlineSessionViewController: ViewController
{
	bool Hide;

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
		
	PasswordEditBoxWidget PasswordEditBox;
	
	void EditorJoinOnlineSessionView()
	{
		m_TemplateController = EditorJoinOnlineSessionViewController.Cast(m_Controller);
	}
	
	void OnJoinExecute(ButtonCommandArgs args)
	{
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(PasswordEditBox.GetText());
		rpc.Send(null, EditorOnlineSessionManager.RPC_REQUEST_JOIN_SESSION, true);
		
		Close();
	}
		
	void OnCloseExecute(ButtonCommandArgs args)
	{
		Close();
	}
		
	override string GetLayoutFile()
	{
		return "Editor\\GUI\\layouts\\EditorJoinOnlineSession.layout";
	}
	
	override typename GetControllerType()
	{
		return EditorJoinOnlineSessionViewController;
	}
}