static ref EditorPropertiesViewModel m_EditorPropertiesViewModel;
EditorPropertiesViewModel GetEditorPropertiesViewModel() { return m_EditorPropertiesViewModel; }


class EditorPropertiesViewModel: ViewModel
{
	
	TextWidgetData TitleText;
	ButtonWidgetData CollapseGeneral = true;
	
	vector ObjectPosition;
	
	void EditorPropertiesViewModel()
	{
		m_EditorPropertiesViewModel = this;
	}
	
	override void OnWidgetScriptInit(Widget w)
	{
		super.OnWidgetScriptInit(w);		
		TitleText = "Object Properties";
		ObjectPosition[0] = 1;
		
	}
}


class EditorObjectPropertiesDialog: EditorDialog
{
	
	protected EditorObject m_EditorObject;
	EditorObject GetEditorObject() { return m_EditorObject; }
	
	protected ButtonWidget m_ApplyButton;
	protected ButtonWidget m_CancelButton;
	protected EditorObject m_Object;
	
	protected Widget m_ViewModelFrame;
	protected EditorPropertiesViewModel m_ViewModel;
			
	void EditorObjectPropertiesDialog(EditorObject editor_object)
	{
		EditorLog.Trace("EditorObjectPropertiesDialog");
		m_EditorObject = editor_object;
		
		SetContent(GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/options/EditorObjectProperties.layout"));
		m_ViewModelFrame = m_ContentWrapper.FindAnyWidget("EditorPropertiesViewModel");
		m_ViewModelFrame.GetScript(m_ViewModel);
		
		//m_ViewModel.ObjectPosition = m_EditorObject.GetPosition();
		
		
		AddButton("Save", "SaveCallback");
		AddButton("Cancel", "CancelCallback");
		
	}
	
	void ~EditorObjectPropertiesDialog()
	{
		EditorLog.Trace("~EditorObjectPropertiesDialog");
	}
		
	
	void SaveCallback()
	{
		EditorLog.Trace("EditorObjectPropertiesDialog::SaveCallback");
	}
	
	void CancelCallback()
	{
		EditorLog.Trace("EditorObjectPropertiesDialog::CancelCallback");
	}
	

}
