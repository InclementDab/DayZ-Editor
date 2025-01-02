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
			Name = m_EditorObjects[0].GetDisplayName();
		}
		
		vector average_positon_of_selection = GetEditor().GetObjectManager().GetAveragePositionOfSelection();
		foreach (EditorObject editor_object: m_EditorObjects) {
			if (Name != editor_object.GetDisplayName()) {
				Name = string.Empty;
			}

			OriginalPositions[editor_object.GetWorldObject()] = editor_object.GetWorldObject().GetPosition() - average_positon_of_selection;
			OriginalOrientations[editor_object.GetWorldObject()] = editor_object.GetWorldObject().GetDirection();
		}

		m_CenterPoint = average_positon_of_selection;
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
	
	void PropertyChanged(string property_name)
	{
		vector matrix[4];
		Math3D.YawPitchRollMatrix(Orientation, matrix);
		matrix[3] = m_CenterPoint + Position;
		foreach (EditorObject editor_object: m_EditorObjects) {
			switch (property_name) {
				case "Show": {
					editor_object.Show(Show);
					break;
				}
				
				case "Name": {
					editor_object.SetDisplayName(Name);
					break;
				}
							
				case "Position":
				case "Orientation": {
					EditorAction position_undo = new EditorAction("SetTransform", "SetTransform");
					position_undo.InsertUndoParameter(editor_object.GetTransformArray());

					vector transform[4];
					Math3D.YawPitchRollMatrix(OriginalOrientations[editor_object.GetWorldObject()], transform);
					transform[3] = OriginalPositions[editor_object.GetWorldObject()];
					Math3D.MatrixMultiply4(matrix, transform, transform);

					editor_object.SetTransform(transform);
					position_undo.InsertRedoParameter(editor_object.GetTransformArray());
					GetEditor().InsertAction(position_undo);
					break;
				}
								
				case "Scale": {
					editor_object.SetScale(Scale);
					break;
				}
				
				case "Health": {
					editor_object.SetHealth(Health);
					break;
				}

				case "Locked": {
					editor_object.Lock(Locked);
					break;
				}
				
				case "Physics": {
					editor_object.SetPhysicsEnabled(Physics);
					break;
				}
				
				case "AllowDamage": {
					editor_object.SetAllowDamage(AllowDamage);
					break;
				}
				
				case "Collision": {
					//editor_object.Collision = Collision;
					break;
				}
				
				case "EditorOnly": {
					//editor_object.EditorOnly = EditorOnly;
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
			position = position + object.GetPosition();
		}
		
		for (int i = 0; i < 3; i++) {
			position[i] = position[i] / objects.Count();
		}
		
		return position;
	}
}