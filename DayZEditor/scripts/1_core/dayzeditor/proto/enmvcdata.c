


static ref map<string, ControllerBase> ControllerBaseHashMap;
class ControllerBase: Managed
{
	
	// View List
	protected ref map<string, ref EditorViewBase> m_EditorViewList;
	
	
	void ControllerBase()
	{
		m_EditorViewList = new map<string, ref EditorViewBase>();
	}
	
	EditorViewBase GetEditorView(string property_name)
	{
		return m_EditorViewList.Get(property_name);
	}
	
	static string GetFromWidget(WidgetSource source)
	{
		if (!source) return string.Empty;
		
		string script;
		source.Get(source.VarIndex("scriptclass"), script);
		typename type = script.ToType();
		if (type.IsInherited(ControllerBase)) {
			Print("Controller Found");
			return source.GetName();
		}
		
		if (GetFromWidget(source.GetChildren()) != string.Empty) {
			return GetFromWidget(source.GetChildren());
		}
		
		if (GetFromWidget(source.GetSibling()) != string.Empty) {
			return GetFromWidget(source.GetSibling());
		}
		
		return string.Empty;
	}
	
	static WidgetSource GetWidgetSource()
	{
		ResourceBrowser m_Module = Workbench.GetModule("ResourceManager");
		return m_Module.GetContainer();
	}
}



static ref EditorViewProjectData m_EditorViewProjectData;
EditorViewProjectData GetEditorViewProjectData() { return m_EditorViewProjectData; }

class EditorViewProjectData
{
	private ref map<Widget, ref EditorViewData> m_EditorViewData;
	
	void EditorViewProjectData()
	{
		Print("EditorViewProjectData");
		m_EditorViewProjectData = this;
		m_EditorViewData = new map<Widget, ref EditorViewData>();
	}
	
	static void SaveData(ref EditorViewProjectData data, string file)
	{
		Print("EditorViewProjectData::SaveData");
		FileSerializer serializer = new FileSerializer();
		serializer.Open(file, FileMode.APPEND);
		serializer.Write(data);
		serializer.Close();
	}
	
	static void LoadData(out ref EditorViewProjectData data, string file) 
	{
		Print("EditorViewProjectData::LoadData");
		if (!FileExist(file)) {
			Error("File Not Found!");
			return;
		}
		FileSerializer serializer = new FileSerializer();
		serializer.Open(file, FileMode.READ);
		serializer.Read(data);
		serializer.Close();
	}
	
	void InsertViewData(Widget w, EditorViewData data)
	{
		m_EditorViewData.Insert(w, data);
	}
	
	void RemoveViewData(Widget w)
	{
		m_EditorViewData.Remove(w);
	}
	
	EditorViewData GetData(Widget w)
	{
		return m_EditorViewData.Get(w);
	}
	
}



class EditorViewData 
{	
	
	[Attribute("", "editbox")]
	string DataBindingName;
	
	[Attribute("", "spinbox")]
	int DataBindingIndex;
	
	[Attribute("", "editbox")]
	string ProxyName;
	
	void EditorViewData(string variable_name, int variable_index = 0, string control_name = "")
	{
		DataBindingName = variable_name; DataBindingIndex = variable_index; ProxyName = control_name;
	}
	

	private string DialogCallback;
	private Class DialogCallbackInstance;
	
	void SetDialogCallback(Class inst, string cb)
	{
		DialogCallback = cb; DialogCallbackInstance = inst;
	}
	
	[ButtonAttribute("Save", true)]
	void Save()
	{
		g_Script.Call(DialogCallbackInstance, DialogCallback, 1);
	}
	
	[ButtonAttribute("Cancel")]
	void Cancel() { }
}




class EditorViewBase: ScriptedWidgetEventHandler
{
	protected string ControllerWidget;
	string GetViewModelWidgetName() { return ControllerWidget; }
	
	protected bool Edit_Options;
	
	// Optional
	// if blank, will use name of Widget
	protected string DataBindingName;
	string GetVariableName() { return DataBindingName; } 
	
	// Index of array if using arrays
	protected int DataBindingIndex;
	int GetVariableIndex() { return DataBindingIndex; }
	
	// if blank, will use this widget
	protected string ProxyName;
	string GetControlName() { return ProxyName; } 

	protected ref EditorViewData m_EditorViewData;
	
	ref EditorViewData GetData()
	{
		return m_EditorViewData;
	}
	
	void SetData(ref EditorViewData data)
	{
		m_EditorViewData = data;
	}

	void OnPropertyChanged() {}
	void OnCollectionChanged(CollectionChangedEventArgs args) {}
}


