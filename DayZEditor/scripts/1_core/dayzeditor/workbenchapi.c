

class EditorViewData 
{	
	private static ref array<ref ParamEnum> m_ViewModels = {};
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
	
	int ShowDialog(ref EditorViewData view_data) 
	{
		return -2;
	}
}

static MVCPlugin m_MVCPlugin;

