
class DragHandler
{
	protected ref EditorObject m_EditorObject;
	void DragHandler(EditorObject target)
	{
		m_EditorObject = target;
	}
	
	void OnDragStart()
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_GAMEPLAY).Insert(_OnDragging);
	}
	
	void OnDragFinish()
	{
		GetGame().GetUpdateQueue(CALL_CATEGORY_GAMEPLAY).Remove(_OnDragging);
	}
	
	private void _OnDragging()
	{
		if (GetMouseState(MouseState.LEFT) & MB_PRESSED_MASK) {
			vector transform[4];
			m_EditorObject.GetTransform(transform);
			OnDragging(transform, m_EditorObject);
			m_EditorObject.SetTransform(transform);
			m_EditorObject.Update();
		} else {
			OnDragFinish();
		}
	}
	
	void OnDragging(out vector transform[4], notnull EditorObject target);
	
	
	vector GetGroundPosition(vector transform[4])
	{
		// Raycast ground below object
		vector ground, ground_dir; 
		int component;
		DayZPhysics.RaycastRV(transform[3], transform[3] + transform[1] * -1000, ground, ground_dir, component, null, null, null, false, true);
		return ground;
	}
}


class ObjectDragHandler: DragHandler
{
	override void OnDragging(out vector transform[4], notnull EditorObject target)
	{
		vector begin_pos = GetGame().GetCurrentCameraPosition();
		vector end_pos = begin_pos + GetGame().GetPointerDirection() * 3000;
		vector cursor_pos, contact_dir;
		int component;
		
		DayZPhysics.RaycastRV(begin_pos, end_pos, cursor_pos, contact_dir, component, null, null, null, false, !GetEditor().GetSettings().ObjectDragCollisions);
		
				
		vector size = m_EditorObject.GetSize();
		vector ground_position = GetGroundPosition(transform);
		vector surface_normal = GetGame().SurfaceGetNormal(ground_position[0], ground_position[2]);
		
		
		// Handle Z-Only motion
		if (KeyState(KeyCode.KC_LMENU)) {

			cursor_pos = GetGame().GetCurrentCameraPosition() + GetGame().GetPointerDirection() * vector.Distance(GetGame().GetCurrentCameraPosition(), ground_position);
			cursor_pos[1] = cursor_pos[1] + size[1]/2;
			transform[3] = ground_position + transform[1] * vector.Distance(ground_position, cursor_pos);
		}
		
		// Handle XY Rotation
		else if (KeyState(KeyCode.KC_LSHIFT)) {
					
			//transform = { "1 0 0", "0 1 0", "0 0 1", transform[3] };
			vector cursor_delta = cursor_pos - transform[3];
			float angle = Math.Atan2(cursor_delta[0], cursor_delta[2]) * Math.RAD2DEG;	
			m_EditorObject.PlaceOnSurfaceRotated(transform, ground_position, surface_normal[0] * -1, surface_normal[2] * -1, angle * -1, GetEditor().GetEditorHud().GetTemplateController().GetToolbarController().MagnetButton);			
		}
		
		// Handle regular motion
		else {
			
			//transform = { "1 0 0", "0 1 0", "0 0 1", transform[3] };
			m_EditorObject.PlaceOnSurfaceRotated(transform, transform[3], surface_normal[0] * -1, surface_normal[2] * -1, m_EditorObject.LocalAngle * -1, GetEditor().GetEditorHud().GetTemplateController().GetToolbarController().MagnetButton);
			cursor_pos[1] = cursor_pos[1] - size[1]/2;
			if (GetEditor().GetEditorHud().GetTemplateController().GetToolbarController().GroundButton) 
				transform[3] = cursor_pos + transform[1] * vector.Distance(ground_position, transform[3]);				
			else {
				transform[3] = cursor_pos;
				transform[3][1] = transform[3][1] + size[1];		
			} 
		}
		
		
		// This handles all other selected objects
		EditorObjectSet selected_objects = GetEditor().GetSelectedObjects();
		foreach (EditorObject selected_object: selected_objects) {
			
			if (selected_object == target) continue;
			
			vector selected_transform[4];	
			selected_object.GetTransform(selected_transform);
			
			vector object_transform[4];
			target.GetTransform(object_transform);
			
			vector pos_delta = selected_transform[3] - transform[3];
			vector cursor_position_delta = object_transform[3] + selected_transform[3] - transform[3];
		
			float angle_delta = Math.Atan2(pos_delta[0], pos_delta[2]) * Math.RAD2DEG;
			surface_normal = GetGame().SurfaceGetNormal(selected_transform[3][0], selected_transform[3][2]);
			ground_position = GetGroundPosition(selected_transform);
			
			// Handle Z-Only motion
			if (KeyState(KeyCode.KC_LMENU)) {
				// Source object position + delta
				//selected_transform[3][1] = object_transform[3][1] + pos_delta[1];
				selected_transform[3] = ground_position + selected_transform[1] * vector.Distance(cursor_position_delta, ground_position);
				
			// Handle XY Rotation
			} else if (KeyState(KeyCode.KC_LSHIFT)) {
				vector rot_pos;
				angle -= angle_delta;				
				vector new_postion = vector.RotateAroundZero(pos_delta, vector.Up, Math.Cos(angle), Math.Sin(angle));
				selected_object.SetPosition(new_postion + object_transform[3]);
			
			// Handle regular motion for all children
			} else {
				//cursor_position_delta[1] = ground[1];
				if (GetEditor().GetEditorHud().GetTemplateController().GetToolbarController().GroundButton) {
					if (GetEditor().GetEditorHud().GetTemplateController().GetToolbarController().MagnetButton) {
						selected_transform[3] = cursor_position_delta + surface_normal * vector.Distance(ground_position, selected_transform[3]);
					} else {
					
						selected_transform[3] = cursor_position_delta + selected_transform[1] * vector.Distance(ground_position, selected_transform[3]);
					}
				} else {
					selected_transform[3] = cursor_position_delta;				
				} 
				
				selected_transform[0] = "1 0 0";
				selected_transform[1] = "0 1 0";
				selected_transform[2] = "0 0 1";
				selected_object.PlaceOnSurfaceRotated(selected_transform, selected_transform[3], surface_normal[0] * -1, surface_normal[2] * -1, selected_object.LocalAngle * -1, GetEditor().GetEditorHud().GetTemplateController().GetToolbarController().MagnetButton);
			}	
			
		
			selected_object.SetTransform(selected_transform);
			selected_object.Update();
			
		}
	}
}


class MapDragHandler: DragHandler
{
	
	override void OnDragging(out vector transform[4], notnull EditorObject target)
	{		
		vector pos = Editor.CurrentMousePosition;
		transform[3] = pos;
		
		if (GetEditor().GetEditorHud().GetTemplateController().GetToolbarController().GroundButton) {
			vector target_pos = target.GetPosition();
			transform[3][1] = GetGame().SurfaceY(pos[0], pos[2]) + target_pos[1] - GetGame().SurfaceY(target_pos[0], target_pos[2]);
			
		} else {
			transform[3][1] = GetGame().SurfaceY(pos[0], pos[2]) + target.GetSize()[1] / 2;
		}
		
		
		
		
		
		
	}
}



