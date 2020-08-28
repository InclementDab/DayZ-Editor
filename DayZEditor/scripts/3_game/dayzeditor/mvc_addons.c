
modded class MVC
{
	override void RegisterConversionTemplates(out TypeConverterHashMap type_conversions)
	{
		super.RegisterConversionTemplates(type_conversions);
		type_conversions.Insert(EquationEvaluater, TypeConversionEquation);
		type_conversions.Insert(EditorWidget, TypeConversionEditorWidget);
	}
}

typedef string EquationEvaluater;
class EquationEvaluater: string
{
	float Evaluate()
	{
		StringEvaluater evaluater();
		return evaluater.Parse(value);
	}
}

class TypeConversionEquation: TypeConversionTemplate<string>
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