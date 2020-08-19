

typedef ref map<string, ref EditorViewBase> EditorViewHashMap;

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
		if (FileExist(file)) {
			DeleteFile(file);
		}
		
		FileSerializer serializer = new FileSerializer();
		serializer.Open(file, FileMode.WRITE);
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



class EditorViewOptionsCallback 
{
	static string SearchAction;
	static Class Instance;
	static EditorViewHashMap ResourceSearch() 
	{
		EditorViewHashMap result;
		g_Script.CallFunction(Instance, SearchAction, result, null);
		return result;
	}
}

