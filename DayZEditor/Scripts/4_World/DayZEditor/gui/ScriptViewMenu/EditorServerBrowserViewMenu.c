class EditorServerBrowserViewMenu: ScriptViewMenu
{	
	static ref EditorServerBrowserViewMenu Instance;
	
	EditBoxWidget Host, Password, Join;
	
	void OnCreateExecute(ButtonCommandArgs args)
	{
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(Host.GetText());
		rpc.Write(Password.GetText());
		rpc.Write(32);
		rpc.Send(null, EditorOnlineSessionManager.RPC_REQUEST_CREATE_SESSION, true);
		
		Close();
	}
	
	void OnJoinExecute(ButtonCommandArgs args)
	{
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(Join.GetText());
		rpc.Send(null, EditorOnlineSessionManager.RPC_REQUEST_JOIN_SESSION, true);
		
		Close();
	}
	
	void OnCloseExecute(ButtonCommandArgs args)
	{
		Close();
	}
		
	override string GetLayoutFile()
	{
		return "DayZEditor\\GUI\\layouts\\EditorOnlineBrowser.layout";
	}
}