// This is the handler for when the dialog has multiple objects selected
class EditorMultiObjectCommandController
{
	protected ref array<EditorObject> m_EditorObjects = {};
	protected vector m_CenterPoint;
	
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
		
		// determine center point on startup
		m_CenterPoint = GetCenterPoint(m_EditorObjects);
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
	
	void PropertyChanged(Class source, string property_name)
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
					vector movement_on_tick = Orientation - DeltaOrientation;					
					int index_moved = -1;
					for (int i = 0; i < 3; i++) {
						if (movement_on_tick[i] != 0) {
							index_moved = i;
						}
					}
					
					if (index_moved == -1) {
						break;
					}
				
					// some cool logic here	
					vector direction_of_rotation;
					direction_of_rotation[index_moved] = 1;
					
					editor_object.SetPosition(EditorMath.RotateAroundPoint(m_CenterPoint, editor_object.Position, direction_of_rotation, Math.Cos(Orientation[index_moved] - DeltaOrientation[index_moved]), Math.Sin(Orientation[index_moved] - DeltaOrientation[index_moved])));
					
					/*
					//holy FUCK this is hard
					vector new_ori = editor_object.GetOrientation();
					
					new_ori[1] = new_ori[1] + ((Orientation[0] - DeltaOrientation[0]) * Math.RAD2DEG);
					new_ori[0] = new_ori[0] + ((Orientation[1] - DeltaOrientation[1]) * Math.RAD2DEG);
					new_ori[2] = new_ori[2] + ((Orientation[2] - DeltaOrientation[2]) * Math.RAD2DEG);
					editor_object.SetOrientation(new_ori);
					*/
					// actually updating the position prop now ;)
					//editor_object.PropertyChanged(source, "Position");
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
			
			editor_object.PropertyChanged(source, property_name);
		}
		
		DeltaPosition = Position;
		DeltaOrientation = Orientation;
	}
	
	vector GetCenterPoint(array<EditorObject> objects)
	{
		vector position;
		foreach (EditorObject object: objects) {
			position = position + object.Position;
		}
		
		for (int i = 0; i < 3; i++) {
			position[i] = position[i] / objects.Count();
		}
		
		return position;
	}
}