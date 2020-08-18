static ref EditorPropertiesViewModel m_EditorPropertiesViewModel;
EditorPropertiesViewModel GetEditorPropertiesViewModel() { return m_EditorPropertiesViewModel; }


class EditorPropertiesViewModel: ViewModel
{
	private EditorObject m_EditorObject;
	
	TextWidgetData TitleText;
	ButtonWidgetData CollapseGeneral = true;
	
	EditBoxWidgetDataF pos_x, pos_y, pos_z;
	
	void EditorPropertiesViewModel()
	{
		m_EditorPropertiesViewModel = this;
	}
	
	override void OnWidgetScriptInit(Widget w)
	{
		super.OnWidgetScriptInit(w);		
		TitleText = "Object Properties";
	}
	
	void SetEditorObject(EditorObject editor_object)
	{
		m_EditorObject = editor_object;
		SetPosition(m_EditorObject.GetPosition());
		OnPropertyChanged("pos_x");
		OnPropertyChanged("pos_y");
		OnPropertyChanged("pos_z");
	}
	
	void SetPosition(vector position)
	{
		
		pos_x = position[0].ToString();
		pos_y = position[1].ToString();
		pos_z = position[2].ToString();
	}
	
	vector GetPosition()
	{
		StringEvaluater eval();		
		return Vector(eval.Parse(pos_x), eval.Parse(pos_y), eval.Parse(pos_z));
	}
	
	override void OnPropertyChanged(string property_name)
	{
		m_EditorObject.SetPosition(GetPosition());
		super.OnPropertyChanged(property_name);
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
	protected ref EditorPropertiesViewModel m_ViewModel;
	
	private vector m_StartPosition;
	
	void EditorObjectPropertiesDialog(EditorObject editor_object)
	{
		EditorLog.Trace("EditorObjectPropertiesDialog");
		m_EditorObject = editor_object;
		
		SetContent(GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/options/EditorObjectProperties.layout"));
		m_ViewModelFrame = m_ContentWrapper.FindAnyWidget("EditorPropertiesViewModel");
		m_ViewModelFrame.GetScript(m_ViewModel);
		m_ViewModel.SetEditorObject(m_EditorObject);
		m_StartPosition = m_ViewModel.GetPosition();
		
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
		m_EditorObject.SetPosition(m_ViewModel.GetPosition());
		CloseDialog();
	}
	
	void CancelCallback()
	{
		EditorLog.Trace("EditorObjectPropertiesDialog::CancelCallback");
		m_ViewModel.SetPosition(m_StartPosition);
		
		
		CloseDialog();
		
		
		
	}
	

}
