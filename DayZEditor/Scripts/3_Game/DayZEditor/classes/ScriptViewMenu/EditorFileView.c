class EditorFileView: ScriptView
{
	protected string m_File;
	protected ref ScriptCaller m_OnClicked, m_OnDoubleClicked;
	protected bool m_IsDirectory;
	protected float m_ClickTick;
	
	ImageWidget Icon;
	TextWidget FileName, Extension;

	void EditorFileView(string file, ScriptCaller on_click, ScriptCaller on_double_click, bool is_directory)
	{
		m_File = file;
		m_OnClicked = on_click;
		m_OnDoubleClicked = on_double_click;
		m_IsDirectory = is_directory;

		string file_name = File.GetName(m_File);
		string file_extension = File.GetExtension(m_File);
		FileName.SetText(file_name);
		
		// Set icon to folder
		if (is_directory) {
			Symbols.FOLDER.Load(Icon, 3);
			Icon.SetColor(0xFFFFCD45);
		} else {
			Extension.SetText(file_extension);
		}
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		
		
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		return super.OnMouseLeave(w, enterW, x, y);
	}

	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		if (m_OnClicked) {
			m_OnClicked.Invoke(this, m_File);
		}
		
		if (GetGame().GetTickTime() < m_ClickTick + 0.3) {
			if (m_OnDoubleClicked) {
				m_OnDoubleClicked.Invoke(this, m_File);
			}
			m_ClickTick = 0;
		} else {
			m_ClickTick = GetGame().GetTickTime();
		}
		
		return super.OnMouseButtonDown(w, x, y, button);
	}

	override bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
		return super.OnMouseButtonUp(w, x, y, button);
	}

	bool IsDirectory()
	{
		return m_IsDirectory;
	}

	override string GetLayoutFile()
	{
		return "DayZEditor/GUI/layouts/dialogs/File.layout";
	}
}