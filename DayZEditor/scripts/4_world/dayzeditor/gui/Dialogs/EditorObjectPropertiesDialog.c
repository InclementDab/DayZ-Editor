

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
		
		
		vector orientation = m_EditorObject.GetOrientation();
		rot_x = orientation[0].ToString();
		rot_y = orientation[1].ToString();
		rot_z = orientation[2].ToString();
		
		NotifyPropertyChanged("rot_x");
		NotifyPropertyChanged("rot_y");
		NotifyPropertyChanged("rot_z");
		
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
			
			case "rot_x":
			case "rot_y":
			case "rot_z": {
				m_EditorObject.SetOrientation(Vector(rot_x.Evaluate(), rot_y.Evaluate(), rot_z.Evaluate()));
				break;
			}
		}
	}
	
	override void OnMouseWheel(Widget w, int direction, int x, int y)
	{
		EditorLog.Trace("EditorPropertiesController::OnMouseWheel %1", w.GetName());
		string w_name = w.GetName();
		
		EquationEvaluater w_eval;
		EnScript.GetClassVar(this, w_name, 0, w_eval);
		EnScript.SetClassVar(this, w_name, 0, (w_eval.Evaluate() + direction).ToString());
		NotifyPropertyChanged(w_name);
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
	override Controller GetController() {
		return m_EditorPropertiesController;
	}
	
	private vector m_StartPosition;
	private vector m_StartOrientation;
	
	void EditorObjectPropertiesDialog(EditorObject editor_object)
	{
		EditorLog.Trace("EditorObjectPropertiesDialog");
		m_EditorObject = editor_object;
		
		Widget content = GetGame().GetWorkspace().CreateWidgets("DayZEditor/gui/Layouts/options/EditorDialogOptionPropertiesPrefab.layout");
		AddContent(content);
		content.GetScript(m_EditorPropertiesController);
		m_EditorPropertiesController.SetEditorObject(m_EditorObject);
		m_StartPosition = m_EditorObject.GetPosition();
		m_StartOrientation = m_EditorObject.GetOrientation();
		
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
		m_EditorObject.SetPosition(m_StartPosition);
		m_EditorObject.SetOrientation(m_StartOrientation);
		CloseDialog();
	}
	

}
