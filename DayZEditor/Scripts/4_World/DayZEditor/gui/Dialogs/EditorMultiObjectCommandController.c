// This is the handler for when the dialog has multiple objects selected
class EditorMultiObjectCommandController
{
	protected ref array<EditorObject> m_EditorObjects = {};
	
	void EditorMultiObjectCommandController(notnull array<EditorObject> editor_objects)
	{
		m_EditorObjects.InsertArray(editor_objects);
		
		// Name will be, by default, the first object to be selected
		// if there are multiple objects with different names, then the name dialog will go blank
		if (m_EditorObjects.Count() > 0) {
			Name = m_EditorObjects[0].Name;
		}
		
		foreach (EditorObject editor_object: m_EditorObjects) {
			if (Name != editor_object.Name) {
				Name = string.Empty;
			}
		}
	}
	
	void ~EditorMultiObjectCommandController()
	{
		delete m_EditorObjects;
	}
	
	bool Show = true;
	string Name;
	vector Position, DeltaPosition;
	vector Orientation, DeltaOrientation;
	float Scale = 1.0;
	
	float Health = 100;
	bool Locked;
	bool Physics;
	bool Simulate = true;
	bool AllowDamage = false;
	bool Collision = true;
	bool EditorOnly = false;
	
	void PropertyChanged(string property_name)
	{
		foreach (EditorObject editor_object: m_EditorObjects) {
			switch (property_name) {
				case "Show": {
					editor_object.Show = Show;
					break;
				}
				
				case "Name": {
					editor_object.Name = Name;
					break;
				}
							
				case "Position": {
					editor_object.Position += Position - DeltaPosition;
					break;
				}
				
				case "Orientation": {
					editor_object.Orientation += Orientation - DeltaOrientation;
					break;
				}
				
				case "Scale": {
					editor_object.Scale = Scale;
					break;
				}
				
				case "Health": {
					editor_object.Health = Health;
					break;
				}

				case "Locked": {
					editor_object.Locked = Locked;
					break;
				}
				
				case "Physics": {
					editor_object.Physics = Physics;
					break;
				}
				
				case "Simulate": {
					editor_object.Simulate = Simulate;
					break;
				}
				
				case "AllowDamage": {
					editor_object.AllowDamage = AllowDamage;
					break;
				}
				
				case "Collision": {
					editor_object.Collision = Collision;
					break;
				}
				
				case "EditorOnly": {
					editor_object.EditorOnly = EditorOnly;
					break;
				}
			}
			
			editor_object.PropertyChanged(property_name);
		}
		
		DeltaPosition = Position;
		DeltaOrientation = Orientation;
	}
}