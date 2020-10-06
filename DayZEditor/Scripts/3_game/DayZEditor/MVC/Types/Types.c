
class PropertyInfo
{
	string Name;
	typename Type;
	
	void PropertyInfo(string name, typename type)
	{
		Name = name;
		Type = type;
	}
	
	static PropertyInfo GetFromClass(Class context, string name)
	{
		if (!context)
			return null;

		PropertyTypeHashMap hash_map = PropertyTypeHashMap.FromType(context.Type());
		if (hash_map[name])
		{
			return new PropertyInfo(name, hash_map[name]);
		}
		
		return null;
	}
	
	static PropertyInfo GetFromType(typename parent_type, string name)
	{
		PropertyTypeHashMap hash_map = PropertyTypeHashMap.FromType(parent_type);
		if (hash_map[name])
		{
			return new PropertyInfo(name, hash_map[name]);
		}
		
		return null;
	}
}


// 0: Property Name
// 1: Property Type
class PropertyTypeHashMap: map<string, typename>
{
	static PropertyTypeHashMap FromType(typename type)
	{
		PropertyTypeHashMap hash_map = new PropertyTypeHashMap();
		for (int i = 0; i < type.GetVariableCount(); i++)
		{
			hash_map.Insert(type.GetVariableName(i), type.GetVariableType(i));	
		}
		
		return hash_map;
	}
	
	void RemoveType(typename removed_type)
	{
		PropertyTypeHashMap hash_map = FromType(removed_type);
		foreach (string name, typename type: hash_map)
			Remove(name);
	}
}


// 0: Source Widget
// 1: View Binding
typedef map<Widget, ViewBinding> ViewBindingHashMap;

// 0: View Binding
typedef set<ViewBinding> ViewBindingArray;

// 0: Property Name
// 1: View Binding Set
class DataBindingHashMap: map<string, autoptr ViewBindingArray>
{
	void DebugPrint()
	{
		foreach (string name, ViewBindingArray viewSet: this)
		{
			LayoutBindingManager.Log("[%1]:", name);
			foreach (ViewBinding view: viewSet)
			{
				LayoutBindingManager.Log("    %1", view.GetLayoutRoot().GetName());
			}
		}
	}
	
	void InsertView(ViewBinding view)
	{
		ViewBindingArray viewSet = Get(view.Binding_Name);
		if (!viewSet)
		{
			viewSet = new ViewBindingArray();
			viewSet.Insert(view);
			Insert(view.Binding_Name, viewSet);
			Insert(view.Selected_Item, viewSet);
		} else
		{
			viewSet.Insert(view);
		}		
	}
}

// 0: Relay_Command parameter
// 1: Command Value
typedef map<string, RelayCommand> RelayCommandHashMap

// 0: Source Type
// 1: Conversion Type
typedef map<typename, typename> TypenameHashMap;

// 0: Source Type
// 1: Conversion Type
class TypeConversionHashMap
{
	private autoptr map<typename, typename> value = new map<typename, typename>();
	
	
	typename Get(typename conversionType)
	{
		typename result = value.Get(conversionType);
		
		if (!result)
		{
			foreach (typename type, typename conversion: value)
			{
				if (conversionType.IsInherited(type))
				{
					return conversion;
				}
			}
		}
		
		return result;
	}
	
	void Remove(typename conversionType) {
		value.Remove(conversionType);
	}
	
	void Set(typename conversionType, typename conversionClass)
	{
		if (!conversionClass.IsInherited(TypeConversionTemplate))
		{
			LayoutBindingManager.Error(string.Format("TypeConverterHashMap: %1 must inherit from type TypeConversionTemplate", conversionClass.ToString()));
			return;
		}
		
		value.Set(conversionType, conversionClass);
	} 
	
	bool Insert(typename conversionType, typename conversionClass)
	{
		if (!conversionClass.IsInherited(TypeConversionTemplate))
		{
			LayoutBindingManager.Error(string.Format("TypeConverterHashMap: %1 must inherit from type TypeConversionTemplate", conversionClass.ToString()));
			return false;
		}
		
		return value.Insert(conversionType, conversionClass);
	}
};