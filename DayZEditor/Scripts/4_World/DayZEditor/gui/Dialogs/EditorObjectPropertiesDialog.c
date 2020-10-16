
// Stores state of the Collapse data
class EditorObjectPropertiesDialogState
{
	static bool GeneralGroup = true;
	static bool ObjectGroup = true;
	static bool FlagsGroup = true;
}

class EditorObjectPropertiesDialog: EditorDialogBase
{
	
	protected ref GroupPrefab m_GeneralGroup;
	protected ref GroupPrefab m_ObjectGroup;
	protected ref GroupPrefab m_FlagsGroup;
	
	protected EditorObject m_EditorObject;
	
	void EditorObjectPropertiesDialog(string title, EditorObject editor_object)
	{
		SetEditorObject(editor_object);
				
		AddButton(DialogResult.OK);
		AddButton(DialogResult.Cancel);
		
		EditorEvents.OnObjectSelected.Insert(OnObjectSelected);
	}
	
	void ~EditorObjectPropertiesDialog()
	{
		EditorObjectPropertiesDialogState.GeneralGroup = m_GeneralGroup.IsOpen();
		EditorObjectPropertiesDialogState.ObjectGroup = m_ObjectGroup.IsOpen();
		EditorObjectPropertiesDialogState.FlagsGroup = m_FlagsGroup.IsOpen();
		
		delete m_GeneralGroup;
		delete m_ObjectGroup;
		delete m_FlagsGroup;
	}
	
	private void OnObjectSelected(Class context, EditorObject editor_object)
	{
		SetEditorObject(editor_object);
	}
	
	// Maximum Elegance
	void SetEditorObject(EditorObject editor_object)
	{
		m_EditorObject = editor_object;
		
		EditorObjectPropertiesDialogState.GeneralGroup = m_GeneralGroup.IsOpen();
		EditorObjectPropertiesDialogState.ObjectGroup = m_ObjectGroup.IsOpen();
		EditorObjectPropertiesDialogState.FlagsGroup = m_FlagsGroup.IsOpen();
		
		delete m_GeneralGroup;
		delete m_ObjectGroup;
		delete m_FlagsGroup;
		
		m_GeneralGroup = new GroupPrefab("General", m_EditorObject, string.Empty);
		m_GeneralGroup.Insert(new CheckBoxPrefab("Show", m_EditorObject, "Show", m_EditorObject.IsVisible()));
		m_GeneralGroup.Insert(new EditBoxPrefab("Name", m_EditorObject, "Name", m_EditorObject.GetDisplayName()));
		m_GeneralGroup.Insert(new VectorPrefab("Position", m_EditorObject, "Position", m_EditorObject.GetPosition()));
		m_GeneralGroup.Insert(new VectorPrefab("Orientation", m_EditorObject, "Orientation", m_EditorObject.GetOrientation()));
		m_GeneralGroup.Insert(new EditBoxNumberPrefab("Scale", m_EditorObject, "Scale", m_EditorObject.GetScale().ToString(), 0.01));
		m_GeneralGroup.Open(EditorObjectPropertiesDialogState.GeneralGroup);
		
		m_ObjectGroup = new GroupPrefab("Object Settings", m_EditorObject, string.Empty);
		m_ObjectGroup.Insert(new CheckBoxPrefab("Lock", m_EditorObject, "Locked", m_EditorObject.IsLocked()));
		m_ObjectGroup.Insert(new CheckBoxPrefab("Static Object", m_EditorObject, "StaticObject", m_EditorObject.IsStaticObject()));
		m_ObjectGroup.Open(EditorObjectPropertiesDialogState.ObjectGroup);
		
		m_FlagsGroup = new GroupPrefab("Object Flags", m_EditorObject, string.Empty);
		m_FlagsGroup.Insert(new CheckBoxPrefab("Bounding Box", m_EditorObject, "BoundingBoxEnabled", m_EditorObject.BoundingBoxEnabled));
		m_FlagsGroup.Insert(new CheckBoxPrefab("World Marker", m_EditorObject, "WorldMarkerEnabled", m_EditorObject.WorldMarkerEnabled));
		m_FlagsGroup.Insert(new CheckBoxPrefab("Map Marker", m_EditorObject, "MapMarkerEnabled", m_EditorObject.MapMarkerEnabled));
		m_FlagsGroup.Insert(new CheckBoxPrefab("List Item", m_EditorObject, "ListItemEnabled", m_EditorObject.ListItemEnabled));
		m_FlagsGroup.Open(EditorObjectPropertiesDialogState.FlagsGroup);
		
		AddContent(m_GeneralGroup);
		AddContent(m_ObjectGroup);
		AddContent(m_FlagsGroup);
		
		if (editor_object.GetWorldObject().IsMan()) {
			GroupPrefab human_controller = new GroupPrefab("Human Controller", m_EditorObject, string.Empty);
			human_controller.Insert(new ButtonPrefab("Button1", m_EditorObject, "button"));
			AddContent(human_controller);
		}
	}
}