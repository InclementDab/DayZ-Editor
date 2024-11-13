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

			OriginalPositions[editor_object.GetWorldObject()] = editor_object.GetWorldObject().GetPosition();
			OriginalOrientations[editor_object.GetWorldObject()] = editor_object.GetWorldObject().GetDirection();
			m_AveragePosition += editor_object.GetPosition();
		}

		m_AveragePosition = Vector(m_AveragePosition[0] / m_EditorObjects.Count(), m_AveragePosition[1] / m_EditorObjects.Count(), m_AveragePosition[2] / m_EditorObjects.Count());
		
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
	protected ref map<Object, vector> OriginalPositions = new map<Object, vector>();
	protected ref map<Object, vector> OriginalOrientations = new map<Object, vector>();
	float Scale = 1.0;
	
	float Health = 100;
	bool Locked;
	bool Physics;
	bool AllowDamage = false;
	bool Collision = true;
	bool EditorOnly = false;

	protected vector m_AveragePosition;
	
	void PropertyChanged(string property_name)
	{		
		switch (property_name) {
			case "Position": {
				m_AveragePosition += Position - DeltaPosition;
				break;
			}
			
		}
		
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
					vector rot_matrix[4];
					Math3D.YawPitchRollMatrix(Orientation, rot_matrix);
					rot_matrix[3] = m_AveragePosition;

					vector new_orientation = OriginalOrientations[editor_object.GetWorldObject()] + Orientation;
					vector new_position = (OriginalPositions[editor_object.GetWorldObject()] - m_AveragePosition).Multiply4(rot_matrix);
					editor_object.Orientation = new_orientation;
					editor_object.Position = new_position;
					editor_object.PropertyChanged("Position");
					

					//vector old_orientation = editor_object.GetOrientation();
					
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