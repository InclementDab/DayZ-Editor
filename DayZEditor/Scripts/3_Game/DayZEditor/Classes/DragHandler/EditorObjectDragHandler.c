class EditorObjectDragHandler: EditorDragHandler
{
	protected float m_LastAngle;
	
	override void OnDragging(out vector transform[4], notnull EditorObject target)
	{
		m_Editor.GetEditorHud().SetCurrentTooltip(null);
		
		vector cursor_pos = Editor.CurrentMousePosition;
		
		vector size, ground_position, surface_normal, local_dir, local_ori;
		vector deltapos = m_EditorObject.GetWorldObject().GetPosition();
		size = m_EditorObject.GetSize();
		float scale = m_EditorObject.GetWorldObject().GetScale();
		ground_position = GetGroundPosition(transform);
		surface_normal = GetGame().SurfaceGetNormal(ground_position[0], ground_position[2]);
		float angle;
		int i;
		// Handle Z-Only motion
		// Todo will people want this as a keybind?
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
				local_dir = vector.Direction(ground_position, cursor_pos);
				local_dir.Normalize();
				transform[0] = surface_normal * local_dir;
				transform[1] = surface_normal;
				transform[2] = surface_normal * (local_dir * vector.Up);
				
			} else {
				if (m_Editor.GroundMode) {
					transform[3] = ground_position + transform[1] * vector.Distance(ground_position, transform[3]);
				}
			}
		}
		
		// Handle regular motion
		else {
			if (m_Editor.MagnetMode) {
				local_ori = m_EditorObject.GetWorldObject().GetDirection();
				transform[0] = surface_normal * local_ori;
				transform[1] = surface_normal;
				transform[2] = surface_normal * (local_ori * vector.Up);
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