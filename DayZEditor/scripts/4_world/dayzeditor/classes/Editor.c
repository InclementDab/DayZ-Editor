


class Editor
{
	void Editor(PlayerIdentity player)
	{
		EditorLog.Trace("Editor");
		
		
	}
	
	
	
	void OnUpdate(float timeslice)
	{
		
		Input input = GetGame().GetInput();
		if (input.LocalPress("UAUIMenu", false) || input.LocalPress("UAUIBack", false)) {
			
		}
		
		
		if (input.LocalPress("EditorToggleUI")) {
			
		} 
		
		if (input.LocalPress("EditorToggleCursor")) {

		} 
	}
}
