



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
	}
	
	private int m_DialogResult = -1;
	
	override int ShowDialog(ref EditorViewData view_data) 
	{
		
		ViewModelWidget = view_data.ViewModelWidget; 
		VariableName 	= view_data.VariableName; 
		VariableIndex 	= view_data.VariableIndex; 
		ControlName 	= view_data.ControlName;
		
		Workbench.ScriptDialog("View Options", "Edit View Binding Options", this);
		
		view_data.ViewModelWidget = ViewModelWidget;
		view_data.VariableName = VariableName;
		view_data.VariableIndex = VariableIndex;
		view_data.ControlName = ControlName;
		
		return m_DialogResult;
	}
	
	override void Run() { m_MVCPlugin = new EditorViewOptions(); }

	[ButtonAttribute("Save", true)]
	void Save()
	{
		m_DialogResult = 1;
	}
	
	[ButtonAttribute("Cancel")]
	void Cancel()
	{
		m_DialogResult = 0;
	}
}

