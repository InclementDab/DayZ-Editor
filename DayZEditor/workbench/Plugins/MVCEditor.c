


[WorkbenchPluginAttribute("MVC Controller Manager", "Edit MVC Settings", "Alt+3", "", {"ResourceManager", "ScriptEditor"})]
class EditorViewOptions: WorkbenchPlugin
{
	private static ref ParamEnumArray param_enums = {};
	
	[Attribute("0", "combobox", "ViewBinding: ", "", param_enums)]
	int CurrentViewEdit;

	protected ResourceBrowser m_Module;	
	protected EditorViewHashMap m_EditorViewHashMap;
	
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
		
		m_EditorViewHashMap = EditorViewOptionsCallback.ResourceSearch();
		for (int i = 0; i < m_EditorViewHashMap.Count(); i++) {
			string property_name = m_EditorViewHashMap.GetKey(i);
			param_enums.Insert(new ParamEnum(property_name, "" + i));
		}

		Workbench.ScriptDialog("View Options", "Edit View Binding Options", this);
	}
	
	[ButtonAttribute("Edit")]
	void Edit() 
	{
		EditorViewBase view = m_EditorViewHashMap.Get(m_EditorViewHashMap.GetKey(CurrentViewEdit));
		EditorViewData data = view.GetData();
		data.ShowDialog(view.GetLayoutRoot());
		
		
	}
	
	[ButtonAttribute("Close")]
	void Close() { }
}