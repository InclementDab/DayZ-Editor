


[WorkbenchPluginAttribute("Testing", "Hi mom", "Alt+3", "", {"ResourceManager", "ScriptEditor"})]
class EditorViewOptions: WorkbenchPlugin
{
	private static ref array<ref ParamEnum> param_enums = {};
	[Attribute("0", "combobox", "ViewBinding: ", "", param_enums)]
	int CurrentViewEdit;
	
	
	protected ResourceBrowser m_Module;
	protected ControllerBase m_Controller;
	
	void ResourceSearchCb(string file)
	{
		Print("Resource Found! " + file);

		m_Module.SetOpenedResource(file);
		WidgetSource widget = m_Module.GetContainer();	
		m_Controller = ControllerBaseHashMap.Get(ControllerBase.GetFromWidget(widget));	
		EnumerateViewBindings(widget, param_enums);

		
		Workbench.ScriptDialog("View Options", "Edit View Binding Options", this);
	
	}
	
	void EnumerateViewBindings(WidgetSource source, out ref array<ref ParamEnum> view_bindings)
	{
		if (!source) return;
			
		string script;
		source.Get(source.VarIndex("scriptclass"), script);
		
		if (script == "EditorView") {
			view_bindings.Insert(ParamEnum(source.GetName(), view_bindings.Count().ToString()));
		}
		
		EnumerateViewBindings(source.GetChildren(), view_bindings);
		EnumerateViewBindings(source.GetSibling(), view_bindings);
	}
		
	
	override void Run()
	{		
		m_Module = Workbench.GetModule("ResourceManager");
		Workbench.SearchResources("EditorObjectProperties.layout", ResourceSearchCb);		
	}
		

	
	[ButtonAttribute("Edit")]
	void Edit() 
	{
		EditorViewBase view = m_Controller.GetEditorView(param_enums.Get(CurrentViewEdit).m_Key);
		Print(view);
		EditorViewData data = view.GetData();
		Print(data);
		Workbench.ScriptDialog("Edit View Data", "Edit View Binding Options", data);
	}
	
	[ButtonAttribute("Close")]
	void Close() { }
	
}


