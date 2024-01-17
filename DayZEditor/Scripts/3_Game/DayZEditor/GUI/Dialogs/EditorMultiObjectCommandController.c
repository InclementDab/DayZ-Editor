// This is the handler for when the dialog has multiple objects selected
class EditorMultiObjectCommandController
{
	protected ref array<EditorObject> m_EditorObjects = {};
	protected vector m_CenterPoint;
	
	void EditorMultiObjectCommandController(notnull array<EditorObject> editor_objects)
	{
		m_EditorObjects.InsertArray(editor_objects);
		
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
	
	void PropertyChanged(string property_name)
	{		
		foreach (EditorObject editor_object: m_EditorObjects) {
			switch (property_name) {
				case "Show": {
					editor_object.SetFlag(EditorObjectFlags.HIDDEN * !Show);
					break;
				}
				
				case "Name": {
					//editor_object.SetDisplayName(Name);
					break;
				}
							
				case "Position": {
					editor_object.GetWorldObject().SetPosition(editor_object.GetWorldObject().GetPosition() + Position - DeltaPosition);
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
					
					editor_object.GetWorldObject().SetPosition(EditorMath.RotateAroundPoint(m_CenterPoint, editor_object.GetWorldObject().GetPosition(), direction_of_rotation, Math.Cos(Orientation[index_moved] - DeltaOrientation[index_moved]), Math.Sin(Orientation[index_moved] - DeltaOrientation[index_moved])));
					
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
					editor_object.GetWorldObject().SetScale(Scale);
					break;
				}
				
				case "Health": {
					editor_object.GetWorldObject().SetHealth("", "", Health);
					break;
				}

				case "Locked": {
					editor_object.SetFlag(EditorObjectFlags.LOCKED * Locked);
					break;
				}
								
				case "Simulate": {
					editor_object.SetFlag(EditorObjectFlags.SIMULATE * Simulate);
					break;
				}
				
				case "AllowDamage": {
					editor_object.SetFlag(EditorObjectFlags.ALLOW_DAMAGE * AllowDamage);
					break;
				}
								
				case "EditorOnly": {
					editor_object.SetFlag(EditorObjectFlags.EDITOR_ONLY * EditorOnly);
					break;
				}
			}
		}
		
		DeltaPosition = Position;
		DeltaOrientation = Orientation;
	}
	
	vector GetCenterPoint(array<EditorObject> objects)
	{
		vector position;
		foreach (EditorObject object: objects) {
			position = position + object.GetWorldObject().GetPosition();
		}
		
		for (int i = 0; i < 3; i++) {
			position[i] = position[i] / objects.Count();
		}
		
		return position;
	}
}