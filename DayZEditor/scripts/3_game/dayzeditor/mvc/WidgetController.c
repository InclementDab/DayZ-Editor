
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
		
		case SpacerWidget:
		case GridSpacerWidget:
		case WrapSpacerWidget: 
			return new SpacerWidgetController(widget);
		
		case XComboBoxWidget:
			return new XComboBoxWidgetController(widget);
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
	void GetSelection(out TypeConverter type_converter);
	void InsertData(int index, TypeConverter type_converter);
	void RemoveData(int index, TypeConverter type_converter);
	void ReplaceData(int index, TypeConverter type_converter);
	void MoveData(int start_index, int final_index);
	void ClearData();
}

class GroupWidgetController: WidgetController
{
	private ref array<ref WidgetController> m_WidgetControllers = {};
	
	void AddController(WidgetController widget_controller) {
		m_WidgetControllers.Insert(widget_controller);
	}
	
	override void SetData(TypeConverter type_converter) {
		foreach (WidgetController widget_controller: m_WidgetControllers) {
			widget_controller.SetData(type_converter);
		}
	}
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
	override bool CanTwoWayBind() {
		return true;
	}
	
	override void SetData(TypeConverter type_converter) {
		MultilineEditBoxWidget.Cast(m_Widget).SetText(type_converter.GetString());
	}
	
	override void GetData(out TypeConverter type_converter) {
		string out_text;
		MultilineEditBoxWidget.Cast(m_Widget).GetText(out_text);
		type_converter.SetString(out_text);
	}
	
	override void InsertData(int index, TypeConverter type_converter) {
		MultilineEditBoxWidget.Cast(m_Widget).SetLine(index, type_converter.GetString());
	}
	
	override void RemoveData(int index, TypeConverter type_converter) {
		MultilineEditBoxWidget.Cast(m_Widget).SetLine(index, string.Empty);
	}
	
	override void ReplaceData(int index, TypeConverter type_converter) {
		MultilineEditBoxWidget.Cast(m_Widget).SetLine(index, type_converter.GetString());
	}
		
	override void ClearData() {
		MultilineEditBoxWidget w = MultilineEditBoxWidget.Cast(m_Widget);
		for (int i = 0; i < w.GetLinesCount(); i++)
			w.SetLine(i, string.Empty);		
	}
}

class SpacerWidgetController: WidgetController
{
	/*
	override bool CanTwoWayBind() {
		return true;
	}
	
	override void SetData(TypeConverter type_converter) {
		SpacerWidget.Cast(m_Widget).SetText(type_converter.GetWidget());
	}
	
	override void GetData(out TypeConverter type_converter) {
		string out_text;
		SpacerWidget.Cast(m_Widget).GetText(out_text);
		type_converter.SetString(out_text);
	}*/
	
	override void InsertData(int index, TypeConverter type_converter) {
		SpacerWidget.Cast(m_Widget).AddChild(type_converter.GetWidget());
	}
	
	override void RemoveData(int index, TypeConverter type_converter) {
		SpacerWidget.Cast(m_Widget).RemoveChild(type_converter.GetWidget());
	}
	
	override void ReplaceData(int index, TypeConverter type_converter) {
		SpacerWidget spacer_widget = SpacerWidget.Cast(m_Widget);

		Widget child = spacer_widget.GetChildren();
		while (child) {
			if (index == 0)
				break;
			
			child = child.GetSibling();
			index--;
		}
		
		spacer_widget.AddChildAfter(type_converter.GetWidget(), child);
		spacer_widget.RemoveChild(child);
	}
	
	
		
	override void ClearData() {
		MultilineEditBoxWidget w = MultilineEditBoxWidget.Cast(m_Widget);
		for (int i = 0; i < w.GetLinesCount(); i++)
			w.SetLine(i, string.Empty);		
	}
}

class XComboBoxWidgetController: WidgetController
{
	override void GetSelection(out TypeConverter type_converter) {
		type_converter.SetInt(XComboBoxWidget.Cast(m_Widget).GetCurrentItem());
	}
	
	override void InsertData(int index, TypeConverter type_converter) {
		XComboBoxWidget.Cast(m_Widget).AddItem(type_converter.GetString());
	}
	
	override void RemoveData(int index, TypeConverter type_converter) {
		XComboBoxWidget.Cast(m_Widget).RemoveItem(index);
	}
}




