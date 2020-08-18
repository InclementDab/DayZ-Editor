


static ref map<string, ControllerBase> ControllerHashMap;


class ControllerBase: Managed
{
	
	// View List
	protected ref map<string, ref EditorViewBase> m_EditorViewList;
	
	
	void ControllerBase()
	{
		m_EditorViewList = new map<string, ref EditorViewBase>();
	}
	
	ref EditorViewData GetEditorViewData(string property_name)
	{
		return m_EditorViewList.Get(property_name).GetData();
	}
	
	void SetEditorViewData(ref EditorViewData data, string property_name)
	{
		m_EditorViewList.Get(property_name).SetData(data);
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
	
	static void SaveData(ref EditorViewProjectData data, string file = "")
	{
		Print("EditorViewProjectData::SaveData");
		if (file == string.Empty) file = GetFilePath();
		FileSerializer serializer = new FileSerializer();
		serializer.Open(file, FileMode.APPEND);
		serializer.Write(data);
		serializer.Close();
	}
	
	static void LoadData(out ref EditorViewProjectData data, string file = "") 
	{
		Print("EditorViewProjectData::LoadData");
		if (file == string.Empty) file = GetFilePath();
		if (!FileExist(file)) return;
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
	
	
	static string GetFilePath()
	{
		ScriptEditor browser = Workbench.GetModule("ScriptEditor");
		if (browser) {
			string file;
			browser.GetCurrentFile(file);
			array<string> file_dir = {};
			file.Split("/", file_dir);
			Workbench.GetAbsolutePath(file_dir.Get(0), file);
			file += "/layoutdata.bin";
		}
		
		return file;
	}
}



class EditorViewData 
{	
	[Attribute("", "editbox", "ViewModel Widget Name")]
	string ControllerWidget;

	[Attribute("", "editbox")]
	string DataBindingName;
	
	[Attribute("", "spinbox")]
	int DataBindingIndex;
	
	[Attribute("", "editbox")]
	string ProxyName;
	
	void EditorViewData(string view_model_widget, string variable_name, int variable_index = 0, string control_name = "")
	{
		ControllerWidget = view_model_widget; DataBindingName = variable_name; DataBindingIndex = variable_index; ProxyName = control_name;
	}
	
	
}


class MVCPlugin: WorkbenchPlugin
{
	protected EditorViewData m_EditorViewData;
	void MVCPlugin(EditorViewData view_data)
	{
		m_EditorViewData = view_data;
	}
	
	protected int m_DialogResult = -1;
	int GetDialogResult() { return m_DialogResult; }
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


