static WidgetController GetPrefabController(EditorPrefab prefab)
{
	
	switch (prefab.Type()) {
		
		case EditorPrefabGroup:
			return new EditorGroupPrefabController(prefab);
		
		case EditorPrefabEditText:
			return new EditorEditTextPrefabController(prefab);
		
		case EditorPrefabPosition:
			return new EditorPositionPrefabController(prefab);
		
		
	}
	
	return null;
}

class EditorGroupPrefabController: WidgetControllerTemplate<EditorPrefabGroup>
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


class EditorEditTextPrefabController: WidgetControllerTemplate<EditorPrefabEditText>
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

class EditorPositionPrefabController: WidgetControllerTemplate<EditorPrefabPosition>
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




class EditorPrefabViewBinding: ViewBinding
{
	
	protected ref EditorPrefab m_EditorPrefab;
	
	void SetPrefab(EditorPrefab prefab) {
		m_EditorPrefab = prefab;
		SetBindingName(m_EditorPrefab.GetBindingName());
		
		m_WidgetController = GetPrefabController(m_EditorPrefab);
	}
}



