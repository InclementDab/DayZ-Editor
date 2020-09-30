
class EditorObjectPropertiesDialogController: DialogBaseController
{
	vector position;
	
	override void PropertyChanged(string property_name)
	{
		Print(position);
	}
}



class EditorObjectPropertiesDialog: EditorDialogBase
{
	void EditorObjectPropertiesDialog(Widget parent = null, string title = "", EditorObject editor_object = null)
	{		
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