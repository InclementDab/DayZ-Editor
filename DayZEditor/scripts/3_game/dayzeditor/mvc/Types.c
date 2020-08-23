


class DataBindingHashMap: ref map<string, ref DataBinding<Class>> 
{
	void DebugPrint()
	{
		foreach (string name, DataBinding<Class> data: this) {
			EditorLog.Debug(string.Format("%1: %2", name, data));
		}
	}
}

class ViewBindingHashMap: ref map<string, ref ViewBinding>
{
	void DebugPrint()
	{
		foreach (string name, ViewBinding view: this)
			EditorLog.Debug(string.Format("%1: %2", name, view));
		
	}
}

class DataBindingBase 
{
	private void DataBindingBase() {}
	
	ref ViewBinding View;
	
	static DataBindingBase Create(ViewBinding view)
	{ 
		DataBindingBase base = _Create(view.GetRoot().Type(), view.GetBindingName());
		base.View = view;
		return base;
	}
	
	private static DataBindingBase _Create(typename widget_type, string property_name)
	{
		switch (widget_type) {
			
			case Widget:
			case SpacerBaseWidget:
				return new DataBinding<Widget>(property_name);
			
			case ButtonWidget:
			case CheckBoxWidget:
				return new DataBinding<bool>(property_name);
			
			case SliderWidget:
			case ProgressBarWidget:
			case SimpleProgressBarWidget:
				return new DataBinding<float>(property_name);
			
			case TextWidget:
			case ImageWidget:
			case EditBoxWidget:
			case HtmlWidget:
			case VideoWidget:
				return new DataBinding<string>(property_name);
			
			case RichTextWidget:
			case MultilineTextWidget:
			case MultilineEditBoxWidget:
			case XComboBoxWidget:
				return new DataBinding<TStringArray>(property_name);
			
			case ItemPreviewWidget:
				return new DataBinding<EntityAI>(property_name);
			
			case PlayerPreviewWidget:
				return new DataBinding<DayZPlayer>(property_name);
			
			default: {
				Error(string.Format("Unknown Type Specified %1", widget_type));
			}
		}
		
		return new DataBindingBase;
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
	
	bool CanConvertFrom(typename from_type)
	{
		typename type = GetType();
		
		// No conversion required
		if (type == from_type) return true;
		
		switch (type) {
			
			case int: {
				switch (from_type) {
					case float:
					case string: {
						return true;
					}
				}
				break;
			}
			
			case float: {
				switch (from_type) {
					case int:
					case string: {
						return true;
					}
				}
				
				break;
			}
			
			case string: {
				switch (from_type) {
					case bool:				
					case int:
					case float:
					case TStringArray:
						return true;
				}
				
				break;
			}
		}
		
		EditorLog.Debug(string.Format("Cannot convert from type %1 to %2", from_type, type));
		return false;
	}
}


class TypeConverter<Class T>
{
	
	private static void HandleError(typename type)
	{
		Controller.ErrorDialog(string.Format("Invalid Conversion Type! %1 to %2", type.ToString(), T.ToString().ToType()));
	}
}


class DataBinding<Class T>: DataBindingBase
{
	T Data;
	ref PropertyInfo Property;
	
	void DataBinding(string property_name)
	{
		Property = new PropertyInfo(property_name, T);
	}	
	
	static void UnsupportedTypeError(typename type)
	{
		Controller.ErrorDialog(string.Format("DataBinding: Unsupported Type %1", type));
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


/*

	T ConvertFrom(string data)
	{

		
		return Data;
	}
	
	T ConvertFrom(Widget data)
	{

		
		return Data;
	}	
	
	T ConvertFrom(TStringArray data)
	{
		
		
		return Data;
	}	
	
	T ConvertFrom(EntityAI data)
	{

		
		return Data;
	}	
	
	T ConvertFrom(DayZPlayer data)
	{

		
		return Data;
	}*/
