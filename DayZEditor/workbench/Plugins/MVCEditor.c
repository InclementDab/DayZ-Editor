


[WorkbenchPluginAttribute("Testing", "Hi mom", "Alt+3", "", {"ResourceManager", "ScriptEditor"})]
class EditorViewOptions: MVCPlugin
{
	private static ref array<ref ParamEnum> param_enums = {};
	[Attribute("0", "combobox", "ViewBinding: ", "", param_enums)]
	int CurrentViewEdit;
	
	
	protected ResourceBrowser m_Module;
	
	
	void ResourceSearchCb(string file)
	{
		Print("Resource Found! " + file);

		m_Module.SetOpenedResource(file);
		WidgetSource widget = m_Module.GetContainer();
		
		ControllerBase controller = GetController(widget);
	
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
	
	ControllerBase GetController(WidgetSource source)
	{
		if (!source) return null;
		
		string script;
		source.Get(source.VarIndex("scriptclass"), script);
		typename type = script.ToType();
		if (type.IsInherited(ControllerBase)) {
			Print("Controller Found");
			return ControllerHashMap.Get(source.GetName());
		}
		
		if (GetController(source.GetChildren()) != null) {
			return GetController(source.GetChildren());
		}
		
		if (GetController(source.GetSibling()) != null) {
			return GetController(source.GetSibling());
		}
		
		return null;
	}
	
	
	override void Run()
	{		
		m_Module = Workbench.GetModule("ResourceManager");
		Workbench.SearchResources("EditorObjectProperties.layout", ResourceSearchCb);		
	}
	
	[ButtonAttribute("Save", true)]
	void Save()
	{
		m_DialogResult = 1;
	}
	
	[ButtonAttribute("Cancel")]
	void Cancel() { }
	
	
	
}


