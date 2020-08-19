




class EditorViewCreateDialog
{
	
	[Attribute("0", "editbox", "Widget Name: ", "")]
	string Name;
	
	void EditorViewCreateDialog(WidgetSource widget)
	{

	}	
	
	[ButtonAttribute("Create")]
	void Create() 
	{
		
	}
		
	[ButtonAttribute("Cancel", true)]
	void Cancel() { }
}

class EditorViewCreatePrompt
{
	
	private WidgetSource m_WidgetSource;
	void EditorViewCreatePrompt(WidgetSource widget)
	{
		m_WidgetSource = widget;
	}
	
	[ButtonAttribute("Yes")]
	void Yes() 
	{
		EditorViewCreateDialog create_dialog(m_WidgetSource);
		Workbench.ScriptDialog("Create Controller", "", create_dialog);
	}
		
	[ButtonAttribute("Fack off cunt", true)]
	void Close() { }
}



[WorkbenchPluginAttribute("MVC Controller Manager", "Edit MVC Settings", "Alt+3", "", {"ResourceManager", "ScriptEditor"})]
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
		
		Print(m_Module.GetNumContainers());
		Print(widget.GetName());
		
		if (ControllerBaseHashMap != null) {
			m_Controller = ControllerBaseHashMap.Get(ControllerBase.GetFromWidget(widget));
		} else {
			EditorViewCreatePrompt create_dialog(widget);
			Workbench.ScriptDialog("oof!", "Controller not found! Create New?", create_dialog);
			return;
		}
		
		if (m_Controller == null) {
			Workbench.ScriptDialog("oof!", "Controller not found! Create New?", create_dialog);
			return;
		}
		
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
		Print("EditorViewOptions::Run");
		m_Module = Workbench.GetModule("ResourceManager");
									// replace once GetCurrentFile is fixed
		Workbench.SearchResources("EditorDialogOptionPropertiesPrefab.layout", ResourceSearchCb);		
	}
		

	
	[ButtonAttribute("Edit")]
	void Edit() 
	{
		string property_name = param_enums.Get(CurrentViewEdit).m_Key;
		Print(property_name);
		EditorViewBase view = m_Controller.GetEditorView(property_name);
		Print(view);
		EditorViewData data = view.GetData();
		Workbench.ScriptDialog("Edit View Data", "Edit View Binding Options", data);
	}
	
	[ButtonAttribute("Close")]
	void Close() { }
	
}


