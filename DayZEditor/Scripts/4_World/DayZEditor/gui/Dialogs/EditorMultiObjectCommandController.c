// This is the handler for when the dialog has multiple objects selected
class EditorMultiObjectCommandController: EditorObjectController
{
	protected ref array<EditorObject> m_EditorObjects = {};	
	protected vector m_CenterPoint;

	void EditorMultiObjectCommandController(EditorObject editor_object, notnull array<EditorObject> editor_objects)
	{
		m_EditorObjects.InsertArray(editor_objects);
		
		// Name will be, by default, the first object to be selected
		// if there are multiple objects with different names, then the name dialog will go blank
		if (m_EditorObjects.Count() > 0) {
			Position = vector.Zero;
			Orientation = vector.Zero;
			Scale = 1.0;

			// Store the initial state for calculating deltas on subsequent changes.
			DeltaPosition = Position;
			DeltaOrientation = Orientation;
			DeltaScale = Scale;
			
			// Calculate the group's pivot point. This does not affect the UI display.
			m_CenterPoint = GetEditor().GetObjectManager().GetAveragePositionOfSelection();
			
			// Logic to handle ambiguous properties for the UI (like Name).
			bool hasConsistentName = true;
			EditorObject first_obj = m_EditorObjects[0];
			Name = first_obj.GetDisplayName();
			
			foreach (EditorObject editor_object2: m_EditorObjects) {
				if (Name != editor_object2.GetDisplayName())
					hasConsistentName = false;
			}

			if (!hasConsistentName)
				Name = string.Empty;
		}
	}
	
	void ~EditorMultiObjectCommandController()
	{
	}
	
	vector DeltaPosition;
	vector DeltaOrientation;
	float DeltaScale = 1.0;
		
	override void PropertyChanged(string property_name)
	{
		Event_OnPropertyChanged.Invoke(property_name);
		
		switch (property_name) {
			case "Show": {
				foreach (EditorObject obj_show : m_EditorObjects)
					obj_show.Show(Show);
				break;
			}
			
			case "Name": {
				foreach (EditorObject obj_name : m_EditorObjects)
					obj_name.SetDisplayName(Name);
				break;
			}
						
			case "Position": {
				vector position_delta = Position - DeltaPosition;
				foreach (EditorObject obj_pos : m_EditorObjects)
				{
					obj_pos.SetPosition(obj_pos.GetPosition() + position_delta);
				}
				break;
			}

			case "Orientation": {
				vector orientation_delta = Orientation - DeltaOrientation;
				vector delta_rotation_matrix[4];
				Math3D.YawPitchRollMatrix(orientation_delta, delta_rotation_matrix);

				foreach (EditorObject obj_ori : m_EditorObjects)
				{
					// 1. Get the object's current transform matrix.
					vector current_transform[4];
					obj_ori.GetTransform(current_transform);

					// 2. Translate the object so the group's pivot point is at the world origin (0,0,0).
					current_transform[3] = current_transform[3] - m_CenterPoint;

					// 3. Create the final transformation matrix by multiplying the delta rotation
					//    by the object's temporarily translated matrix. This rotates the object
					//    around the world origin (which is where our pivot is).
					vector final_transform[4];
					Math3D.MatrixMultiply4(delta_rotation_matrix, current_transform, final_transform);

					// 4. Translate the object back, moving the pivot from the world origin
					//    to its original position.
					final_transform[3] = final_transform[3] + m_CenterPoint;
					
					// 5. Apply the fully calculated new transform.
					obj_ori.SetTransform(final_transform);
				}
				break;
			}
							
			case "Scale": {
				float scale_multiplier = 1.0;
				if (DeltaScale != 0) {
					scale_multiplier = Scale / DeltaScale;
				}

				foreach (EditorObject obj_scale : m_EditorObjects)
				{
					obj_scale.SetScale(obj_scale.GetScale() * scale_multiplier);
				}
				break;
			}
			
			case "Health": {
				foreach (EditorObject obj_health : m_EditorObjects)
					obj_health.SetHealth(Health);
				break;
			}

			case "Locked": {
				foreach (EditorObject obj_lock : m_EditorObjects)
					obj_lock.Lock(Locked);
				break;
			}
			
			case "UsePhysics": {
				foreach (EditorObject obj_phys : m_EditorObjects)
					obj_phys.SetPhysicsEnabled(UsePhysics);
				break;
			}
			
			case "AllowDamage": {
				foreach (EditorObject obj_damage : m_EditorObjects)
					obj_damage.SetAllowDamage(AllowDamage);
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
		
		// Update the 'last known' values for the next delta calculation.
		DeltaPosition = Position;
		DeltaOrientation = Orientation;
		DeltaScale = Scale;
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