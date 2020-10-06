
class TypeConversionEquation: TypeConversionTemplate<StringEvaluater>
{
	override void SetString(string value) {
		m_Value = string.Empty;
		for (int i = 0; i < value.Length(); i++) {
			int ascii = value[i].Hash();
			if (ascii >= 40 && ascii <= 57 || ascii == 32 || ascii == 94)
				m_Value += value[i];
		}
	}
	
	override string GetString() {
		return m_Value;
	}	
}


class EditorWidget
{
	protected Widget m_LayoutRoot;
	Widget GetLayoutRoot() { 
		return m_LayoutRoot; 
	}
	
	void SetLayoutRoot(Widget layout_root) {
		m_LayoutRoot = layout_root;
	}
}


class TypeConversionEditorWidget: TypeConversionTemplate<EditorWidget>
{	
	override Widget GetWidget() {
		return m_Value.GetLayoutRoot();
	}
	
	override void SetWidget(Widget value) {
		m_Value.SetLayoutRoot(value);
	}	
}


class TypeConversionBrush: TypeConversionTemplate<EditorBrushData>
{	
	override void SetString(string value) {
		m_Value.Name = value;
	}
	
	override string GetString() {
		return m_Value.Name;
	}
}


class TypeConversionBrushObject: TypeConversionTemplate<EditorBrushObject>
{
	
	override void SetString(string value) {
		m_Value.Name = value;
	}
	
	override string GetString() {
		return m_Value.Name;
	}
	
	override Widget GetWidget() {
		return GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/options/EditorDialogOptionEditBrush.layout");
	}
}

class DropdownListPrefabItemConverter: TypeConversionTemplate<DropdownListPrefabItem>
{
	override string GetString() {
		if (m_Value)
			return m_Value.GetTemplateController().Text;
		
		return string.Empty;
	}
	
	override Widget GetWidget() {
		// Todo: why can this be null? not sure
		if (m_Value) {
			return m_Value.GetLayoutRoot();
		}
		
		return null;
	}
	
	override void SetWidget(Widget value) {
		m_Value.OnWidgetScriptInit(value);
	}
}


modded class LayoutBindingManager
{
	override void RegisterConversionTemplates(out TypeConversionHashMap type_conversions)
	{
		super.RegisterConversionTemplates(type_conversions);
		type_conversions.Insert(StringEvaluater, TypeConversionEquation);
		type_conversions.Insert(EditorWidget, TypeConversionEditorWidget);
		type_conversions.Insert(EditorBrushData, TypeConversionBrush);
		type_conversions.Insert(EditorBrushObject, TypeConversionBrushObject);
		type_conversions.Insert(DropdownListPrefabItem, DropdownListPrefabItemConverter);
	}
}



