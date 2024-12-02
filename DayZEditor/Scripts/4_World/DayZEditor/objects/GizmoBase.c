class EditorGizmo: Managed
{
	const int INTERACTION_CENTER = 0;
	
	// Translation gizmo
	const int INTERACTION_X_AXIS = 1;
	const int INTERACTION_Y_AXIS = 2;
	const int INTERACTION_Z_AXIS = 3;
	
	const int INTERACTION_XY_PLANE = 4;
	const int INTERACTION_XZ_PLANE = 5;
	const int INTERACTION_YZ_PLANE = 6;
	
	// Rotation Gizmo
	const int INTERACTION_XY_ROTATE = 1;
	const int INTERACTION_XZ_ROTATE = 2;
	const int INTERACTION_YZ_ROTATE = 3;
	
	// Scale Gizmo
	const int INTERACTION_X_SCALE = 1;
	const int INTERACTION_Y_SCALE = 2;
	const int INTERACTION_Z_SCALE = 3;
	
	// Box sizes for cursor collision
	const float BOX_WIDTH = 0.125;
	const float BOX_WIDTH_LARGE = 0.165;
	const float BOX_LENGTH = 2.0;
	
	protected Editor m_Editor;
	protected EditorObject m_EditorObject;
	protected EditorObjectMap m_AllObjects;
	protected Object m_Gizmo;
	protected int m_InteractionIndex = -1;
	protected vector m_DragOffset;
	protected ref EditorAction m_RewindAction;
	protected ref map<int, ref array<vector>> m_InteractionCollisions = new map<int, ref array<vector>>();
	
	void EditorGizmo()
	{
		m_Gizmo = GetGame().CreateObjectEx(GetGizmoMesh(), vector.Zero, ECE_NONE);
		RegisterInteractionClips(m_InteractionCollisions);
	}
		
	void ~EditorGizmo()
	{	
		if (m_Gizmo) {	
			m_Gizmo.Delete();
		}
	}	
	
	void Initialize(notnull Editor editor, EditorObject editor_object, EditorObjectMap all_objects)
	{		
		m_Editor = editor;
		m_EditorObject = editor_object;
		m_AllObjects = all_objects;
	}

	bool CollideAABB(Ray ray, vector clip_info[2], out vector hit_pos)
	{
		// r.dir is unit direction vector of ray
		vector dirfrac = Vector(1.0 / ray.Direction[0], 1.0 / ray.Direction[1], 1.0 / ray.Direction[2]);

		// lb is the corner of AABB with minimal coordinates - left bottom, rt is maximal corner
		// r.org is origin of ray
		float t1 = (clip_info[0][0] - ray.Position[0]) * dirfrac[0];
		float t2 = (clip_info[1][0] - ray.Position[0]) * dirfrac[0];
		float t3 = (clip_info[0][1] - ray.Position[1]) * dirfrac[1];
		float t4 = (clip_info[1][1] - ray.Position[1]) * dirfrac[1];
		float t5 = (clip_info[0][2] - ray.Position[2]) * dirfrac[2];
		float t6 = (clip_info[1][2] - ray.Position[2]) * dirfrac[2];

		float tmin = Math.Max(Math.Max(Math.Min(t1, t2), Math.Min(t3, t4)), Math.Min(t5, t6));
		float tmax = Math.Min(Math.Min(Math.Max(t1, t2), Math.Max(t3, t4)), Math.Max(t5, t6));
		float t = 0;
		// if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
		if (tmax < 0) {
			t = tmax;
			return false;
		}

		// if tmin > tmax, ray doesn't intersect AABB
		if (tmin > tmax) {
			t = tmax;
			return false;
		}

		t = tmin;
		hit_pos = ray.Position + ray.Direction.Normalized() * t;
		return true;
	}

	bool IsInteracting()
	{
		return m_InteractionIndex != -1;
	}
			
	// called by Editor.Update
	void Update(float dt)
	{		
#ifdef DIAG_DEVELOPER
		bool debug_collisions = 1;
#endif
		// todo, grabbing this every frame?
		EditorObjectMap all_editor_objects = GetEditor().GetSelectedObjects();
		EditorObject editor_object = all_editor_objects.GetElement(0);
		if (!editor_object || all_editor_objects.Count() == 0) {
			m_InteractionIndex = -1;
			m_DragOffset = vector.Zero;
			return;
		}
	
		vector top_transform[4];
		editor_object.GetTopTransform(top_transform);
		Ray cursor_ray = GetEditor().GetCursorRay();
		Raycast cursor_raycast = cursor_ray.PerformRaycast();
		
		// lol @lad
		vector gizmo_transform[4];
		copyarray(gizmo_transform, top_transform);
		gizmo_transform[0] = -gizmo_transform[0];
		gizmo_transform[2] = -gizmo_transform[2];
				
		m_Gizmo.SetTransform(gizmo_transform);

#ifdef DIAG_DEVELOPER
		foreach (int debug_interaction_index, array<vector> debug_clip_info: m_InteractionCollisions) {
			LinearColor dbg_color = LinearColor.FLORAL_WHITE;
		
			switch (debug_interaction_index) {
				case 1: {
					dbg_color = LinearColor.RED;
					break;
				}

				case 2: {
					dbg_color = LinearColor.GREEN;
					break;
				}

				case 3: {
					dbg_color = LinearColor.BLUE;
					break;
				}
			}

			if (m_InteractionIndex == debug_interaction_index) {
				dbg_color = LinearColor.YELLOW;
			}
			
			if (debug_collisions) {
				Shape s = Shape.Create(ShapeType.BBOX, dbg_color, ShapeFlags.TRANSP | ShapeFlags.ONCE | ShapeFlags.ADDITIVE, debug_clip_info[0], debug_clip_info[1]);
				s.SetMatrix(top_transform);
			}
		}
#endif
		
		UAInput interact_input = GetUApi().GetInputByID(UAFire);
		if (interact_input.LocalPress() && m_InteractionIndex == -1) {
			float collide_dist = 0;
			int collide_index = -1;
			vector collision_hit = vector.Zero;
			foreach (int interaction_index, array<vector> clip_info: m_InteractionCollisions) {							
				vector hit_pos;
				vector box_clip_global[2] = {
					clip_info[0].Multiply4(top_transform),
					clip_info[1].Multiply4(top_transform),
				};
	
				bool hit = CollideAABB(cursor_ray, box_clip_global, hit_pos);
				if (!hit) {
					continue;
				}
	
				if (collide_index != -1 && vector.Distance(cursor_ray.Position, hit_pos) > collide_dist) {
					continue;
				}
	
				collide_index = interaction_index;
				collide_dist = vector.Distance(cursor_ray.Position, hit_pos);
				collision_hit = hit_pos;
			}
			
			m_DragOffset = collision_hit.InvMultiply4(top_transform);
			m_InteractionIndex = collide_index;
			
			// Register rewinds
			m_RewindAction = new EditorAction("SetTransform", "SetTransform");
			foreach (EditorObject selected_rewind_object: all_editor_objects) {
				m_RewindAction.InsertUndoParameter(selected_rewind_object.GetTransformArray());
			}
		}

		if (!interact_input.LocalValue() && m_InteractionIndex != -1) {
			m_InteractionIndex = -1;
			m_DragOffset = vector.Zero;

			foreach (EditorObject selected_rewind_object2: all_editor_objects) {
				m_RewindAction.InsertRedoParameter(selected_rewind_object2.GetTransformArray());
			}

			GetEditor().InsertAction(m_RewindAction);
		}
	}
	
	// Each clipping bounds must be vector[2]
	void RegisterInteractionClips(inout notnull map<int, ref array<vector>> clipping_infos)
	{
		clipping_infos[INTERACTION_CENTER] = {
			-Vector(BOX_WIDTH_LARGE, BOX_WIDTH_LARGE, BOX_WIDTH_LARGE),
			Vector(BOX_WIDTH_LARGE, BOX_WIDTH_LARGE, BOX_WIDTH_LARGE)
		};
	}

	string GetGizmoMesh()
	{
		return "";
	}
}

class EditorTranslationGizmo: EditorGizmo
{
	override void RegisterInteractionClips(inout notnull map<int, ref array<vector>> clipping_infos)
	{
		super.RegisterInteractionClips(clipping_infos);
		
		float BOX_WIDTH_HALF = BOX_WIDTH / 2;
		float BOX_LENGTH_HALF = BOX_LENGTH / 3;
		
		// X
		clipping_infos[INTERACTION_X_AXIS] = {
			Vector(BOX_LENGTH, -BOX_WIDTH, -BOX_WIDTH),
			Vector(0, BOX_WIDTH, BOX_WIDTH)
		};
		
		// Y
		clipping_infos[INTERACTION_Y_AXIS] = {
			Vector(-BOX_WIDTH, -0, -BOX_WIDTH),
			Vector(BOX_WIDTH, BOX_LENGTH, BOX_WIDTH)
		};
		
		// Z
		clipping_infos[INTERACTION_Z_AXIS] = {
			Vector(-BOX_WIDTH, -BOX_WIDTH, BOX_LENGTH),
			Vector(BOX_WIDTH, BOX_WIDTH, 0)
		};
		
		clipping_infos[INTERACTION_XZ_PLANE] = {
			Vector(BOX_WIDTH, BOX_WIDTH_HALF, BOX_WIDTH), // BOX_WIDTH[0], BOX_WIDTH[2] to keep it from intersecting with others
			Vector(BOX_LENGTH_HALF, -BOX_WIDTH_HALF, BOX_LENGTH_HALF)
		};
		
		clipping_infos[INTERACTION_XY_PLANE] = {
			Vector(BOX_WIDTH, BOX_WIDTH, BOX_WIDTH_HALF),
			Vector(BOX_LENGTH_HALF, BOX_LENGTH_HALF, -BOX_WIDTH_HALF)
		};
		
		clipping_infos[INTERACTION_YZ_PLANE] = {
			Vector(BOX_WIDTH_HALF, BOX_WIDTH, BOX_WIDTH),
			Vector(-BOX_WIDTH_HALF, BOX_LENGTH_HALF, BOX_LENGTH_HALF)
		};
	}
	
	override void Update(float dt)
	{
		super.Update(dt);
		
		// todo: copied from above?
		EditorObjectMap all_editor_objects = GetEditor().GetSelectedObjects();
		EditorObject editor_object = all_editor_objects.GetElement(0);
		if (!editor_object || all_editor_objects.Count() == 0) {
			return;
		}
		
		if (m_InteractionIndex == -1) {
			return;
		}
					
		vector top_transform[4];
		editor_object.GetTopTransform(top_transform);
		Ray cursor_ray = GetEditor().GetCursorRay();
		Raycast cursor_raycast = cursor_ray.PerformRaycast();

		vector transform_without_scale[4];
		copyarray(transform_without_scale, top_transform);
		Math3D.MatrixOrthogonalize4(transform_without_scale);
		
		vector camera_transform[4];
		GetEditor().GetCamera().GetTransform(camera_transform);
				
		//GetDayZGame().ReloadShape(m_Gizmo);
	
		// debug
		//Shape.CreateSphere(LinearColor.GREEN, ShapeFlags.ONCE, m_DragOffset.Multiply4(top_transform), 0.1);
		//Shape.Create(ShapeType.BBOX, LinearColor.BLUE, ShapeFlags.ONCE, m_InteractionCollisions[m_InteractionIndex][0].Multiply4(top_transform), m_InteractionCollisions[m_InteractionIndex][1].Multiply4(top_transform));
		
		
		map<EditorObject, ref array<vector>> local_transforms_to_target = new map<EditorObject, ref array<vector>>();
		foreach (EditorObject additional_drag_target: all_editor_objects) {
			if (additional_drag_target == editor_object) {
				continue;
			}
			
			vector additional_drag_target_mat[4];
			additional_drag_target.GetTopTransform(additional_drag_target_mat);
			vector inv_additional_drag_target_mat[4];
			Math3D.MatrixInvMultiply4(transform_without_scale, additional_drag_target_mat, inv_additional_drag_target_mat);
			local_transforms_to_target[additional_drag_target] = {
				inv_additional_drag_target_mat[0],
				inv_additional_drag_target_mat[1],
				inv_additional_drag_target_mat[2],
				inv_additional_drag_target_mat[3]
			};
		}
		
		vector cursor_intersect = vector.Zero;
		switch (m_InteractionIndex) {
			// center interaction
			case INTERACTION_CENTER: {
				Plane3D camera_forward_plane = Plane3D(camera_transform[2], top_transform[3]);
				cursor_intersect = camera_forward_plane.Intersect(cursor_ray);
				break;
			}
			
			case INTERACTION_X_AXIS: {
				Plane3D xy_plane = Plane3D(top_transform[1], top_transform[3]);
				cursor_intersect = xy_plane.Intersect(cursor_ray);
				vector cursor_intersect_xy_local = cursor_intersect.InvMultiply4(top_transform) - m_DragOffset;
				cursor_intersect_xy_local[1] = 0;
				cursor_intersect_xy_local[2] = 0;
				cursor_intersect = cursor_intersect_xy_local.Multiply4(top_transform);
				break;
			}
			
			case INTERACTION_Y_AXIS: {
				Plane3D xz_plane = Plane3D(top_transform[2], top_transform[3]);
				cursor_intersect = xz_plane.Intersect(cursor_ray);
				vector cursor_intersect_xz_local = cursor_intersect.InvMultiply4(top_transform) - m_DragOffset;
				cursor_intersect_xz_local[0] = 0;
				cursor_intersect_xz_local[2] = 0;
				cursor_intersect = cursor_intersect_xz_local.Multiply4(top_transform);
				break;
			}
			
			case INTERACTION_Z_AXIS: {
				Plane3D yz_plane = Plane3D(top_transform[0], top_transform[3]);
				cursor_intersect = yz_plane.Intersect(cursor_ray);
				vector cursor_intersect_yz_local = cursor_intersect.InvMultiply4(top_transform) - m_DragOffset;
				cursor_intersect_yz_local[0] = 0;
				cursor_intersect_yz_local[1] = 0;
				cursor_intersect = cursor_intersect_yz_local.Multiply4(top_transform);
				break;
			}
			
			case INTERACTION_XY_PLANE: {
				Plane3D xy_plane2 = Plane3D(top_transform[2], top_transform[3]);
				cursor_intersect = xy_plane2.Intersect(cursor_ray);
				vector cursor_intersect_xy_local2 = (cursor_intersect.InvMultiply4(top_transform) - m_DragOffset);
				cursor_intersect_xy_local2[2] = 0;
				cursor_intersect = cursor_intersect_xy_local2.Multiply4(top_transform);
				break;
			}
			
			case INTERACTION_XZ_PLANE: {
				Plane3D xz_plane2 = Plane3D(top_transform[1], top_transform[3]);
				cursor_intersect = xz_plane2.Intersect(cursor_ray);
				vector cursor_intersect_xz_local2 = (cursor_intersect.InvMultiply4(top_transform) - m_DragOffset);
				cursor_intersect_xz_local2[1] = 0;
				cursor_intersect = cursor_intersect_xz_local2.Multiply4(top_transform);
				break;
			}
			
			case INTERACTION_YZ_PLANE: {
				Plane3D yz_plane2 = Plane3D(top_transform[0], top_transform[3]);
				cursor_intersect = yz_plane2.Intersect(cursor_ray);
				vector cursor_intersect_yz_local2 = (cursor_intersect.InvMultiply4(top_transform) - m_DragOffset);
				cursor_intersect_yz_local2[0] = 0;
				cursor_intersect = cursor_intersect_yz_local2.Multiply4(top_transform);
				break;
			}
		}
		
		if (cursor_intersect != vector.Zero) {
			top_transform[3] = cursor_intersect;
			transform_without_scale[3] = cursor_intersect;
			
			// Handle all child objects
			foreach (EditorObject selected_object: all_editor_objects) {
				if (selected_object == editor_object) {
					continue;
				}
	
				array<vector> dyn_vec_arry = local_transforms_to_target[selected_object];
				vector local_additional_mat[4] = {
					dyn_vec_arry[0],
					dyn_vec_arry[1],
					dyn_vec_arry[2],
					dyn_vec_arry[3]
				};
				
				Math3D.MatrixOrthogonalize4(local_additional_mat);
	
				vector output_additional_mat[4];
				Math3D.MatrixMultiply4(transform_without_scale, local_additional_mat, output_additional_mat);
				selected_object.SetTopTransform(output_additional_mat);
			}
			
			editor_object.SetTopTransform(top_transform);
		}
	}

	override string GetGizmoMesh()
	{
		return "GizmoTranslation";
	}
}