modded class MainMenu 
{
	protected ImageWidget m_Logo;
	
	override Widget Init()
	{
		Print("Editor::MainMenu");
		super.Init();
		
		m_ChooseServer.Show(true);
		m_CustomizeCharacter.Show(false);
		m_Stats.HideStats();
		
		Widget c = layoutRoot.FindAnyWidget("character");
		c.Show(false);
		
		TextWidget tw = TextWidget.Cast(layoutRoot.FindAnyWidget("play_label"));
		tw.SetText("Open Editor");
		
		m_Logo = ImageWidget.Cast(layoutRoot.FindAnyWidget("dayz_logo"));
		m_Logo.LoadImageFile(0, "DayZEditor/gui/images/logo_editor_big.edds");
		m_Logo.SetImage(0);
		m_Logo.SetFlags(m_Logo.GetFlags() | WidgetFlags.SOURCEALPHA | WidgetFlags.BLEND | WidgetFlags.STRETCH);
		
		return layoutRoot;
	}

    override void Play()
    {
		thread CreateLoadDialog();
    }
	
	// Copy pasted from EditorLoadMapCommand
	// but since editor isnt loaded, cant run it
	private void CreateLoadDialog()
	{
		EditorMapSelectDialog select_dialog = new EditorMapSelectDialog("Select Map...");
		string selected_map;
		DialogResult result = select_dialog.ShowDialog(selected_map);
		
		if (selected_map != string.Empty && result == DialogResult.OK) {
			GetGame().PlayMission(CreateEditorMission(selected_map));
		}
	}

    override bool OnMouseEnter(Widget w, int x, int y)
    {
		if (IsFocusable(w)) {
			ColorHighlight(w);
			return true;
		}
		return false;
	}
}