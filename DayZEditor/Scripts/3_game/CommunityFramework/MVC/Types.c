

// 0: Property Name
// 1: Property Type
class PropertyTypeHashMap: ref map<string, typename>
{
	static ref PropertyTypeHashMap FromType(typename type)
	{
		ref PropertyTypeHashMap hash_map = new PropertyTypeHashMap();
		for (int i = 0; i < type.GetVariableCount(); i++) {
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
typedef ref map<Widget, ViewBinding> ViewBindingHashMap;


typedef ref set<ViewBinding> ViewBindingArray;

// 0: Property Name
// 1: View Binding Set
class DataBindingHashMap: ref map<string, ref ViewBindingArray>
{
	void DebugPrint()
	{
		foreach (string name, ViewBindingArray view_set: this) {
			MVC.Log("[%1]:", name);
			foreach (ViewBinding view: view_set) {
				MVC.Log("    %1", view.GetLayoutRoot().GetName());
			}
		}
	}
	
	void InsertView(ViewBinding view)
	{
		ViewBindingArray view_set = Get(view.Binding_Name);
		if (!view_set) {
			view_set = new ViewBindingArray();
			view_set.Insert(view);
			Insert(view.Binding_Name, view_set);
			Insert(view.Selected_Item, view_set);
		} else {
			view_set.Insert(view);
		}		
	}
}


class RelayCommandHashMap: ref map<string, RelayCommand>
{
	
}

typedef map<typename, typename> TypenameHashMap;
class TypeConversionHashMap
{
	private ref map<typename, typename> value = new map<typename, typename>();
	
	
	typename Get(typename conversion_type) {
		typename result = value.Get(conversion_type);
		
		if (!result) {
			foreach (typename type, typename conversion: value) {
				if (conversion_type.IsInherited(type)) {
					return conversion;
				}
			}
		}
		
		return result;
	}
	
	void Remove(typename conversion_type) {
		value.Remove(conversion_type);
	}
	
	void Set(typename conversion_type, typename conversion_class)
	{
		//GetLogger().Log("TypeConverterHashMap::Set", "JM_CF_MVC");
		if (!conversion_class.IsInherited(TypeConversionTemplate)) {
			MVC.Error(string.Format("TypeConverterHashMap: %1 must inherit from type TypeConversionTemplate", conversion_class.ToString()));
			return;
		}
		
		value.Set(conversion_type, conversion_class);
	} 
	
	bool Insert(typename conversion_type, typename conversion_class)
	{
		//GetLogger().Log("TypeConverterHashMap::Insert", "JM_CF_MVC");
		if (!conversion_class.IsInherited(TypeConversionTemplate)) {
			MVC.Error(string.Format("TypeConverterHashMap: %1 must inherit from type TypeConversionTemplate", conversion_class.ToString()));
			return false;
		}
		
		return value.Insert(conversion_type, conversion_class);
	}
}


enum WidgetEventType { // unused
	WidgetEventChange,
	WidgetEventChildAdd,
	WidgetEventChildRemove,
	WidgetEventClick,
	WidgetEventController,
	WidgetEventDrag,
	WidgetEventDragging,
	WidgetEventDraggingOver,
	WidgetEventDrop,
	WidgetEventDropReceived,
	WidgetEventFocus,
	WidgetEventFocusLost,
	WidgetEventItemSelected,
	WidgetEventItemSelectedTypeID,
	WidgetEventKeyDown,
	WidgetEventKeyPress,
	WidgetEventModalResult,
	WidgetEventMouseButtonDown,
	WidgetEventMouseButtonUp,
	WidgetEventMouseDoubleClick,
	WidgetEventMouseEnter,
	WidgetEventMouseLeave,
	WidgetEventMouseMove,
	WidgetEventMouseWheel,
	WidgetEventResize,
	WidgetEventSliderChange,
	WidgetEventUpdate	
};


