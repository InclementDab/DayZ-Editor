
class EditorPrefabGroupController: WidgetControllerTemplate<EditorPrefabGroup>
{
	/*
	override bool CanTwoWayBind() {
		return true;
	}
	
	override void SetData(TypeConverter type_converter) {
		m_Widget.SetState(type_converter.GetBool());
	}
	
	override void GetData(out TypeConverter type_converter) {
		type_converter.SetBool(m_Widget.GetState());
	}*/
}


class EditorPrefabEditTextController: WidgetControllerTemplate<EditorPrefabEditText>
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

class EditorPrefabPositionController: WidgetControllerTemplate<EditorPrefabPosition>
{
	override bool CanTwoWayBind() {
		return true;
	}
	
	override void SetData(TypeConverter type_converter) {
		m_Widget.SetVector(type_converter.GetVector());
	}
	
	override void GetData(out TypeConverter type_converter) {
		type_converter.SetVector(m_Widget.GetVector());
	}
}

class EditorPrefabSliderController: WidgetControllerTemplate<EditorPrefabSlider>
{
	override bool CanTwoWayBind() {
		return true;
	}
	
	override void SetData(TypeConverter type_converter) {
		m_Widget.SetFloat(type_converter.GetFloat());
	}
	
	override void GetData(out TypeConverter type_converter) {
		type_converter.SetFloat(m_Widget.GetFloat());
	}
}

class EditorPrefabButtonController: WidgetControllerTemplate<EditorPrefabButton>
{
	override bool CanTwoWayBind() {
		return true;
	}
	
	override void SetData(TypeConverter type_converter) {
		m_Widget.SetBool(type_converter.GetBool());
	}
	
	override void GetData(out TypeConverter type_converter) {
		type_converter.SetBool(m_Widget.GetBool());
	}
}

class EditorPrefabDropdownController: WidgetControllerTemplate<EditorPrefabDropdown>
{
	
	override void InsertData(int index, TypeConverter type_converter) {
		m_Widget.AddChild(type_converter.GetString());
	}
	
	override void RemoveData(int index, TypeConverter type_converter) {
		m_Widget.RemoveChild(type_converter.GetString());
	}
}

