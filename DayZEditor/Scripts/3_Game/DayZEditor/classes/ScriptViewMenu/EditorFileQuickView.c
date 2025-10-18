class EditorFileQuickView: ScriptView
{
	protected string m_FullPath;
	TextWidget Text;

	protected ref ScriptCaller m_OnClicked;

	void EditorFileQuickView(string full_path, ScriptCaller on_clicked)
	{
		m_FullPath = full_path;
		m_OnClicked = on_clicked;
		
		string text = File.GetName(full_path);
		if (!text) {
			text = full_path;
		}
		
		Text.SetText(text);
	}

	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		if (m_OnClicked) {
			m_OnClicked.Invoke(this, m_FullPath);
		}
		
		return super.OnMouseButtonDown(w, x, y, button);
	}
	
	override string GetLayoutFile()
	{
		return "DayZEditor/GUI/layouts/dialogs/FileDialogFolderQuickView.layout";
	}
}