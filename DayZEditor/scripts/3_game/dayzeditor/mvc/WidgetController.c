
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
	}
	
	
	return null;
}

class WidgetController
{
	protected Widget m_Widget;
	void WidgetController(Widget w) {
		m_Widget = w;
	}
	
	void SetData(TypeConverter type_converter);	
	void GetData(out TypeConverter type_converter);
}


class WidgetCollectionController: WidgetController
{
	void AddData(TypeConverter type_converter);
	void RemoveData(int index);
	void SetData(int index, TypeConverter type_converter);
	void ClearData();
}


class ButtonWidgetController: WidgetController
{
	override void SetData(TypeConverter type_converter) {
		ButtonWidget.Cast(m_Widget).SetState(type_converter.GetBool());
	}
	
	override void GetData(out TypeConverter type_converter) {
		type_converter.SetBool(ButtonWidget.Cast(m_Widget).GetState());
	}
}

class EditBoxWidgetController: WidgetController
{
	override void SetData(TypeConverter type_converter) {
		EditBoxWidget.Cast(m_Widget).SetText(type_converter.GetString());
	}
	
	override void GetData(out TypeConverter type_converter) {
		type_converter.SetString(EditBoxWidget.Cast(m_Widget).GetText());
	}
}

class CheckBoxWidgetController: WidgetController
{
	override void SetData(TypeConverter type_converter) {
		CheckBoxWidget.Cast(m_Widget).SetChecked(type_converter.GetBool());
	}
	
	override void GetData(out TypeConverter type_converter) {
		type_converter.SetBool(CheckBoxWidget.Cast(m_Widget).IsChecked());
	}
}

