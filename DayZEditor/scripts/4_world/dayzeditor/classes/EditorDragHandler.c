
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
			OnDragging(transform);
			m_EditorObject.SetTransform(transform);
			m_EditorObject.Update();
		} else {
			OnDragFinish();
		}
	}
	
	void OnDragging(out vector transform[4]);
	
	
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
	override void OnDragging(out vector transform[4])
	{
		vector begin_pos = GetGame().GetCurrentCameraPosition();
		vector end_pos = begin_pos + GetGame().GetPointerDirection() * 3000;
		vector cursor_pos, contact_dir;
		int component;
		
		DayZPhysics.RaycastRV(begin_pos, end_pos, cursor_pos, contact_dir, component, null, null, null, false, true);
		
				
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
					
			transform = { "1 0 0", "0 1 0", "0 0 1", transform[3] };
			vector cursor_delta = cursor_pos - transform[3];
			float angle = Math.Atan2(cursor_delta[0], cursor_delta[2]) * Math.RAD2DEG;	
			m_EditorObject.PlaceOnSurfaceRotated(transform, ground_position, surface_normal[0] * -1, surface_normal[2] * -1, angle * -1, EditorSettings.MAGNET_PLACEMENT);			
		}
		
		// Handle regular motion
		else {
			
			transform = { "1 0 0", "0 1 0", "0 0 1", transform[3] };
			m_EditorObject.PlaceOnSurfaceRotated(transform, transform[3], surface_normal[0] * -1, surface_normal[2] * -1, m_EditorObject.LocalAngle * -1, EditorSettings.MAGNET_PLACEMENT);
			
			
			cursor_pos[1] = cursor_pos[1] - size[1]/2;
			if (EditorSettings.MAINTAIN_HEIGHT) 
				transform[3] = cursor_pos + transform[1] * vector.Distance(ground_position, transform[3]);				
			else {
				transform[3] = cursor_pos;
				transform[3][1] = transform[3][1] + size[1];		
			} 
			
			//transform[3] = cursor_pos;
			//transform[3][1] = cursor_pos[1] + size[1]/2;
		}
		
		
		
	}
	
	
	/*
	// Handles Marker Drag from Object Base Marker
	void ObjectDragUpdate(notnull EditorObject target)
	{

		Input input = GetGame().GetInput();
		if (input.LocalRelease("UAFire")) {
			EditorEvents.DropInvoke(this, target);
			return;
		}
		
		// Get all object data 
		vector object_size = target.GetSize();
		vector object_transform[4], start_transform[4];
		
		// non-updated version of object_transform
		target.GetTransform(start_transform); 
		target.GetTransform(object_transform);
		
		
		// Raycast ground below object
		set<Object> o;
		vector ground, ground_dir; int component;
		DayZPhysics.RaycastRV(object_transform[3], object_transform[3] + object_transform[1] * -1000, ground, ground_dir, component, o, NULL, target.GetWorldObject(), false, true); // set to ground only

		vector cursor_position = MousePosToRay(o, target.GetWorldObject(), EditorSettings.OBJECT_VIEW_DISTANCE, 0, !EditorSettings.COLLIDE_ON_DRAG);
		vector surface_normal = GetGame().SurfaceGetNormal(ground[0], ground[2]);
		float surface_level = GetGame().SurfaceY(ground[0], ground[2]);
	
		// debug
		Editor.DebugObject0.SetPosition(cursor_position);

		// Handle Z only motion
		
		if (input.LocalValue("UALookAround")) {	
			
		
		// Handle XY Rotation
		} else if (input.LocalValue("UATurbo")) {

		
		} else {			
			
		
		}
		
	
		target.SetTransform(object_transform);
		target.Update();
		
		// This handles all other selected objects
		EditorObjectSet selected_objects = GetObjectManager().GetSelectedObjects();
		foreach (EditorObject selected_object: selected_objects) {
			
			if (selected_object == target) continue;
			
			vector selected_transform[4];	
			selected_object.GetTransform(selected_transform);
			
			vector pos_delta = selected_transform[3] - start_transform[3];
			vector cursor_position_delta = object_transform[3] + selected_transform[3] - start_transform[3];
			vector delta2 = start_transform[3] - selected_transform[3];
		
			float angle_delta = Math.Atan2(pos_delta[0], pos_delta[2]) * Math.RAD2DEG;
			surface_normal = GetGame().SurfaceGetNormal(selected_transform[3][0], selected_transform[3][2]);
			DayZPhysics.RaycastRV(selected_transform[3], selected_transform[3] + selected_transform[1] * -1000, ground, ground_dir, component, o, NULL, selected_object.GetWorldObject(), false, true); // set to ground only

			
			// Handle Z only motion for all children				
			if (input.LocalValue("UALookAround")) {
				// Source object position + delta
				//selected_transform[3][1] = object_transform[3][1] + pos_delta[1];
				selected_transform[3] = ground + selected_transform[1] * vector.Distance(cursor_position_delta, ground);
				
			// Handle XY Rotation for all children
			} else if (input.LocalValue("UATurbo")) {
				vector rot_pos;
				angle -= angle_delta;				
				vector new_postion = vector.RotateAroundZero(pos_delta, vector.Up, Math.Cos(angle), Math.Sin(angle));
				selected_object.SetPosition(new_postion + object_transform[3]);
			
			// Handle regular motion for all children
			} else {
				//cursor_position_delta[1] = ground[1];
				if (EditorSettings.MAINTAIN_HEIGHT) {
					if (EditorSettings.MAGNET_PLACEMENT) {
						selected_transform[3] = cursor_position_delta + surface_normal * vector.Distance(ground, selected_transform[3]);
					} else {
					
						selected_transform[3] = cursor_position_delta + selected_transform[1] * vector.Distance(ground, selected_transform[3]);
					}
				} else {
					selected_transform[3] = cursor_position_delta;				
				} 
				
				selected_transform[0] = "1 0 0";
				selected_transform[1] = "0 1 0";
				selected_transform[2] = "0 0 1";
				selected_object.PlaceOnSurfaceRotated(selected_transform, selected_transform[3], surface_normal[0] * -1, surface_normal[2] * -1, selected_object.LocalAngle * -1, EditorSettings.MAGNET_PLACEMENT);
			}	
			
		
			selected_object.SetTransform(selected_transform);
			selected_object.Update();
			
		}
		
		
		//GlobalTranslationWidget.UpdatePosition();
		//Print(TickCount(starttime) / 1000);
	}
*/
}


class MapDragHandler: DragHandler
{
	
	override void OnDragging(out vector transform[4])
	{
		
		
		
	}
}


