class EditorCreateOnlineSessionViewController: ViewController
{
	bool Public;
	int MaxPlayers;
	string Password;
}

class EditorCreateOnlineSessionView: ScriptViewMenu
{	
	protected EditorCreateOnlineSessionViewController m_TemplateController;
		
	void EditorCreateOnlineSessionView()
	{
		m_TemplateController = EditorCreateOnlineSessionViewController.Cast(m_Controller);
	}
	
	void OnCreateExecute(ButtonCommandArgs args)
	{
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(m_TemplateController.Public);
		rpc.Write(m_TemplateController.MaxPlayers);
		rpc.Write(m_TemplateController.Password);
		rpc.Send(null, EditorOnlineSessionManager.RPC_REQUEST_CREATE_SESSION, true);
		
		Close();
	}
		
	void OnCloseExecute(ButtonCommandArgs args)
	{
		Close();
	}
		
	override string GetLayoutFile()
	{
		return "Editor\\GUI\\layouts\\EditorCreateOnlineSession.layout";
	}
	
	override typename GetControllerType()
	{
		return EditorCreateOnlineSessionViewController;
	}
}