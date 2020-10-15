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
	
	bool show;
	bool static_object;
	bool locked;
	
	bool bounding_box;
	bool world_marker;
	bool map_marker;
	bool list_item;
	
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
						
			case "show": {
				m_EditorObject.Show(show);
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
			
			case "bounding_box": {
				m_EditorObject.EnableBoundingBox(bounding_box);
				break;
			}	
					
			case "world_marker": {
				m_EditorObject.EnableObjectMarker(world_marker);
				break;
			}	
							
			case "map_marker": {
				m_EditorObject.EnableMapMarker(map_marker);
				break;
			}
			
			case "list_item": {
				m_EditorObject.EnableListItem(list_item);
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
		general_group.Insert(new CheckBoxPrefab("Show", m_Controller, "show", editor_object.IsVisible()));
		general_group.Insert(new EditBoxPrefab("Name", m_Controller, "name", editor_object.GetDisplayName()));
		general_group.Insert(new VectorPrefab("Position", m_Controller, "position", editor_object.GetPosition()));
		general_group.Insert(new VectorPrefab("Orientation", m_Controller, "orientation", editor_object.GetOrientation()));
		general_group.Insert(new EditBoxNumberPrefab("Scale", m_Controller, "scale", editor_object.GetScale().ToString(), 0.01));
		
		GroupPrefab object_group = new GroupPrefab("Object Settings", m_Controller, string.Empty);
		object_group.Insert(new CheckBoxPrefab("Lock", m_Controller, "locked", editor_object.IsLocked()));
		object_group.Insert(new CheckBoxPrefab("Static Object", m_Controller, "static_object", editor_object.IsStaticObject()));
		
		GroupPrefab flags_Group = new GroupPrefab("Object Flags", m_Controller, string.Empty);
		flags_Group.Insert(new CheckBoxPrefab("Bounding Box", m_Controller, "bounding_box", editor_object.BoundingBoxEnabled()));
		flags_Group.Insert(new CheckBoxPrefab("World Marker", m_Controller, "world_marker", editor_object.ObjectMarkerEnabled()));
		flags_Group.Insert(new CheckBoxPrefab("Map Marker", m_Controller, "map_marker", editor_object.MapMarkerEnabled()));
		flags_Group.Insert(new CheckBoxPrefab("List Item", m_Controller, "list_item", editor_object.ListItemEnabled()));
		
		
		AddContent(general_group);
		AddContent(object_group);
		AddContent(flags_Group);
		
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