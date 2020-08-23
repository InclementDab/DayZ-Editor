


class PropertyInfo
{
	typename Type;
	string Name;
	
	void PropertyInfo(typename type, string name)
	{
		Type = type; Name = name;
	}
}

class DataBindingHashMap: map<string, ref DataBindingBase> 
{
	
	void DebugPrint()
	{
		foreach (string name, DataBindingBase data: this) {
			EditorLog.Debug(string.Format("%1: %2", name, data));
		}
	}
}