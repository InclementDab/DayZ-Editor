class EditorMainMenu: ScriptViewMenu
{	
	protected EditorMainMenuController m_TemplateController;
	
	Widget Main, Host, Join, Offline;
	
	void EditorMainMenu()
	{
		m_TemplateController = EditorMainMenuController.Cast(m_Controller);
	}
	
	override void Update(float dt)
	{
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		TextWidget tw = FindWidget<TextWidget>.SearchDown(w, "Text");
		return false;
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		return false;
	}
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		switch (w) {
			case Offline: {				
				GetGame().PlayMission("P:\\Missions\\EditorOnline.ChernarusPlus");
				break;
			}
		}
		
		return false;
	}

	override bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
		return false;
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