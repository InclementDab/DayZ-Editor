

class EditorPropertiesController: Controller
{
	protected EditorObject m_EditorObject;
	
	EquationEvaluater pos_x, pos_y, pos_z;
	EquationEvaluater rot_x, rot_y, rot_z;
	
	void SetEditorObject(EditorObject editor_object) {
		EditorLog.Trace("EditorPropertiesController::SetEditorObject");
		m_EditorObject = editor_object;
		vector position = m_EditorObject.GetPosition();
		pos_x = position[0].ToString();
		pos_y = position[1].ToString();
		pos_z = position[2].ToString();
		
		NotifyPropertyChanged("pos_x");
		NotifyPropertyChanged("pos_y");
		NotifyPropertyChanged("pos_z");
		
	}
	
	override void PropertyChanged(string property_name)
	{
		EditorLog.Trace("EditorPropertiesController::PropertyChanged");
		switch (property_name) {
			
			case "pos_x":
			case "pos_y":
			case "pos_z": {
				m_EditorObject.SetPosition(Vector(pos_x.Evaluate(), pos_y.Evaluate(), pos_z.Evaluate()));
				break;
			}
		}
	}
}


class EditorObjectPropertiesDialog: EditorDialog
{
	
	protected EditorObject m_EditorObject;
	EditorObject GetEditorObject() { return m_EditorObject; }
	
	protected ButtonWidget m_ApplyButton;
	protected ButtonWidget m_CancelButton;
	protected EditorObject m_Object;
	
	protected ref EditorPropertiesController m_EditorPropertiesController;
	
	
	private vector m_StartPosition;
	
	void EditorObjectPropertiesDialog(EditorObject editor_object)
	{
		EditorLog.Trace("EditorObjectPropertiesDialog");
		m_EditorObject = editor_object;
		
		Widget content = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/options/EditorDialogOptionPropertiesPrefab.layout");
		AddContent(content);
		content.GetScript(m_EditorPropertiesController);
		m_EditorPropertiesController.SetEditorObject(m_EditorObject);
		
		SetTitle("Edit: Object Properties");
		AddButton("Save", "SaveCallback");
		AddButton("Cancel", "CancelCallback");
		
	}
	
	void ~EditorObjectPropertiesDialog() {
		EditorLog.Trace("~EditorObjectPropertiesDialog");
	}
		
	
	void SaveCallback() {
		EditorLog.Trace("EditorObjectPropertiesDialog::SaveCallback");
		CloseDialog();
	}
	
	void CancelCallback() {
		EditorLog.Trace("EditorObjectPropertiesDialog::CancelCallback");
		CloseDialog();
	}
	

}
