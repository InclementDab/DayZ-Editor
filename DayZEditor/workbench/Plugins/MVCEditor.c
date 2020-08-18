
typedef int[] FrameWidgetClass;




[WorkbenchPluginAttribute("Testing", "Hi mom", "Alt+3", "", {"ResourceManager", "ScriptEditor"})]
class EditorViewOptions: MVCPlugin
{
	
	protected ResourceBrowser m_Module;
	
	void ResourceSearchCb(string file)
	{
		Print("Resource Found! " + file);
					
		m_Module.SetOpenedResource(file);
		WidgetSource widget = m_Module.GetContainer();
		
		WidgetSource widget_chidren = widget.GetChildren();
		Print(widget_chidren.GetClassName());
		Print(widget_chidren.GetName());
		
		
		
		int var_index = widget_chidren.VarIndex("scriptclass");
		Print(var_index);
		
		
		if (widget_chidren.IsVariableSet(var_index)) {
			string scriptclass;
			widget_chidren.Get(var_index, scriptclass);
			Print(scriptclass);
		} else {
			Print("Vartype");
			Print(widget_chidren.IsType(var_index, string));
		}
		
		for (int i = 0; i < 10; i++) {
			
		}
		
		//widget.Get(0, frame_widget);
		
		
		
	}
	
	
	override void Run()
	{
		
		m_Module = Workbench.GetModule("ResourceManager");
		Workbench.SearchResources("EditorObjectProperties.layout", ResourceSearchCb);
		
		
		Workbench.ScriptDialog("View Options", "Edit View Binding Options", m_EditorViewData);
	}
	
	[ButtonAttribute("Save", true)]
	void Save()
	{
		m_DialogResult = 1;
	}
	
	[ButtonAttribute("Cancel")]
	void Cancel() { }
	
	
	
}


