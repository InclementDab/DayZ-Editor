
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
typedef ref set<ref ViewBinding> ViewBindingSet;
class ViewBindingHashMap: ref map<Widget, ref ViewBindingSet>
{
	void DebugPrint()
	{
		foreach (Widget w, ViewBindingSet view_set: this) {
			EditorLog.Debug(string.Format("[%1]:", w.GetName())); 
			foreach (ViewBinding view: view_set) {
				EditorLog.Debug(string.Format("    %1", view.GetRoot().GetName()));
			}
		}		
	}
	
	void InsertView(ViewBinding view)
	{
		Widget key = view.GetRoot();
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


// 0: Source Widget
// 1: Button
// 2: Button State
typedef Param3<ButtonWidget, int, bool> ButtonCommandArgs;

// 0: Source Widget
// 1: CheckBox State
typedef Param2<CheckBoxWidget, bool> CheckBoxCommandArgs;


// 0: Source Widget
// 1: Selected Item
typedef Param2<XComboBoxWidget, int> XComboBoxCommandArgs;


typedef Param2<WrapSpacerWidget, Widget> WrapSpacerCommandArgs;



enum WidgetEventType {
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


