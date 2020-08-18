



[WorkbenchPluginAttribute("Testing", "Hi mom", "Alt+3", "", {"ResourceManager", "ScriptEditor"})]
class EditorViewOptions: MVCPlugin
{
	
	[Attribute("", "editbox", "ViewModel Widget Name")]
	string ViewModelWidget;

	[Attribute("", "editbox")]
	string VariableName;
	
	[Attribute("", "spinbox")]
	int VariableIndex;
	
	[Attribute("", "editbox")]
	string ControlName;
	
	
	void EditorViewOptions()
	{
		Print("EditorViewOptions");
		m_MVCPlugin = this;
	}
	
	private int m_DialogResult = -1;
	private EditorViewData m_ViewData;
	
	override int ShowDialog(ref EditorViewData view_data) 
	{
		m_ViewData 		= view_data;
		ViewModelWidget = m_ViewData.ViewModelWidget; 
		VariableName 	= m_ViewData.VariableName; 
		VariableIndex 	= m_ViewData.VariableIndex; 
		ControlName 	= m_ViewData.ControlName;
		
		Workbench.ScriptDialog("View Options", "Edit View Binding Options", this);
		
		m_ViewData.ViewModelWidget = ViewModelWidget;
		m_ViewData.VariableName = VariableName;
		m_ViewData.VariableIndex = VariableIndex;
		m_ViewData.ControlName = ControlName;
		
		return m_DialogResult;
	}
	
	override void Run() { m_MVCPlugin = new EditorViewOptions(); }

	[ButtonAttribute("Save", true)]
	void Save()
	{
		m_DialogResult = 1;
		ScriptEditor browser = Workbench.GetModule("ScriptEditor");
		Print(browser);
		
		if (browser) {
			string file;
			browser.GetCurrentFile(file);
			array<string> file_dir = {};
			file.Split("/", file_dir);
			Workbench.GetAbsolutePath(file_dir.Get(0), file);
			file += "/layoutdata.bin";
			FileName = file;
			browser.Save();
		}
	}
	
	[ButtonAttribute("Cancel")]
	void Cancel()
	{
		m_DialogResult = 0;
	}
}

