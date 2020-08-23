
class WidgetData
{
	static WidgetData GetDataType(Widget widget)
	{
		return GetDataType(widget.Type());
	}
	
	static WidgetData GetDataType(typename widget_type)
	{
		switch (widget_type) {
			
			case Widget:
			case SpacerBaseWidget:
				return new WidgetDataConverter<Widget>;
			
			case ButtonWidget:
			case CheckBoxWidget:
				return new WidgetDataConverter<bool>;
			
			case SliderWidget:
			case ProgressBarWidget:
			case SimpleProgressBarWidget:
				return new WidgetDataConverter<float>;
			
			case TextWidget:
			case ImageWidget:
			case EditBoxWidget:
			case HtmlWidget:
			case VideoWidget:
				return new WidgetDataConverter<string>;
			
			case RichTextWidget:
			case MultilineTextWidget:
			case MultilineEditBoxWidget:
			case XComboBoxWidget:
				return new WidgetDataConverter<TStringArray>;
			
			case ItemPreviewWidget:
				return new WidgetDataConverter<EntityAI>;
			
			case PlayerPreviewWidget:
				return new WidgetDataConverter<DayZPlayer>;
			
			default: {
				Error(string.Format("Unknown Type Specified %1", widget_type));
			}			
		}
		
		return null;
	}
	
	void SetData(Class inst, string var_name, int var_index) {}
	
}


class WidgetDataConverter<Class T>: WidgetData
{
	private T m_Data;
	
	override void SetData(Class inst, string var_name, int var_index)
	{
		EnScript.GetClassVar(inst, var_name, var_index, m_Data);
	}
	
	T GetData()
	{
		return m_Data;
	}
	
}

