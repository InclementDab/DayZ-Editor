
class EditorPropertiesViewModel: ViewModel
{
	
	TextWidgetData TitleText;
	
	ButtonWidgetData CollapseGeneral = true;
	
	override void OnWidgetScriptInit(Widget w)
	{
		super.OnWidgetScriptInit(w);
		
		TitleText = "Object Properties";
	
	}
	
	override void OnPropertyChanged(string property_name)
	{
		
		
		switch (property_name) {
			
			
			case "CollapseGeneral": {		
				Widget w = m_LayoutRoot.FindAnyWidget("EditorDialogOptionContent");
				w.Show(CollapseGeneral);
				w.Update();
				m_LayoutRoot.FindAnyWidget("expand_image").Show(CollapseGeneral);
				m_LayoutRoot.FindAnyWidget("collapse_image").Show(!CollapseGeneral);
				break;
			}
			
		}
		
		super.OnPropertyChanged(property_name);
	}
	
}

