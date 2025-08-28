class EditorDirectoryView: ScriptView
{
	protected string m_Directory;
	protected ref ScriptCaller m_OnClicked;
	TextWidget Text;

	void EditorDirectoryView(string directory, ScriptCaller on_click)
	{
		m_Directory = directory;
		m_OnClicked = on_click;
		
		Text.SetText(m_Directory);
	}

	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		if (m_OnClicked) {
			m_OnClicked.Invoke(this, m_Directory);
		}
		
		return super.OnMouseButtonDown(w, x, y, button);
	}

	override bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
		return super.OnMouseButtonUp(w, x, y, button);
	}

	override string GetLayoutFile()
	{
		return "DayZEditor/GUI/layouts/dialogs/Directory.layout";
	}
}