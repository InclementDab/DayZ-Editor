class EditorFileView: ScriptView
{
	protected string m_File;
	protected bool m_IsDirectory;
	protected float m_ClickTick;
	
	ImageWidget Icon;
	TextWidget FileName, Extension;

	void EditorFileView(string file, bool is_directory)
	{
		m_File = file;
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
        EditorFileDialog parent_dialog = EditorFileDialog.Cast(GetParent());
        if (!parent_dialog) return super.OnMouseButtonDown(w, x, y, button);
		
		if (GetGame().GetTickTime() < m_ClickTick + 0.3) {
            GetGame().GameScript.CallFunctionParams( parent_dialog, "OnFileDoublePressed", null, new Param2<string, bool>(m_File, m_IsDirectory) );
			m_ClickTick = 0;
		} else {
			m_ClickTick = GetGame().GetTickTime();
            GetGame().GameScript.CallFunctionParams( parent_dialog, "OnFilePressed", null, new Param1<string>(m_File) );
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
	
	string GetFile()
	{
		return m_File;
	}

	override string GetLayoutFile()
	{
		return "DayZEditor/GUI/layouts/dialogs/File.layout";
	}
}