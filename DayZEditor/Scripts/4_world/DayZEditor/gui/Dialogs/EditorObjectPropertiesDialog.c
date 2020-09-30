
class EditorObjectPropertiesDialogController: DialogBaseController
{
	vector position;
	
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
		}
	}
}



class EditorObjectPropertiesDialog: EditorDialogBase
{
	protected EditorObjectPropertiesDialogController m_EditorObjectPropertiesDialogController;
	
	void EditorObjectPropertiesDialog(Widget parent = null, string title = "", EditorObject editor_object = null)
	{		
		m_EditorObjectPropertiesDialogController = EditorObjectPropertiesDialogController.Cast(GetController());
		m_EditorObjectPropertiesDialogController.SetEditorObject(editor_object);
		
		GroupPrefab group_prefab = new GroupPrefab(null, "Position", m_Controller);
		group_prefab.Insert(new VectorPrefab(null, "Position", m_Controller, "position", editor_object.GetPosition()));
		
		AddContent(group_prefab);
		
		AddButton(DialogResult.OK);
		AddButton(DialogResult.Cancel);
	}
	
	override typename GetControllerType() {
		return EditorObjectPropertiesDialogController;
	}
}