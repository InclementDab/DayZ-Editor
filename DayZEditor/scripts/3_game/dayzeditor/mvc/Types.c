
// 0: Property Name
// 1: Proprety Type
class PropertyHashMap: ref map<string, typename>
{
	static ref PropertyHashMap FromType(typename type)
	{
		ref PropertyHashMap hash_map = new PropertyHashMap();
		for (int i = 0; i < type.GetVariableCount(); i++) {
			hash_map.Insert(type.GetVariableName(i), type.GetVariableType(i));	
		}
		
		return hash_map;
	}
}


// 0: Property Name
// 1: View Binding
class ViewBindingHashMap: ref map<string, ref ViewBinding>
{
	void DebugPrint()
	{
		foreach (string name, ViewBinding view: this)
			EditorLog.Debug(string.Format("%1: %2", name, view));
		
	}
}

class PropertyInfo
{
	typename Type;
	string Name;
	
	void PropertyInfo(string name, typename type)
	{
		Name = name; Type = type;
	}
}





