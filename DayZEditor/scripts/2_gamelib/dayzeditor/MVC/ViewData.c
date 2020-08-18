
#ifdef COMPONENT_SYSTEM
static ref EditorViewProjectData m_EditorViewProjectData;
EditorViewProjectData GetEditorViewProjectData() { return m_EditorViewProjectData; }

class EditorViewProjectData
{
	private ref map<Widget, ref EditorViewData> m_EditorViewData;
	
	void EditorViewProjectData(string file)
	{
		Print("EditorViewProjectData");
		m_EditorViewProjectData = this;
		m_EditorViewData = new map<Widget, ref EditorViewData>();
		LoadData(file);
	}
	
	void SaveData(string file)
	{
		Print("EditorViewProjectData::SaveData");
		FileSerializer serializer = new FileSerializer();
		serializer.Open(file, FileMode.APPEND);
		serializer.Write(this);
		serializer.Close();
	}
	
	void LoadData(string file) 
	{
		Print("EditorViewProjectData::LoadData");
		FileSerializer serializer = new FileSerializer();
		serializer.Open(file, FileMode.READ);
		serializer.Read(this);
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
}



class EditorViewData 
{	
	string ViewModelWidget;
	string VariableName;
	int VariableIndex;
	string ControlName;
	
	void EditorViewData(string view_model_widget, string variable_name, int variable_index, string control_name)
	{
		ViewModelWidget = view_model_widget; VariableName = variable_name; VariableIndex = variable_index; ControlName = control_name;
	}
}


class MVCPlugin: WorkbenchPlugin
{
	string FileName;
	
	int ShowDialog(ref EditorViewData view_data) 
	{
		return -2;
	}
}

static MVCPlugin m_MVCPlugin;


#endif