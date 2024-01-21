/*

class EditorObjectDragHandler: EditorDragHandler
{
	static vector DoCursorRaycast(float max_distance = 3000, Object ignore_object = null)
	{
		vector position;	
		vector end_pos = GetGame().GetCurrentCameraPosition() + GetGame().GetPointerDirection() * max_distance;
		int interaction_layers = PhxInteractionLayers.BUILDING | PhxInteractionLayers.ROADWAY | PhxInteractionLayers.TERRAIN | PhxInteractionLayers.ITEM_SMALL | PhxInteractionLayers.DYNAMICITEM | PhxInteractionLayers.ITEM_LARGE;
		Object hit_object;
		vector normal;
		float fraction;
		DayZPhysics.RayCastBullet(GetGame().GetCurrentCameraPosition(), end_pos, interaction_layers, ignore_object, hit_object, position, normal, fraction);
		return position;
	}
	
	static void Drag(notnull Object object, vector offset)
	{
		vector transform[4];		
		object.GetTransform(transform);
		vector original_position = transform[3];
		
		vector cursor_position = DoCursorRaycast(3000, object);
		
		// Attempted placement position, now filter it
		transform[3] = cursor_position.InvMultiply4(transform) + offset.Multiply4(transform);
		
		if (KeyState(KeyCode.KC_LMENU)) {			
			Debug.DrawCylinder(cursor_position, 0.1, 1000, COLOR_GREEN, ShapeFlags.ONCE);
			transform[3][0] = object.GetPosition()[0];
			transform[3][2] = object.GetPosition()[2];
		}
		
		if (KeyState(KeyCode.KC_LSHIFT)) {
			transform[3] = object.GetPosition();
			
			Shape.CreateArrow(transform[3], cursor_position, 1, COLOR_WHITE, ShapeFlags.ONCE);
						
			vector new_forward = vector.Direction(transform[3], cursor_position);
			
			vector transform_matrix[4];
			Math3D.DirectionAndUpMatrix(new_forward, vector.Up, transform);
			
			//Math3D.MatrixMultiply3(transform_matrix, transform, transform);
		}
		
		if (GetDayZGame().GetEditor().MagnetMode) {
			vector ground_position = GetGroundPosition(transform);
			vector surface_normal = GetGame().SurfaceGetNormal(ground_position[0], ground_position[2]);
			Math3D.DirectionAndUpMatrix(transform[2], surface_normal, transform);
		}
		
		if (GetDayZGame().GetEditor().GroundMode) {
			
		}
		
		object.SetTransform(transform);
		//editor_object.GetWorldObject().SetPosition(cursor_position - m_Offset.Multiply3(transform));
	}
	
	static void ProcessMagnetTransform(inout vector transform[4])
	{

	}
	
	protected float m_LastAngle;
	
	override void OnDragging(out vector transform[4], notnull EditorObject target)
	{
		delete m_Editor.GetHud().CurrentTooltip;
		
		vector cursor_pos = Editor.CurrentMousePosition;
		
		vector local_dir, local_ori;
		vector deltapos = m_EditorObject.GetWorldObject().GetPosition();
		vector size = m_EditorObject.GetSize();
		float scale = m_EditorObject.GetWorldObject().GetScale();
		vector ground_position = GetGroundPosition(transform);
		vector surface_normal = GetGame().SurfaceGetNormal(ground_position[0], ground_position[2]);
		float angle;
		int i;
		
		if (KeyState(KeyCode.KC_LMENU)) {
			cursor_pos = GetGame().GetCurrentCameraPosition() + GetGame().GetPointerDirection() * vector.Distance(GetGame().GetCurrentCameraPosition(), m_EditorObject.GetBasePoint());
			cursor_pos[1] = cursor_pos[1] + size[1] / 2;
			if (m_Editor.MagnetMode) {
				transform[3] = ground_position + transform[1] * vector.Distance(ground_position, cursor_pos + GetGame().GetCurrentCameraDirection() * 1);
			} else {
				transform[3][1] = cursor_pos[1];
			}
		}
		
		// Handle XY Rotation
		else if (KeyState(KeyCode.KC_LSHIFT)) {
			vector cursor_delta = ground_position - Editor.CurrentMousePosition;
			local_ori = m_EditorObject.GetWorldObject().GetOrientation();
			angle = Math.Atan2(cursor_delta[0], cursor_delta[2]);
			local_ori[0] = local_ori[0] + ((angle - m_LastAngle) * Math.RAD2DEG);
			local_ori.RotationMatrixFromAngles(transform);
			
			for (i = 0; i < 3; i++) {
				transform[i] = transform[i] * scale;
			}
			
			if (m_Editor.MagnetMode) {
				Math3D.DirectionAndUpMatrix(vector.Direction(ground_position, cursor_pos).Normalized(), surface_normal, transform);
				
			} else {
				if (m_Editor.GroundMode) {
					transform[3] = ground_position + transform[1] * vector.Distance(ground_position, transform[3]);
				}
			}
		}
		
		// Handle regular motion
		else {
			if (m_Editor.MagnetMode) {
				
			}
			
			if (m_Editor.GroundMode) {
				if (m_Editor.MagnetMode) {
					transform[3] = cursor_pos + surface_normal * vector.Distance(ground_position, transform[3]);				
				} else {
					transform[3] = cursor_pos + transform[1] * vector.Distance(ground_position, transform[3]);
				}
				
			} else {				
				transform[3] = cursor_pos + m_EditorObject.GetBasePointOffset();
			} 			
		}
		
		deltapos = transform[3] - deltapos;
		
		// Handle all child objects
		array<EditorObject> selected_objects = EditorObject.GetSelectedEditorObjects();	
		foreach (EditorObject selected_object: selected_objects) {
			if (selected_object == target) { 
				continue; 
			}
			
			vector selected_pos = selected_object.GetWorldObject().GetPosition();
			vector selected_ori = selected_object.GetWorldObject().GetOrientation();
			
			// Handle Z-Only motion
			if (KeyState(KeyCode.KC_LMENU)) {
				selected_object.GetWorldObject().SetPosition(selected_object.GetWorldObject().GetPosition() + deltapos);
			}
			
			else if (KeyState(KeyCode.KC_LSHIFT)) {
				if (angle - m_LastAngle == 0) {
					continue;
				}
				
				selected_object.GetWorldObject().SetPosition(EditorMath.RotateAroundPoint(transform[3], selected_object.GetWorldObject().GetPosition(), vector.Up, Math.Cos(angle - m_LastAngle), Math.Sin(angle - m_LastAngle)));
				vector new_ori = selected_object.GetWorldObject().GetOrientation();
				new_ori[0] = new_ori[0] + ((angle - m_LastAngle) * Math.RAD2DEG);
				selected_object.GetWorldObject().SetOrientation(new_ori);
			} else {
				selected_object.GetWorldObject().SetPosition(selected_object.GetWorldObject().GetPosition() + deltapos);
			}
		}
		
		m_LastAngle = angle;
	}
}