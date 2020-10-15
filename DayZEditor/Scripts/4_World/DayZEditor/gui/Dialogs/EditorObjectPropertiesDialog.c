//! TODO: remove this controller and directly connect with EditorObject
//! (maybe add a NotifyPropertyChanged to EditorObject to handle once things get changed?)
//! This would be another step closer to removing EditorObjectData since Scale, Position, and Orientation would 
//! become public variables, with a Notify once things get changed? (Along with SetPosition methods etc..)
class EditorObjectPropertiesDialogController: DialogBaseController
{
	string name;
	vector position;
	vector orientation;
	float scale;
	
	bool static_object;
	bool locked;
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
				m_EditorObject.SetScale(scale);
				break;
			}
			
			case "static_object": {
				m_EditorObject.SetStaticObject(static_object);
				break;
			}
			
			case "locked": {
				m_EditorObject.SetLocked(locked);
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
		
		GroupPrefab object_group = new GroupPrefab("Object Settings", m_Controller, string.Empty);
		object_group.Insert(new CheckBoxPrefab("Lock", m_Controller, "locked", editor_object.IsLocked()));
		object_group.Insert(new CheckBoxPrefab("Static Object", m_Controller, "static_object", editor_object.IsStaticObject()));
		
		AddContent(general_group);
		AddContent(object_group);
		
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