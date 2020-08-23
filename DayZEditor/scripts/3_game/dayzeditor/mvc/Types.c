




class DataBindingHashMap: map<string, ref DataBindingBase> 
{
	
	void DebugPrint()
	{
		foreach (string name, DataBindingBase data: this) {
			EditorLog.Debug(string.Format("%1: %2", name, data));
		}
	}
}

class DataBindingBase 
{
	private void DataBindingBase() {}
	
	ref PropertyInfo Property;
	ref ViewBinding View;	
	
	static DataBindingBase Create(ViewBinding view)
	{
		return _Create(view.GetRoot().Type());
	}
	
	private static DataBindingBase _Create(typename widget_type)
	{
		switch (widget_type) {
			
			case Widget:
			case SpacerBaseWidget:
				return new DataBinding<Widget>;
			
			case ButtonWidget:
			case CheckBoxWidget:
				return new DataBinding<bool>;
			
			case SliderWidget:
			case ProgressBarWidget:
			case SimpleProgressBarWidget:
				return new DataBinding<float>;
			
			case TextWidget:
			case ImageWidget:
			case EditBoxWidget:
			case HtmlWidget:
			case VideoWidget:
				return new DataBinding<string>;
			
			case RichTextWidget:
			case MultilineTextWidget:
			case MultilineEditBoxWidget:
			case XComboBoxWidget:
				return new DataBinding<TStringArray>;
			
			case ItemPreviewWidget:
				return new DataBinding<EntityAI>;
			
			case PlayerPreviewWidget:
				return new DataBinding<DayZPlayer>;
			
			default: {
				Error(string.Format("Unknown Type Specified %1", widget_type));
			}
		}
		
		return new DataBindingBase;
	}
	
	static bool SupportsTwoWayBinding(typename type)
	{
		switch (type) {
			case ButtonWidget:
			case CheckBoxWidget:
			case SliderWidget:
			case EditBoxWidget:
			case MultilineEditBoxWidget:
			case RichTextWidget:
				return true;
	
		}
		
		return false;
	}
	
	typename GetType() 
	{ 
		typename type = Type();
		for (int i = 0; i < type.GetVariableCount(); i++) 
			if (type.GetVariableName(i) == "Data")
				return type.GetVariableType(i);
		
		EditorLog.Error("DataBindingBase::GetType: could not find the Data Type!");
		return typename;
	}

}

class DataBinding<Class T>: DataBindingBase
{
	T Data;
}


// 0: Property Name
// 1: Proprety Type
class PropertyHashMap: map<string, typename>
{
	static ref PropertyHashMap FromType(typename type)
	{
		ref PropertyHashMap hash_map = new PropertyHashMap();
		for (int i = 0; i < type.GetVariableCount(); i++) {
			hash_map.Insert(type.GetVariableName(i), type.GetVariableType(i));	
		}
		
		return hash_map;
	}
	
	PropertyInfo GetPropertyInfo(string key)
	{
		return new PropertyInfo(Get(key), key);
	}
}


class PropertyInfo
{
	typename Type;
	string Name;
	
	void PropertyInfo(typename type, string name)
	{
		Type = type; Name = name;
	}
}

