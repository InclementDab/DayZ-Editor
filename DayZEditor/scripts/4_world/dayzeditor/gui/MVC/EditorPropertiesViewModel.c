
class EditorPropertiesViewModel: ViewModel
{
	
	TextWidgetData TitleText;
	
	ButtonWidgetData CollapseGeneral = true;
	
	override void OnWidgetScriptInit(Widget w)
	{
		super.OnWidgetScriptInit(w);		
		TitleText = "Object Properties";
	}
	
	
}

