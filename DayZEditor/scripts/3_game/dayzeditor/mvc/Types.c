


class DataBindingHashMap: ref map<string, ref DataBindingBase> 
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

class TPropertyInfo<Class T>
{
	T Type;
	string Name;
	
	
	void TPropertyInfo(string name)
	{
		Name = name;
	}
	
	T GetPropertyValue(Class instance, int index = 0)
	{
		T value;
		EnScript.GetClassVar(instance, Name, index, value); 
		return value;
	}
}

