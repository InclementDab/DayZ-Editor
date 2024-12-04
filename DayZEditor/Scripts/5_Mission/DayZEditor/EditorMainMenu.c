class EditorMainMenuController: ViewController
{
	ref ObservableCollection<ref EditorMapView> MapViews = new ObservableCollection<ref EditorMapView>(this);
}

class EditorMainMenu: ScriptViewMenu
{
	protected EditorMainMenuController m_TemplateController;

	TextWidget VersionText, EditorText;
	
	void EditorMainMenu()
	{
		m_TemplateController = EditorMainMenuController.Cast(m_Controller);
		for (int i = 0; i < GetGame().ConfigGetChildrenCount("CfgWorlds"); i++) {
			string name;
			GetGame().ConfigGetChildName("CfgWorlds", i, name);
			if (GetGame().ConfigIsExisting(string.Format("CfgWorlds %1 worldName", name))) {
				m_TemplateController.MapViews.Insert(new EditorMapView(name));
			}
		}
		
		string version;
		GetGame().GetVersion(version);
		VersionText.SetText(string.Format("#main_menu_version %1", version));
		
		EditorText.SetText(string.Format("#STR_EDITOR_MAIN_MENU_VERSION %1, created by InclementDab", Editor.Version));
	}

	override void Update(float dt)
	{
		super.Update(dt);
	}

	override bool UseMouse()
	{
		return true;
	}

	override typename GetControllerType()
	{
		return EditorMainMenuController;
	}

	override string GetLayoutFile()
	{
		return "DayZEditor\\GUI\\layouts\\EditorMainMenu2.layout";
	}
}