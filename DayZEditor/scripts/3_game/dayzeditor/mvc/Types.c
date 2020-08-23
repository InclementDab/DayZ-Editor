
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
typedef set<ref ViewBinding> ViewBindingSet;
class ViewBindingHashMap: ref map<string, ref ViewBindingSet>
{
	void DebugPrint()
	{
		foreach (string name, ViewBindingSet view_set: this) {
			EditorLog.Debug(string.Format("[%1]:", name)); 
			foreach (ViewBinding view: view_set) {
				EditorLog.Debug(string.Format("    %1", view.GetRoot().GetName()));
			}
		}		
	}
	
	void InsertView(string key, ViewBinding view)
	{
		ViewBindingSet view_set = Get(key);
		if (!view_set) {
			view_set = new ViewBindingSet();
			view_set.Insert(view);
			Insert(key, view_set);
		} else {
			view_set.Insert(view);
		}
		
		
		
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





