class EditorMainMenu: ScriptViewMenu
{	
	protected EditorMainMenuController m_TemplateController;
	
	void EditorMainMenu()
	{
		m_TemplateController = EditorMainMenuController.Cast(m_Controller);
	}
	
	override string GetLayoutFile()
	{
		return "Editor\\GUI\\layouts\\EditorMainMenu.layout";
	}
	
	override typename GetControllerType()
	{
		return EditorMainMenuController;
	}
}