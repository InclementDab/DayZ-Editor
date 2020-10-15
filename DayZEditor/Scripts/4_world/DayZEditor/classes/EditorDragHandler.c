
class DragHandler
{
	protected Editor m_Editor;
	protected EditorObject m_EditorObject;
	void DragHandler(EditorObject target)
	{
		m_EditorObject = target;
		m_Editor = GetEditor();
	}
	
	void OnDragStart()
	{
		thread _OnDragging();
	}
	
	void OnDragFinish();
	
	private void _OnDragging()
	{
		EditorAction drag_undo = new EditorAction("SetTransform", "SetTransform");
		drag_undo.InsertUndoParameter(m_EditorObject, m_EditorObject.GetTransformArray());
		
		vector transform[4];
		m_EditorObject.GetTransform(transform);
		m_Editor.ObjectInHand = m_EditorObject;
		
		while (GetMouseState(MouseState.LEFT) & MB_PRESSED_MASK && !(GetMouseState(MouseState.RIGHT) & MB_PRESSED_MASK)) {
			OnDragging(transform, m_EditorObject);
			m_EditorObject.SetTransform(transform);
			Sleep(10);
		} 
		
		drag_undo.InsertRedoParameter(m_EditorObject, m_EditorObject.GetTransformArray());
		GetEditor().InsertAction(drag_undo);
		
		m_Editor.ObjectInHand = null;
		OnDragFinish();
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
		int component;
		
		m_Editor.ObjectInHand = target;
		vector cursor_pos = Editor.CurrentMousePosition;
		
		vector size;
		vector ground_position;
		vector surface_normal;	
		
		// Calcuate position of all selected objects
		// this is done first to avoid confusion with an already affected transformation
		EditorObjectMap selected_objects = GetEditor().GetSelectedObjects();
		foreach (EditorObject selected_object: selected_objects) {
			
			if (selected_object == target) continue;			
			vector selected_transform[4];
			selected_object.GetTransform(selected_transform);
			
			size = selected_object.GetSize();
			ground_position = GetGroundPosition(selected_transform);
			surface_normal = GetGame().SurfaceGetNormal(ground_position[0], ground_position[2]);
			
			vector pos_delta = selected_transform[3] - transform[3];

			// Handle Z-Only motion
			if (KeyState(KeyCode.KC_LMENU)) {
				//selected_transform[3] = ground_position + selected_transform[1] * vector.Distance(GetGroundPosition(transform), transform[3]); // - vector.Distance(GetGroundPosition(selected_transform), selected_transform[3])

			// Handle XY Rotation
			} else if (KeyState(KeyCode.KC_LSHIFT)) {
				
				vector rot_pos;
				//angle -= angle_delta;				
				//float angle = Math.Atan2(cursor_delta[0], cursor_delta[2]) * Math.RAD2DEG;	

				if (m_Editor.MagnetMode) {
					vector local_dir = vector.Direction(ground_position, cursor_pos + pos_delta);
					local_dir.Normalize();
					selected_transform[0] = surface_normal * local_dir;
					selected_transform[1] = surface_normal;
					selected_transform[2] = surface_normal * (local_dir * vector.Up);
				} else {
					if (m_Editor.GroundMode) {
						selected_transform[3] = ground_position + selected_transform[1] * vector.Distance(ground_position, selected_transform[3]);
					}
				}
			
			// Handle regular motion for all children
			} else {

				if (!m_Editor.MagnetMode) {
					surface_normal = vector.Up;
				}
				
				vector local_ori = selected_object.GetWorldObject().GetDirection();
				selected_transform[0] = surface_normal * local_ori;
				selected_transform[1] = surface_normal;
				selected_transform[2] = surface_normal * (local_ori * vector.Up);
				
				if (m_Editor.GroundMode) {
					if (m_Editor.MagnetMode) {
						selected_transform[3] = cursor_pos + pos_delta + surface_normal * vector.Distance(ground_position, selected_transform[3]);
					} else {
						selected_transform[3] = cursor_pos + pos_delta + selected_transform[1] * vector.Distance(ground_position, selected_transform[3]);
					}
				} else {						
					selected_transform[3] = cursor_pos + pos_delta;
					selected_transform[3][1] = cursor_pos[1] + size[1]/2;		
				} 
			}	
			
		
			selected_object.SetTransform(selected_transform);			
		}
		
		size = m_EditorObject.GetSize();
		ground_position = GetGroundPosition(transform);
		surface_normal = GetGame().SurfaceGetNormal(ground_position[0], ground_position[2]);
		
		// Handle Z-Only motion
		// Todo will people want this as a keybind?
		if (KeyState(KeyCode.KC_LMENU)) {
			cursor_pos = GetGame().GetCurrentCameraPosition() + GetGame().GetPointerDirection() * vector.Distance(GetGame().GetCurrentCameraPosition(), ground_position);
			cursor_pos[1] = cursor_pos[1] + size[1]/2;
			transform[3] = ground_position + transform[1] * vector.Distance(ground_position, cursor_pos);
		}
		
		// Handle XY Rotation
		else if (KeyState(KeyCode.KC_LSHIFT)) {
			vector cursor_delta = ground_position - Editor.CurrentMousePosition;
			vector delta = m_EditorObject.GetOrientation();
			float ang = Math.Atan2(cursor_delta[0], cursor_delta[2]);
			delta[0] = ang * Math.RAD2DEG;
			delta.RotationMatrixFromAngles(transform);
			
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
			
			if (!m_Editor.MagnetMode) {
				surface_normal = vector.Up;
			}
			
			local_ori = m_EditorObject.GetWorldObject().GetDirection();
			transform[0] = surface_normal * local_ori;
			transform[1] = surface_normal;
			transform[2] = surface_normal * (local_ori * vector.Up);

			if (m_Editor.GroundMode) {
				if (m_Editor.MagnetMode) {
					transform[3] = cursor_pos + surface_normal * vector.Distance(ground_position, transform[3]);				
				} else {
					transform[3] = cursor_pos + transform[1] * vector.Distance(ground_position, transform[3]);
				}
				
			} else {
				transform[3] = cursor_pos;
				transform[3][1] = transform[3][1] + m_EditorObject.GetYDistance();				
				
			} 			
		}
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
		cursor_position = GetGame().GetCurrentCameraPosition() + GetGame().GetPointerDirection() * vector.Distance(GetGame().GetCurrentCameraPosition(), ground);
		cursor_position[1] = cursor_position[1] + object_size[1]/2; // offset building height
		object_transform[3] = ground + object_transform[1] * vector.Distance(ground, cursor_position);
	
	// Handle XY Rotation
	} else if (input.LocalValue("UATurbo")) {
	
		object_transform = { "1 0 0", "0 1 0", "0 0 1", object_transform[3] };
		vector cursor_delta = cursor_position - object_transform[3];
		float angle = Math.Atan2(cursor_delta[0], cursor_delta[2]) * Math.RAD2DEG;	
		target.PlaceOnSurfaceRotated(object_transform, object_transform[3], surface_normal[0] * -1, surface_normal[2] * -1, angle * -1, EditorSettings.MAGNET_PLACEMENT);
		target.LocalAngle = angle;
		
	// Handle regular motion
	} else {			
		
		object_transform[0] = "1 0 0";
		object_transform[1] = "0 1 0";
		object_transform[2] = "0 0 1";
		target.PlaceOnSurfaceRotated(object_transform, object_transform[3], surface_normal[0] * -1, surface_normal[2] * -1, target.LocalAngle * -1, EditorSettings.MAGNET_PLACEMENT);
		
		
		cursor_position[1] = cursor_position[1] - object_size[1]/2;
		if (EditorSettings.MAINTAIN_HEIGHT) 
			if (EditorSettings.MAGNET_PLACEMENT)
				object_transform[3] = cursor_position + surface_normal * vector.Distance(ground, object_transform[3]);				
			else 
				object_transform[3] = cursor_position + object_transform[1] * vector.Distance(ground, object_transform[3]);
			
		else {
			object_transform[3] = cursor_position;
			object_transform[3][1] = object_transform[3][1] + object_size[1];					
		} 
	
		
	}
	

	target.SetTransform(object_transform);
	target.Update();
	
	// This handles all other selected objects
	EditorObjectMap selected_objects = GetObjectManager().GetSelectedObjects();
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


class MapDragHandler: DragHandler
{
	
	override void OnDragging(out vector transform[4], notnull EditorObject target)
	{		
		vector pos = Editor.CurrentMousePosition;
		transform[3] = pos;
		
		if (GetEditor().GroundMode) {
			vector target_pos = target.GetPosition();
			transform[3][1] = GetGame().SurfaceY(pos[0], pos[2]) + target_pos[1] - GetGame().SurfaceY(target_pos[0], target_pos[2]);
			
		} else {
			transform[3][1] = GetGame().SurfaceY(pos[0], pos[2]) + target.GetSize()[1] / 2;
		}
		
		
		
		
		
		
	}
}



