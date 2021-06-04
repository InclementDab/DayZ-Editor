class EditorProfileSettings
{
	protected ref map<string, typename> m_SavedVariablesList = new map<string, typename>();
	
	void EditorProfileSettings()
	{
		for (int i = 0; i < Type().GetVariableCount(); i++) {
			string variable_name = Type().GetVariableName(i);
			m_SavedVariablesList[variable_name] = Type().GetVariableType(i);
		}
	}
	
	void Save()
	{
		for (int i = 0; i < Type().GetVariableCount(); i++) {
			string variable_name = Type().GetVariableName(i);
			typename variable_type = Type().GetVariableType(i);
			
			switch (variable_type) {				
				case bool: {
					bool _bool;
					EnScript.GetClassVar(this, variable_name, 0, _bool);
					GetGame().SetProfileString(variable_name, string.ToString(_bool));
					break;
				}
				
				case int: {
					int _int;
					EnScript.GetClassVar(this, variable_name, 0, _int);
					GetGame().SetProfileString(variable_name, string.ToString(_int));
					break;
				}
				
				case float: {
					float _float;
					EnScript.GetClassVar(this, variable_name, 0, _float);
					GetGame().SetProfileString(variable_name, string.ToString(_float));
					break;
				}
				
				case string: {
					string _string;
					EnScript.GetClassVar(this, variable_name, 0, _string);
					GetGame().SetProfileString(variable_name, _string);
					break;
				}
			}
		}
		
		GetGame().SaveProfile();
	}
	
	void Load()
	{
		for (int i = 0; i < Type().GetVariableCount(); i++) {
			string variable_name = Type().GetVariableName(i);
			typename variable_type = Type().GetVariableType(i);
			
			switch (variable_type) {				
				case bool: {
					bool _bool;
					EnScript.GetClassVar(this, variable_name, 0, _bool);
					EnScript.SetClassVar(this, variable_name, 0, GetProfileBool(variable_name, _bool));
					break;
				}
				
				case int: {					
					int _int;
					EnScript.GetClassVar(this, variable_name, 0, _int);
					EnScript.SetClassVar(this, variable_name, 0, GetProfileInt(variable_name, _int));
					break;
				}
				
				case float: {
					float _float;
					EnScript.GetClassVar(this, variable_name, 0, _float);
					EnScript.SetClassVar(this, variable_name, 0, GetProfileFloat(variable_name, _float));
					break;
				}
				
				case string: {
					string _string;
					EnScript.GetClassVar(this, variable_name, 0, _string);
					EnScript.SetClassVar(this, variable_name, 0, GetProfileString(variable_name, _string));
					break;
				}
			}
		}
	}
	
	static void SetProfileBool(string variable, bool value)
	{
		GetGame().SetProfileString(variable, value.ToString());	
	}
	
	static void SetProfileInt(string variable, int value)
	{
		GetGame().SetProfileString(variable, value.ToString());
	}
	
	static void SetProfileFloat(string variable, float value)
	{
		GetGame().SetProfileString(variable, value.ToString());
	}
	
	static void SetProfileString(string variable, string value)
	{
		GetGame().SetProfileString(variable, value);
	}
	
	static bool GetProfileBool(string variable, bool default = false)
	{
		string value;
		if (GetGame().GetProfileString(variable, value)) {
			return (value == "true" || value == "1");
		}
		
		return default;
	}
	
	static float GetProfileFloat(string variable, float default = 0)
	{
		string value;
		if (GetGame().GetProfileString(variable, value)) {
			return value.ToFloat();
		}
		
		return default;
	}
	
	static int GetProfileInt(string variable, int default = 0)
	{
		string value;
		if (GetGame().GetProfileString(variable, value)) {
			return value.ToInt();
		}
		
		return default;
	}
	
	static string GetProfileString(string variable, string default = "")
	{
		string value;
		if (GetGame().GetProfileString(variable, value)) {
			return value;
		}
		
		return default;
	}
}