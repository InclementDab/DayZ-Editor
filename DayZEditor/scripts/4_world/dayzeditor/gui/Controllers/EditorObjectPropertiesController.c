class EditorPropertiesController: Controller
{
	protected EditorObject m_EditorObject;
	protected WrapSpacerWidget EditorDialogOptionContent;
	
	string EditorObjectDisplayName;
	EquationEvaluater pos_x, pos_y, pos_z;
	EquationEvaluater rot_x, rot_y, rot_z;
	
	void SetEditorObject(EditorObject editor_object) {
		EditorLog.Trace("EditorPropertiesController::SetEditorObject");
		m_EditorObject = editor_object;
		vector position = m_EditorObject.GetPosition();
		pos_x = position[0].ToString();
		pos_z = position[1].ToString();
		pos_y = position[2].ToString();
		
		NotifyPropertyChanged("pos_x");
		NotifyPropertyChanged("pos_y");
		NotifyPropertyChanged("pos_z");
		
		
		vector orientation = m_EditorObject.GetOrientation();
		rot_z = orientation[0].ToString();
		rot_x = orientation[1].ToString();
		rot_y = orientation[2].ToString();
		
		NotifyPropertyChanged("rot_x");
		NotifyPropertyChanged("rot_y");
		NotifyPropertyChanged("rot_z");
		
		Print(m_EditorObject.GetDisplayName());
		EditorObjectDisplayName = m_EditorObject.GetDisplayName();
		NotifyPropertyChanged("EditorObjectDisplayName");
		
	}
	
	override void PropertyChanged(string property_name)
	{
		EditorLog.Trace("EditorPropertiesController::PropertyChanged");
		switch (property_name) {
			
			case "pos_x":
			case "pos_y":
			case "pos_z": {
				m_EditorObject.SetPosition(Vector(pos_x.Evaluate(), pos_z.Evaluate(), pos_y.Evaluate()));
				break;
			}
			
			case "rot_x":
			case "rot_y":
			case "rot_z": {
				m_EditorObject.SetOrientation(Vector(rot_z.Evaluate(), rot_x.Evaluate(), rot_y.Evaluate()));
				break;
			}
			
			case "EditorObjectDisplayName": {
				m_EditorObject.SetDisplayName(EditorObjectDisplayName);
				break;
			}
		}
	}
	
	
	override void MVCOnMouseWheel(Widget target, int direction, int x, int y)
	{
		EditorLog.Trace("EditorPropertiesController::OnMouseWheel %1", target.GetName());
		string w_name = target.GetName();
		
		EquationEvaluater w_eval;
		EnScript.GetClassVar(this, w_name, 0, w_eval);
		
		if (KeyState(KeyCode.KC_LCONTROL)) {
			direction *= 10;
		}
		
		EnScript.SetClassVar(this, w_name, 0, (w_eval.Evaluate() + direction).ToString());
		NotifyPropertyChanged(w_name);
	}
	
	void ToggleCategoryButton(ButtonCommandArgs args)
	{
		EditorLog.Trace("EditorObjectPropertiesDialog::ToggleCategoryButton");
		args.param1.FindAnyWidget(string.Format("%1_expand_image", args.param1.GetName())).Show(args.param3);
		args.param1.FindAnyWidget(string.Format("%1_collapse_image", args.param1.GetName())).Show(!args.param3);
		args.param1.GetSibling().Show(args.param3);
	}
}