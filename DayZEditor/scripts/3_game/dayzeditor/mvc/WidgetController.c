
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
		
		case Widget:
		case SpacerWidget:
		case GridSpacerWidget:
		case WrapSpacerWidget: 
			return new SpacerWidgetController(widget);
		
		case XComboBoxWidget:
			return new XComboBoxWidgetController(widget);
		
		case ImageWidget:
			return new ImageWidgetController(widget);
		
		case TextListboxWidget:
			return new TextListboxController(widget);
		
	}
	
	
	return null;
}

class WidgetController
{	
	Widget GetWidget();
	
	bool CanTwoWayBind() {
		return false;
	}
	
	// Base Controller Stuff
	void SetData(TypeConverter type_converter);	
	void GetData(out TypeConverter type_converter);
	
	// Collection Stuff
	void SetSelection(TypeConverter type_converter);
	void GetSelection(out TypeConverter type_converter);
	void InsertData(int index, TypeConverter type_converter);
	void RemoveData(int index, TypeConverter type_converter);
	void ReplaceData(int index, TypeConverter type_converter);
	void MoveData(int start_index, int final_index);
	void ClearData();
}

class WidgetControllerTemplate<Class T>: WidgetController
{
	protected T m_Widget;
	
	
	override Widget GetWidget() {
		return m_Widget;
	}
	
	void WidgetControllerTemplate(Widget w)	{
		Class.CastTo(m_Widget, w);
	}
}



class ButtonWidgetController: WidgetControllerTemplate<ButtonWidget>
{
	override bool CanTwoWayBind() {
		return true;
	}
	
	override void SetSelection(TypeConverter type_converter) {
		m_Widget.SetText(type_converter.GetString());
	}
	
	
	override void SetData(TypeConverter type_converter) {
		m_Widget.SetState(type_converter.GetBool());
	}
	
	override void GetData(out TypeConverter type_converter) {
		type_converter.SetBool(m_Widget.GetState());
	}
}

class EditBoxWidgetController: WidgetControllerTemplate<EditBoxWidget>
{
	override bool CanTwoWayBind() {
		return true;
	}
	
	override void SetData(TypeConverter type_converter) {
		m_Widget.SetText(type_converter.GetString());
	}
	
	override void GetData(out TypeConverter type_converter) {
		type_converter.SetString(m_Widget.GetText());
	}
}

class CheckBoxWidgetController: WidgetControllerTemplate<CheckBoxWidget>
{
	override bool CanTwoWayBind() {
		return true;
	}
	
	override void SetData(TypeConverter type_converter) {
		m_Widget.SetChecked(type_converter.GetBool());
	}
	
	override void GetData(out TypeConverter type_converter) {
		type_converter.SetBool(m_Widget.IsChecked());
	}
}

class SliderWidgetController: WidgetControllerTemplate<SliderWidget>
{
	override bool CanTwoWayBind() {
		return true;
	}
	
	override void SetData(TypeConverter type_converter) {
		m_Widget.SetCurrent(type_converter.GetFloat());
	}
	
	override void GetData(out TypeConverter type_converter) {
		type_converter.SetFloat(m_Widget.GetCurrent());
	}
}

class TextWidgetController: WidgetControllerTemplate<TextWidget>
{
	override void SetData(TypeConverter type_converter) {
		m_Widget.SetText(type_converter.GetString());
	}
}


class MultilineEditBoxWidgetController: WidgetControllerTemplate<MultilineEditBoxWidget>
{
	override bool CanTwoWayBind() {
		return true;
	}
	
	override void SetData(TypeConverter type_converter) {
		m_Widget.SetText(type_converter.GetString());
	}
	
	override void GetData(out TypeConverter type_converter) {
		string out_text;
		m_Widget.GetText(out_text);
		type_converter.SetString(out_text);
	}
	
	override void InsertData(int index, TypeConverter type_converter) {
		m_Widget.SetLine(index, type_converter.GetString());
	}
	
	override void RemoveData(int index, TypeConverter type_converter) {
		m_Widget.SetLine(index, string.Empty);
	}
	
	override void ReplaceData(int index, TypeConverter type_converter) {
		m_Widget.SetLine(index, type_converter.GetString());
	}
		
	override void ClearData() {
		for (int i = 0; i < m_Widget.GetLinesCount(); i++)
			m_Widget.SetLine(i, string.Empty);		
	}
}

class SpacerWidgetController: WidgetControllerTemplate<SpacerWidget>
{
	/*
	override bool CanTwoWayBind() {
		return true;
	}
	
	override void SetData(TypeConverter type_converter) {
		m_Widget.SetText(type_converter.GetWidget());
	}
	
	override void GetData(out TypeConverter type_converter) {
		string out_text;
		m_Widget.GetText(out_text);
		type_converter.SetString(out_text);
	}*/
	
	override void InsertData(int index, TypeConverter type_converter) {
		m_Widget.AddChild(type_converter.GetWidget());
	}
	
	override void RemoveData(int index, TypeConverter type_converter) {
		m_Widget.RemoveChild(type_converter.GetWidget());
	}
	
	override void ReplaceData(int index, TypeConverter type_converter) {

		Widget child = m_Widget.GetChildren();
		while (child) {
			if (index == 0)
				break;
			
			child = child.GetSibling();
			index--;
		}
		
		m_Widget.AddChildAfter(type_converter.GetWidget(), child);
		m_Widget.RemoveChild(child);
	}
	
	

}

class XComboBoxWidgetController: WidgetControllerTemplate<XComboBoxWidget>
{
	override void SetSelection(TypeConverter type_converter) {
		m_Widget.SetCurrentItem(type_converter.GetInt());
	}
	
	override void GetSelection(out TypeConverter type_converter) {
		type_converter.SetInt(m_Widget.GetCurrentItem());
	}
	
	
	override void InsertData(int index, TypeConverter type_converter) {
		m_Widget.AddItem(type_converter.GetString());
	}
	
	override void RemoveData(int index, TypeConverter type_converter) {
		m_Widget.RemoveItem(index);
	}
}

class ImageWidgetController: WidgetControllerTemplate<ImageWidget>
{
	
	override void SetData(TypeConverter type_converter) {
		m_Widget.LoadImageFile(0, type_converter.GetString());
		m_Widget.SetImage(0);
	}
	
}


class TextListboxController: WidgetControllerTemplate<TextListboxWidget>
{
	override void InsertData(int index, TypeConverter type_converter) {
		m_Widget.AddItem(type_converter.GetString(), type_converter.GetParam(), 0, index);
	}
	
	override void RemoveData(int index, TypeConverter type_converter) {
		m_Widget.SetItem(index, string.Empty, null, 0);
	}
	
	override void ClearData() {
		m_Widget.ClearItems();
	}
}


