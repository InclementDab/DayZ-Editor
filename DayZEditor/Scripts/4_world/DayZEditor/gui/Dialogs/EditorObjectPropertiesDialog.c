
class EditorObjectPropertiesDialogController: DialogBaseController
{
	vector position;
	vector orientation;
	string scale;
	
	protected EditorObject m_EditorObject;
	
	void SetEditorObject(EditorObject editor_object)
	{
		m_EditorObject = editor_object;
	}
	
	override void PropertyChanged(string property_name)
	{
		switch (property_name) {
			
			case "position": {				
				m_EditorObject.SetPosition(position);
				break;
			}			
			
			case "orientation": {				
				m_EditorObject.SetOrientation(orientation);
				break;
			}
			
			case "scale": {
				m_EditorObject.SetScale(scale.ToFloat());
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
	
		GroupPrefab group_prefab = new GroupPrefab("Position", m_Controller, string.Empty);
		group_prefab.Insert(new VectorPrefab("Position", m_Controller, "position", editor_object.GetPosition()));
		group_prefab.Insert(new VectorPrefab("Orientation", m_Controller, "orientation", editor_object.GetOrientation()));
		group_prefab.Insert(new EditBoxPrefab("Scale", m_Controller, "scale", editor_object.GetScale().ToString()));
		
		AddContent(group_prefab);
		
		AddButton(DialogResult.OK);
		AddButton(DialogResult.Cancel);
	}
	
	override typename GetControllerType() {
		return EditorObjectPropertiesDialogController;
	}
}