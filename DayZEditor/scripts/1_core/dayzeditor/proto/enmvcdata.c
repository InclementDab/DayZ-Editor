

static ref EditorViewProjectData m_EditorViewProjectData;
EditorViewProjectData GetEditorViewProjectData() { return m_EditorViewProjectData; }

typedef ref array<ref ControllerData> TControllerDataArray;
typedef ref array<ref DataBindingData> TDataBindingDataArray;


// Serialized Data for a full project
class EditorViewProjectData
{
	// Serialized Stuff
	ref TControllerDataArray ControllerDataArray = {};

	private void EditorViewProjectData() 
	{
		Print("EditorViewProjectData");
	}


		
	static void Save(string file, notnull ref EditorViewProjectData data)
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
	
	static ref EditorViewProjectData Load(string file) 
	{
		ref EditorViewProjectData data = new EditorViewProjectData();

		Print("EditorViewProjectData::LoadData");

		FileSerializer serializer = new FileSerializer();
		serializer.Open(file, FileMode.READ);
		serializer.Read(data);
		serializer.Close();
		return data;
	}	
}


// Serializable Data for a Controller
class ControllerData
{
	// Serialized Data
	string ControllerWidgetName;
	ref TDataBindingDataArray DataBindingArray = {};
	
	[NonSerialized()]
	private static ref ParamEnumArray param_enums = {};
	

	[Attribute("0", "combobox", "ViewBinding: ", "", param_enums)]
	int CurrentDataBinding;

	
	[ButtonAttribute("Close")]
	void Close() 
	{ 
		
	}
}




// Serializable Data for a DataBinding
class DataBindingData
{
	
	string DataBindingWidgetName;
	
	[Attribute("", "editbox")]
	string DataBindingName;
	
	[Attribute("", "spinbox")]
	int DataBindingIndex;
	
	[Attribute("", "editbox")]
	string ProxyName;
	
	void DataBindingData(string variable_name, int variable_index = 0, string control_name = "")
	{
		DataBindingName = variable_name; DataBindingIndex = variable_index; ProxyName = control_name;
	}
	
	string _DataBindingName;
	int _DataBindingIndex;
	string _ProxyName;
	
	private Widget m_Widget;
	void ShowDialog(Widget w)
	{
		m_Widget = w;
		_DataBindingName = DataBindingName; _DataBindingIndex = DataBindingIndex; _ProxyName = ProxyName;
		Workbench.ScriptDialog("Edit View Data", "Edit View Binding Options", this);
	}
	
	[ButtonAttribute("Save", true)]
	void Save()
	{
		//m_EditorViewProjectData.InsertViewData(m_Widget, this);
		//EditorViewProjectData.SaveData(m_EditorViewProjectData, "P:\\DayZEditor\\layoutdata.bin");
	}
	
	[ButtonAttribute("Cancel")]
	void Cancel() 
	{ 
		DataBindingName = _DataBindingName; DataBindingIndex = _DataBindingIndex; ProxyName = _ProxyName;
	}
}



class EditorViewOptionsCallback 
{
	static string SearchAction;
	static Class Instance;
	static TDataBindingDataArray ResourceSearch() 
	{
		TDataBindingDataArray result;
		g_Script.CallFunction(Instance, SearchAction, result, null);
		return result;
	}
}


