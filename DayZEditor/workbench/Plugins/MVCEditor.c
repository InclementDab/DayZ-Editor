



[WorkbenchPluginAttribute("MVC Controller Manager", "Edit MVC Settings", "Alt+3", "DayZEditor/kekw.png", {"ResourceManager", "ScriptEditor"})]
class EditorViewOptions: WorkbenchPlugin
{
	private static ref array<ref ParamEnum> param_enums = {};
	[Attribute("0", "combobox", "ViewBinding: ", "", param_enums)]
	int CurrentViewEdit;

	protected ResourceBrowser m_Module;
	
	override void Run()
	{	
		Print("EditorViewOptions::Run");
		m_Module = Workbench.GetModule("ResourceManager");

									// replace once GetCurrentFile is fixed
		Workbench.SearchResources("EditorDialogOptionPropertiesPrefab.layout", OnSuccess);
		
	}
	
	void OnSuccess(string file)
	{
		Print("Resource Found! " + file);
		m_Module.SetOpenedResource(file);
		
		EditorViewOptionsCallback cb();
		
		
		g_Script.Call(cb, "ResourceSearch", param_enums);
		
		
		Workbench.ScriptDialog("View Options", "Edit View Binding Options", this);
	}
	
	[ButtonAttribute("Edit")]
	void Edit() 
	{
		string property_name = param_enums.Get(CurrentViewEdit).m_Key;
		Print(property_name);
		//EditorViewBase view = m_Controller.GetEditorView(property_name);
		//Print(view);
		//EditorViewData data = view.GetData();
		//Workbench.ScriptDialog("Edit View Data", "Edit View Binding Options", data);
	}
	
	[ButtonAttribute("Close")]
	void Close() { }
}