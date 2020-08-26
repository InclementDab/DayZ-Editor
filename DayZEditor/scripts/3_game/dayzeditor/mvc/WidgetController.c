
static WidgetController GetWidgetController(Widget widget)
{
	switch (widget.Type())
	{
		case ButtonWidget:
			return new ButtonWidgetController(widget);
		
		case CheckBoxWidget:
			return new CheckBoxWidgetController(widget);
		
		case EditBoxWidget:
			return new EditBoxWidgetController(widget):
		
		case SliderWidget:
			return new SliderWidgetController(widget);
		
		case TextWidget:
			return new TextWidgetController(widget);
		
		// Collection Controllers
		case MultilineEditBoxWidget:
			return new MultilineEditBoxWidgetController(widget);
	}
	
	
	return null;
}

class WidgetController
{
	protected Widget m_Widget;
	void WidgetController(Widget w) {
		m_Widget = w;
	}
	
	bool CanTwoWayBind() {
		return false;
	}
	
	// Base Controller Stuff
	void SetData(TypeConverter type_converter);	
	void GetData(out TypeConverter type_converter);
	
	// Collection Stuff
	void AddData(TypeConverter type_converter);
	void RemoveData(int index);
	void SetData(int index, TypeConverter type_converter);
	void ClearData();
}



class ButtonWidgetController: WidgetController
{
	override bool CanTwoWayBind() {
		return true;
	}
	
	override void SetData(TypeConverter type_converter) {
		ButtonWidget.Cast(m_Widget).SetState(type_converter.GetBool());
	}
	
	override void GetData(out TypeConverter type_converter) {
		type_converter.SetBool(ButtonWidget.Cast(m_Widget).GetState());
	}
}

class EditBoxWidgetController: WidgetController
{
	override bool CanTwoWayBind() {
		return true;
	}
	
	override void SetData(TypeConverter type_converter) {
		EditBoxWidget.Cast(m_Widget).SetText(type_converter.GetString());
	}
	
	override void GetData(out TypeConverter type_converter) {
		type_converter.SetString(EditBoxWidget.Cast(m_Widget).GetText());
	}
}

class CheckBoxWidgetController: WidgetController
{
	override bool CanTwoWayBind() {
		return true;
	}
	
	override void SetData(TypeConverter type_converter) {
		CheckBoxWidget.Cast(m_Widget).SetChecked(type_converter.GetBool());
	}
	
	override void GetData(out TypeConverter type_converter) {
		type_converter.SetBool(CheckBoxWidget.Cast(m_Widget).IsChecked());
	}
}

class SliderWidgetController: WidgetController
{
	override bool CanTwoWayBind() {
		return true;
	}
	
	override void SetData(TypeConverter type_converter) {
		SliderWidget.Cast(m_Widget).SetCurrent(type_converter.GetFloat());
	}
	
	override void GetData(out TypeConverter type_converter) {
		type_converter.SetFloat(SliderWidget.Cast(m_Widget).GetCurrent());
	}
}

class TextWidgetController: WidgetController
{
	override void SetData(TypeConverter type_converter) {
		TextWidget.Cast(m_Widget).SetText(type_converter.GetString());
	}
}


class MultilineEditBoxWidgetController: WidgetController
{
	
	override void AddData(TypeConverter type_converter) {
		
	}
	
	override void RemoveData(int index) {
		
	}
	
	override void SetData(TypeConverter type_converter) {
		TextWidget.Cast(m_Widget).SetText(type_converter.GetString());
	}
}

