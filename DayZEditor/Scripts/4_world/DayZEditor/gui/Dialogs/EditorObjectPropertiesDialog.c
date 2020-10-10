
class EditorObjectPropertiesDialogController: DialogBaseController
{
	string name;
	vector position;
	vector orientation;
	float scale;
	
	bool button;
	
	protected EditorObject m_EditorObject;
	
	void SetEditorObject(EditorObject editor_object)
	{
		m_EditorObject = editor_object;
	}
	
	override void PropertyChanged(string property_name)
	{
		switch (property_name) {
			
			case "name": {
				m_EditorObject.SetDisplayName(name);
				break;
			}
			
			case "position": {				
				m_EditorObject.SetPosition(position);
				break;
			}			
			
			case "orientation": {				
				m_EditorObject.SetOrientation(orientation);
				break;
			}
			
			case "scale": {
				//m_EditorObject.SetScale(scale);
				break;
			}
			
			case "button": {
				
				Print("AAA");
				break;
			}
		}
	}
}



class EditorObjectPropertiesDialog: EditorDialogBase
{
	protected EditorObjectPropertiesDialogController m_EditorObjectPropertiesDialogController;
	
	void EditorObjectPropertiesDialog(string title, EditorObject editor_object)
	{		
		m_EditorObjectPropertiesDialogController = EditorObjectPropertiesDialogController.Cast(GetController());
		m_EditorObjectPropertiesDialogController.SetEditorObject(editor_object);
	
		GroupPrefab general_group = new GroupPrefab("General", m_Controller, string.Empty);
		general_group.Insert(new EditBoxPrefab("Name", m_Controller, "name", editor_object.GetDisplayName()));
		general_group.Insert(new VectorPrefab("Position", m_Controller, "position", editor_object.GetPosition()));
		general_group.Insert(new VectorPrefab("Orientation", m_Controller, "orientation", editor_object.GetOrientation()));
		general_group.Insert(new EditBoxNumberPrefab("Scale", m_Controller, "scale", editor_object.GetScale().ToString(), 0.01));
		
		AddContent(general_group);
		
		
		if (editor_object.GetWorldObject().IsMan()) {
			GroupPrefab human_controller = new GroupPrefab("Human Controller", m_Controller, string.Empty);
			human_controller.Insert(new ButtonPrefab("Button1", m_Controller, "button"));
			AddContent(human_controller);
		}
		
		AddButton(DialogResult.OK);
		AddButton(DialogResult.Cancel);
	}
	
	override typename GetControllerType() {
		return EditorObjectPropertiesDialogController;
	}
}