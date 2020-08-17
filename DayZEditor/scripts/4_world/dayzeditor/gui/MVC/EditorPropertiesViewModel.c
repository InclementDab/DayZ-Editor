
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
		super.OnPropertyChanged(property_name);
		
		Widget root = GetWidgetRoot(m_LayoutRoot);
		switch (property_name) {
			
			
			case "CollapseGeneral": {		
				Widget w = root.FindAnyWidget("EditorDialogOptionContent");
				w.Show(CollapseGeneral);
				w.Update();
				root.FindAnyWidget("expand_image").Show(CollapseGeneral);
				root.FindAnyWidget("collapse_image").Show(!CollapseGeneral);
				break;
			}
			
		}
		
		
	}
	
}

