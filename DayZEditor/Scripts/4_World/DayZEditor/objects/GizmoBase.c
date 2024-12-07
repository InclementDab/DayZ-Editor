class GizmoInteractionSource: Managed
{
	ref array<vector> Clipping = {};
	LinearColor DefaultColor;
	LinearColor HoverColor;

	void GizmoInteractionSource(notnull array<vector> clipping, LinearColor default_color, LinearColor hover_color = 0xFFFFFF00)
	{
		Clipping.Copy(clipping);
		DefaultColor = default_color;
		HoverColor = hover_color;
	}
	
	bool CollideAABB(Ray ray, vector transform[4], out vector hit_pos)
	{
		vector box_clip_global[2] = {
			Clipping[0].Multiply4(transform),
			Clipping[1].Multiply4(transform),
		};
		
		// r.dir is unit direction vector of ray
		vector dirfrac = Vector(1.0 / ray.Direction[0], 1.0 / ray.Direction[1], 1.0 / ray.Direction[2]);

		// lb is the corner of AABB with minimal coordinates - left bottom, rt is maximal corner
		// r.org is origin of ray
		float t1 = (box_clip_global[0][0] - ray.Position[0]) * dirfrac[0];
		float t2 = (box_clip_global[1][0] - ray.Position[0]) * dirfrac[0];
		float t3 = (box_clip_global[0][1] - ray.Position[1]) * dirfrac[1];
		float t4 = (box_clip_global[1][1] - ray.Position[1]) * dirfrac[1];
		float t5 = (box_clip_global[0][2] - ray.Position[2]) * dirfrac[2];
		float t6 = (box_clip_global[1][2] - ray.Position[2]) * dirfrac[2];

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
}

class EditorGizmo: Managed
{
	const int INTERACTION_CENTER = 0;
	
	// Translation gizmo
	const int INTERACTION_X_AXIS = 1;
	const int INTERACTION_Y_AXIS = 2;
	const int INTERACTION_Z_AXIS = 3;
	
	const int INTERACTION_YZ_PLANE = 4;
	const int INTERACTION_XZ_PLANE = 5;
	const int INTERACTION_XY_PLANE = 6;
	
	// Rotation Gizmo
	const int INTERACTION_YZ_ROTATE = 1;
	const int INTERACTION_XZ_ROTATE = 2;
	const int INTERACTION_XY_ROTATE = 3;
	
	// Scale Gizmo
	const int INTERACTION_X_SCALE = 1;
	const int INTERACTION_Y_SCALE = 2;
	const int INTERACTION_Z_SCALE = 3;
	
	// Box sizes for cursor collision
	const float BOX_WIDTH = 0.125;
	const float BOX_WIDTH_LARGE = 0.1875;
	const float BOX_LENGTH = 2.0;
	
	protected Editor m_Editor;
	protected EntityAI m_Gizmo;
	protected int m_InteractionIndex = -1;
	protected vector m_DragOffset;
	protected ref EditorAction m_RewindAction;
	protected ref map<int, ref GizmoInteractionSource> m_InteractionCollisions = new map<int, ref GizmoInteractionSource>();
	protected ref array<int> m_VisibleSortedInteractions = new array<int>();
	
	void EditorGizmo()
	{
		m_Editor = GetEditor();
		m_Gizmo = EntityAI.Cast(GetGame().CreateObjectEx(GetGizmoMesh(), vector.Zero, ECE_NONE));
		RegisterInteractionClips(m_InteractionCollisions);
	}
		
	void ~EditorGizmo()
	{	
		if (m_Gizmo) {	
			m_Gizmo.Delete();
		}
	}	

	bool IsInteracting()
	{
		return m_InteractionIndex != -1;
	}
	
	protected void PreUpdateGizmo(float dt)
	{
		m_LocalTransformsToTarget = new map<EditorObject, ref array<vector>>();
		foreach (EditorObject additional_drag_target: m_AllSelectedObjects) {
			if (additional_drag_target == m_TopSelectedObject) {
				continue;
			}
			
			vector additional_drag_target_mat[4];
			additional_drag_target.GetTopTransform(additional_drag_target_mat);
			vector inv_additional_drag_target_mat[4];
			vector top_transform_ortho[4];
			copyarray(top_transform_ortho, m_TopTransformOrthogonal);
			Math3D.MatrixInvMultiply4(top_transform_ortho, additional_drag_target_mat, inv_additional_drag_target_mat);
			m_LocalTransformsToTarget[additional_drag_target] = {
				inv_additional_drag_target_mat[0],
				inv_additional_drag_target_mat[1],
				inv_additional_drag_target_mat[2],
				inv_additional_drag_target_mat[3]
			};
		}
	}

	protected void UpdateGizmo(float dt, inout vector gizmo_transform[4])
	{

	}

	protected void PostUpdateGizmo(float dt)
	{
		// Handle all child objects
		foreach (EditorObject selected_object: m_AllSelectedObjects) {
			if (selected_object == m_TopSelectedObject) {
				continue;
			}

			array<vector> dyn_vec_arry = m_LocalTransformsToTarget[selected_object];
			vector local_additional_mat[4] = {
				dyn_vec_arry[0],
				dyn_vec_arry[1],
				dyn_vec_arry[2],
				dyn_vec_arry[3]
			};
			
			Math3D.MatrixOrthogonalize4(local_additional_mat);

			vector output_additional_mat[4];
			Math3D.MatrixMultiply4(m_TopTransformOrthogonal, local_additional_mat, output_additional_mat);
			selected_object.SetTopTransform(output_additional_mat);
		}
	}

	protected ref map<EditorObject, ref array<vector>> m_LocalTransformsToTarget = new map<EditorObject, ref array<vector>>();
	protected ref array<EditorObject> m_AllSelectedObjects = {};
	protected EditorObject m_TopSelectedObject;

	protected vector m_TopTransform[4], m_TopTransformOrthogonal[4], m_CameraTransform[4], m_TopTransformScaledToGizmo[4], m_TopTransformOriginal[4];
	protected float m_CameraFieldOfView;
	protected ref Ray m_CursorRay;
	protected ref Raycast m_CursorRaycast;

	// called by Editor.Update
	void Update(float dt)
	{
		bool doSort = false;
		float colorArr[4];
		int bias = 0;
		
#ifdef DIAG_DEVELOPER
		bool debug_collisions = 0;
		//GetDayZGame().ReloadShape(m_Gizmo);
#endif
		// todo, grabbing this every frame?
		m_AllSelectedObjects = GetEditor().GetSelectedObjectsOrdered();
		m_TopSelectedObject = m_AllSelectedObjects[m_AllSelectedObjects.Count() - 1];
		if (!m_TopSelectedObject || m_AllSelectedObjects.Count() == 0) {
			m_InteractionIndex = -1;
			m_DragOffset = vector.Zero;
			return;
		}
			
		// Cursor raycasts
		m_CursorRay = GetEditor().GetCursorRay();
		m_CameraFieldOfView = GetEditor().GetCamera().GetCurrentFOV();

		// Top transform
		vector top_transform[4];
		m_TopSelectedObject.GetTopTransform(top_transform);
		copyarray(m_TopTransform, top_transform);
		
		vector transform_without_scale[4];
		copyarray(transform_without_scale, top_transform);
		Math3D.MatrixOrthogonalize4(transform_without_scale);
		copyarray(m_TopTransformOrthogonal, transform_without_scale);
				
		vector camera_transform[4];
		GetEditor().GetCamera().GetTransform(camera_transform);
		copyarray(m_CameraTransform, camera_transform);

		// lol @lad
		vector gizmo_transform[4];
		copyarray(gizmo_transform, transform_without_scale);
		gizmo_transform[0] = -gizmo_transform[0];
		gizmo_transform[2] = -gizmo_transform[2];
		
		float gizmo_distance = vector.Distance(top_transform[3], camera_transform[3]);
		float gizmo_scale = gizmo_distance * m_CameraFieldOfView * 0.05;
		vector gizmo_scale_mat[4];
		Math3D.ScaleMatrix(gizmo_scale, gizmo_scale_mat);
		Math3D.MatrixMultiply3(gizmo_scale_mat, gizmo_transform, gizmo_transform);
		
		m_Gizmo.SetTransform(gizmo_transform);

		// Top transform but scaled to le gizmosis
		vector top_transform_scaled_to_gizmo[4];
		copyarray(top_transform_scaled_to_gizmo, transform_without_scale);
		Math3D.MatrixMultiply3(gizmo_scale_mat, top_transform_scaled_to_gizmo, top_transform_scaled_to_gizmo);
		copyarray(m_TopTransformScaledToGizmo, top_transform_scaled_to_gizmo);
		
#ifdef DIAG_DEVELOPER
		foreach (int debug_interaction_index, GizmoInteractionSource debug_clip_info: m_InteractionCollisions) {
			LinearColor dbg_color = debug_clip_info.DefaultColor;
		
			if (m_InteractionIndex == debug_interaction_index) {
				dbg_color = LinearColor.YELLOW;
			}
			
			if (debug_collisions) {
				Shape s = Shape.Create(ShapeType.BBOX, dbg_color, ShapeFlags.TRANSP | ShapeFlags.ONCE | ShapeFlags.ADDITIVE, debug_clip_info.Clipping[0], debug_clip_info.Clipping[1]);
				s.SetMatrix(m_TopTransformScaledToGizmo);
			}
		}
#endif
		
		UAInput interact_input = GetUApi().GetInputByID(UAFire);

		float collide_dist = 0;
		int collide_index = -1;
		vector collision_hit = vector.Zero;
		foreach (int interaction_index, GizmoInteractionSource clip_info: m_InteractionCollisions) {							
			vector hit_pos;	
			bool hit = clip_info.CollideAABB(m_CursorRay, m_TopTransformScaledToGizmo, hit_pos);
			if (!hit) {
				continue;
			}

			// Check for closest collision
			if (collide_index != -1 && vector.Distance(m_CursorRay.Position, hit_pos) > collide_dist) {
				continue;
			}

			collide_index = interaction_index;
			collide_dist = vector.Distance(m_CursorRay.Position, hit_pos);
			collision_hit = hit_pos;
		}
	
		if (m_InteractionIndex == -1 && interact_input.LocalPress()) {
			m_DragOffset = collision_hit.InvMultiply4(m_TopTransformOrthogonal);
			m_InteractionIndex = collide_index;
			copyarray(m_TopTransformOriginal, top_transform);

			// Register rewinds
			m_RewindAction = new EditorAction("SetTransform", "SetTransform");
			foreach (EditorObject selected_rewind_object: m_AllSelectedObjects) {
				m_RewindAction.InsertUndoParameter(selected_rewind_object.GetTransformArray());
			}
		}

		if (!interact_input.LocalValue() && m_InteractionIndex != -1) {
			m_InteractionIndex = -1;
			m_DragOffset = vector.Zero;

			foreach (EditorObject selected_rewind_object2: m_AllSelectedObjects) {
				m_RewindAction.InsertRedoParameter(selected_rewind_object2.GetTransformArray());
			}

			GetEditor().InsertAction(m_RewindAction);
		}
		
		m_VisibleSortedInteractions.Clear();

		PreUpdateGizmo(dt);
		UpdateGizmo(dt, gizmo_transform);
		
#ifdef DIAG_DEVELOPER
		if (m_InteractionCollisions.Count() != m_VisibleSortedInteractions.Count())
		{
			Error("Invalid setup for sorted interactions");
		}
#endif

		foreach (int interaction_index_color : m_VisibleSortedInteractions) {
			GizmoInteractionSource clip_info_color = m_InteractionCollisions[interaction_index_color];
			
			LinearColor color = clip_info_color.DefaultColor;
			if (interaction_index_color == collide_index && m_InteractionIndex == -1) {
				color = LinearColor.YELLOW;
			}
			
			if (interaction_index_color == m_InteractionIndex) {
				color = LinearColor.ORANGE;
			}
			
			if (doSort)
			{
				string materialName = string.Format("DayZEditor/Editor/data/sort_bias_%1", bias);
	
				string rvMaterialName = materialName + ".rvmat";
				m_Gizmo.SetObjectMaterial(interaction_index_color, rvMaterialName);
				m_Gizmo.SetObjectTexture(interaction_index_color, "");
	
				string enfMaterialName = materialName;
				Material mat = GetGame().GetWorld().GetMaterial(enfMaterialName);
				if (mat)
				{
					colorArr[0] = color.GetRed() / 255;
					colorArr[1] = color.GetGreen() / 255;
					colorArr[2] = color.GetBlue() / 255;
					colorArr[3] = color.GetAlpha() / 255;
					
					mat.SetParam("Color", colorArr);
					mat.SetParam("Diffuse", colorArr);
					mat.SetParam("Ambient", colorArr);
					//mat.SetParam("Specular", colorArr);
				}
				else
				{
					Error(string.Format("Material %1 not found", enfMaterialName));
				}
				
				bias++;
			}
			else
			{
				m_Gizmo.SetObjectMaterial(interaction_index_color, "");
				m_Gizmo.SetObjectTexture(interaction_index_color, string.Format("#(argb,8,8,3)color(%1,%2,%3,1.000,co)", color.GetRed() / 255.0, color.GetGreen() / 255.0, color.GetBlue() / 255.0));
			}
		}
		
		m_TopSelectedObject.SetTopTransform(m_TopTransform);
		m_Gizmo.SetTransform(gizmo_transform);
		m_Gizmo.Update();

		PostUpdateGizmo(dt);
	}
	
	// Each clipping bounds must be vector[2]
	void RegisterInteractionClips(inout notnull map<int, ref GizmoInteractionSource> clipping_infos)
	{
		clipping_infos[INTERACTION_CENTER] = new GizmoInteractionSource({
			-Vector(BOX_WIDTH_LARGE, BOX_WIDTH_LARGE, BOX_WIDTH_LARGE),
			Vector(BOX_WIDTH_LARGE, BOX_WIDTH_LARGE, BOX_WIDTH_LARGE)
		}, LinearColor.WHITE);
	}

	string GetGizmoMesh()
	{
		return "";
	}
}

class EditorTranslationGizmo: EditorGizmo
{
	override void RegisterInteractionClips(inout notnull map<int, ref GizmoInteractionSource> clipping_infos)
	{
		super.RegisterInteractionClips(clipping_infos);
		
		float BOX_WIDTH_HALF = BOX_WIDTH / 2;
		float BOX_LENGTH_HALF = BOX_LENGTH / 3;
		
		// X
		clipping_infos[INTERACTION_X_AXIS] = new GizmoInteractionSource({
			Vector(BOX_LENGTH, -BOX_WIDTH, -BOX_WIDTH),
			Vector(0, BOX_WIDTH, BOX_WIDTH)
		}, LinearColor.RED);
		
		// Y
		clipping_infos[INTERACTION_Y_AXIS] = new GizmoInteractionSource({
			Vector(-BOX_WIDTH, -0, -BOX_WIDTH),
			Vector(BOX_WIDTH, BOX_LENGTH, BOX_WIDTH)
		}, LinearColor.GREEN);
		
		// Z
		clipping_infos[INTERACTION_Z_AXIS] = new GizmoInteractionSource({
			Vector(-BOX_WIDTH, -BOX_WIDTH, BOX_LENGTH),
			Vector(BOX_WIDTH, BOX_WIDTH, 0)
		}, LinearColor.BLUE);
		
		clipping_infos[INTERACTION_XZ_PLANE] = new GizmoInteractionSource({
			Vector(BOX_WIDTH, BOX_WIDTH_HALF, BOX_WIDTH), // BOX_WIDTH[0], BOX_WIDTH[2] to keep it from intersecting with others
			Vector(BOX_LENGTH_HALF, -BOX_WIDTH_HALF, BOX_LENGTH_HALF)
		}, LinearColor.GREEN);
		
		clipping_infos[INTERACTION_XY_PLANE] = new GizmoInteractionSource({
			Vector(BOX_WIDTH, BOX_WIDTH, BOX_WIDTH_HALF),
			Vector(BOX_LENGTH_HALF, BOX_LENGTH_HALF, -BOX_WIDTH_HALF)
		}, LinearColor.BLUE);
		
		clipping_infos[INTERACTION_YZ_PLANE] = new GizmoInteractionSource({
			Vector(BOX_WIDTH_HALF, BOX_WIDTH, BOX_WIDTH),
			Vector(-BOX_WIDTH_HALF, BOX_LENGTH_HALF, BOX_LENGTH_HALF)
		}, LinearColor.RED);
	}
	
	override void UpdateGizmo(float dt, inout vector gizmo_transform[4])
	{
		m_VisibleSortedInteractions.Insert(INTERACTION_CENTER);
		m_VisibleSortedInteractions.Insert(INTERACTION_X_AXIS);
		m_VisibleSortedInteractions.Insert(INTERACTION_Y_AXIS);
		m_VisibleSortedInteractions.Insert(INTERACTION_Z_AXIS);
		m_VisibleSortedInteractions.Insert(INTERACTION_XY_PLANE);
		m_VisibleSortedInteractions.Insert(INTERACTION_XZ_PLANE);
		m_VisibleSortedInteractions.Insert(INTERACTION_YZ_PLANE);
		
		vector cursor_intersect = vector.Zero;
		switch (m_InteractionIndex) {
			// center interaction
			case INTERACTION_CENTER: {
				Plane3D camera_forward_plane = Plane3D(m_CameraTransform[2], m_TopTransformOrthogonal[3]);
				cursor_intersect = camera_forward_plane.Intersect(m_CursorRay);
				break;
			}
						
			case INTERACTION_X_AXIS: {
				vector plane_xy_norm = m_TopTransformOrthogonal[0] * (m_CameraTransform[2] * m_TopTransformOrthogonal[0]);
				Plane3D xy_plane = Plane3D(plane_xy_norm, m_TopTransformOrthogonal[3]);
				cursor_intersect = xy_plane.Intersect(m_CursorRay);
				
				vector cursor_intersect_xy_local = cursor_intersect.InvMultiply4(m_TopTransformOrthogonal) - m_DragOffset;
				cursor_intersect_xy_local[1] = 0;
				cursor_intersect_xy_local[2] = 0;
				cursor_intersect = cursor_intersect_xy_local.Multiply4(m_TopTransformOrthogonal);
				break;
			}
			
			case INTERACTION_Y_AXIS: {				
				vector plane_xz_norm = m_TopTransformOrthogonal[1] * (m_CameraTransform[2] * m_TopTransformOrthogonal[1]);
				Plane3D xz_plane = Plane3D(plane_xz_norm, m_TopTransformOrthogonal[3]);
				cursor_intersect = xz_plane.Intersect(m_CursorRay);
				vector cursor_intersect_xz_local = cursor_intersect.InvMultiply4(m_TopTransformOrthogonal) - m_DragOffset;
				cursor_intersect_xz_local[0] = 0;
				cursor_intersect_xz_local[2] = 0;
				cursor_intersect = cursor_intersect_xz_local.Multiply4(m_TopTransformOrthogonal);
				break;
			}
			
			case INTERACTION_Z_AXIS: {
				vector plane_yz_norm = m_TopTransformOrthogonal[2] * (m_CameraTransform[2] * m_TopTransformOrthogonal[2]);
				Plane3D yz_plane = Plane3D(plane_yz_norm, m_TopTransformOrthogonal[3]);
				cursor_intersect = yz_plane.Intersect(m_CursorRay);
				vector cursor_intersect_yz_local = cursor_intersect.InvMultiply4(m_TopTransformOrthogonal) - m_DragOffset;
				cursor_intersect_yz_local[0] = 0;
				cursor_intersect_yz_local[1] = 0;
				cursor_intersect = cursor_intersect_yz_local.Multiply4(m_TopTransformOrthogonal);
				break;
			}
			
			case INTERACTION_XY_PLANE: {
				Plane3D xy_plane2 = Plane3D(m_TopTransformOrthogonal[2], m_TopTransformOrthogonal[3]);
				cursor_intersect = xy_plane2.Intersect(m_CursorRay);
				vector cursor_intersect_xy_local2 = (cursor_intersect.InvMultiply4(m_TopTransformOrthogonal) - m_DragOffset);
				cursor_intersect_xy_local2[2] = 0;
				cursor_intersect = cursor_intersect_xy_local2.Multiply4(m_TopTransformOrthogonal);
				break;
			}
			
			case INTERACTION_XZ_PLANE: {
				Plane3D xz_plane2 = Plane3D(m_TopTransformOrthogonal[1], m_TopTransformOrthogonal[3]);
				cursor_intersect = xz_plane2.Intersect(m_CursorRay);
				vector cursor_intersect_xz_local2 = (cursor_intersect.InvMultiply4(m_TopTransformOrthogonal) - m_DragOffset);
				cursor_intersect_xz_local2[1] = 0;
				cursor_intersect = cursor_intersect_xz_local2.Multiply4(m_TopTransformOrthogonal);
				break;
			}
			
			case INTERACTION_YZ_PLANE: {
				Plane3D yz_plane2 = Plane3D(m_TopTransformOrthogonal[0], m_TopTransformOrthogonal[3]);
				cursor_intersect = yz_plane2.Intersect(m_CursorRay);
				vector cursor_intersect_yz_local2 = (cursor_intersect.InvMultiply4(m_TopTransformOrthogonal) - m_DragOffset);
				cursor_intersect_yz_local2[0] = 0;
				cursor_intersect = cursor_intersect_yz_local2.Multiply4(m_TopTransformOrthogonal);
				break;
			}
		}
		
		if (cursor_intersect != vector.Zero) {
			m_TopTransform[3] = cursor_intersect;
			m_TopTransformOrthogonal[3] = cursor_intersect;
			gizmo_transform[3] = cursor_intersect;
		}
	}

	override string GetGizmoMesh()
	{
		return "GizmoTranslation";
	}
}

class EditorRotationGizmo: EditorGizmo
{
	override void UpdateGizmo(float dt, inout vector gizmo_transform[4])
	{
		m_VisibleSortedInteractions.Insert(INTERACTION_CENTER);
		m_VisibleSortedInteractions.Insert(INTERACTION_XZ_ROTATE);
		m_VisibleSortedInteractions.Insert(INTERACTION_YZ_ROTATE);
		m_VisibleSortedInteractions.Insert(INTERACTION_XY_ROTATE);
		
		switch (m_InteractionIndex) {
			case INTERACTION_XZ_ROTATE: {
				Plane3D xz_plane2 = Plane3D(m_TopTransform[1], m_TopTransform[3]);
				vector xz_intersect = xz_plane2.Intersect(m_CursorRay);
				if (vector.Distance(m_TopTransform[3], xz_intersect) > 0.001) {
					vector cursor_intersect_dir = vector.Direction(m_TopTransform[3], xz_intersect);
					Debug.DrawArrow(m_TopTransform[3], m_TopTransform[3] + cursor_intersect_dir * 10, 1, LinearColor.BLUE, ShapeFlags.ONCE);
	
					vector cursor_dir_mat[4];
					cursor_intersect_dir.Normalize();
					if (cursor_intersect_dir.Length() > 0 && Math.AbsFloat(vector.Dot(cursor_intersect_dir, m_TopTransform[1])) != 1) {
						cursor_dir_mat = {
							m_TopTransform[1] * cursor_intersect_dir,
							m_TopTransform[1],
							cursor_intersect_dir,
							m_TopTransform[3]
						};

						Math3D.MatrixOrthogonalize4(cursor_dir_mat);
						vector output_mat[4];
						//Math3D.MatrixMultiply4(cursor_dir_mat, m_TopTransform, output_mat);
	
						//Math3D.MatrixMultiply3(scale_matrix, cursor_dir_mat, cursor_dir_mat);
						copyarray(m_TopTransform, cursor_dir_mat);
					}
				}
				
				break;
			}

			case INTERACTION_YZ_ROTATE: {
				Plane3D yz_plane2 = Plane3D(m_TopTransform[0], m_TopTransform[3]);
				vector yz_intersect = yz_plane2.Intersect(m_CursorRay);				
				if (vector.Distance(m_TopTransform[3], yz_intersect) > 0.001) {
					vector cursor_yz_intersect_dir = vector.Direction(m_TopTransform[3], yz_intersect);

					vector cursor_yz_dir_mat[4];
					cursor_yz_intersect_dir.Normalize();
					if (cursor_yz_intersect_dir.Length() > 0 && Math.AbsFloat(vector.Dot(cursor_yz_intersect_dir, m_TopTransform[0])) != 1) {
						cursor_yz_dir_mat = {
							m_TopTransform[0],
							cursor_yz_intersect_dir,
							m_TopTransform[0] * cursor_yz_intersect_dir,
							m_TopTransform[3]
						};

						Math3D.MatrixOrthogonalize4(cursor_yz_dir_mat);
	
						//Math3D.MatrixMultiply3(scale_matrix, cursor_yz_dir_mat, cursor_yz_dir_mat);
						copyarray(m_TopTransform, cursor_yz_dir_mat);
					}
				}
				
				break;
			}

			case INTERACTION_XY_ROTATE: {				
				Plane3D xy_plane2 = Plane3D(m_TopTransform[2], m_TopTransform[3]);
				vector xy_intersect = xy_plane2.Intersect(m_CursorRay);
				Shape.CreateSphere(-1, ShapeFlags.ONCE, xy_intersect, 0.5);				
				if (vector.Distance(m_TopTransform[3], xy_intersect) > 0.001) {
					vector cursor_xy_intersect_dir = vector.Direction(m_TopTransform[3], xy_intersect);

					vector cursor_xy_dir_mat[4];
					cursor_xy_intersect_dir.Normalize();
					if (cursor_xy_intersect_dir.Length() > 0 && Math.AbsFloat(vector.Dot(cursor_xy_intersect_dir, m_TopTransform[0])) != 1) {
						cursor_xy_dir_mat = {
							cursor_xy_intersect_dir,
							m_TopTransform[2] * cursor_xy_intersect_dir,
							m_TopTransform[2],
							m_TopTransform[3]
						};

						Math3D.MatrixOrthogonalize4(cursor_xy_dir_mat);
	
						//Math3D.MatrixMultiply3(scale_matrix, cursor_xy_dir_mat, cursor_xy_dir_mat);
						copyarray(m_TopTransform, cursor_xy_dir_mat);
					}
				}
				
				break;
			}
		}
	}
	
	override void RegisterInteractionClips(inout notnull map<int, ref GizmoInteractionSource> clipping_infos)	
	{
		super.RegisterInteractionClips(clipping_infos);
		
		float BOX_WIDTH_HALF = 0.025;
		float BOX_LENGTH_HALF = BOX_LENGTH / 3;
		
		clipping_infos[INTERACTION_XZ_ROTATE] = new GizmoInteractionSource({
			Vector(BOX_WIDTH, BOX_WIDTH_HALF, BOX_WIDTH), // BOX_WIDTH[0], BOX_WIDTH[2] to keep it from intersecting with others
			Vector(BOX_LENGTH, -BOX_WIDTH_HALF, BOX_LENGTH)
		}, LinearColor.GREEN);
		
		clipping_infos[INTERACTION_XY_ROTATE] = new GizmoInteractionSource({
			Vector(BOX_WIDTH, BOX_WIDTH, BOX_WIDTH_HALF),
			Vector(BOX_LENGTH, BOX_LENGTH, -BOX_WIDTH_HALF)
		}, LinearColor.BLUE);
		
		clipping_infos[INTERACTION_YZ_ROTATE] = new GizmoInteractionSource({
			Vector(BOX_WIDTH_HALF, BOX_WIDTH, BOX_WIDTH),
			Vector(-BOX_WIDTH_HALF, BOX_LENGTH, BOX_LENGTH)
		}, LinearColor.RED);
	}

	override string GetGizmoMesh()
	{
		return "GizmoRotation";
	}
}

class EditorScaleGizmo: EditorGizmo
{
	override void UpdateGizmo(float dt, inout vector gizmo_transform[4])
	{
		m_VisibleSortedInteractions.Insert(INTERACTION_CENTER);
		m_VisibleSortedInteractions.Insert(INTERACTION_X_SCALE);
		m_VisibleSortedInteractions.Insert(INTERACTION_Y_SCALE);
		m_VisibleSortedInteractions.Insert(INTERACTION_Z_SCALE);
		
		vector scale_matrix[3];
		vector top_transform[4];
		copyarray(top_transform, m_TopTransformOrthogonal);
		switch (m_InteractionIndex) {
			case INTERACTION_X_SCALE: {
				vector plane_xy_norm = m_TopTransform[0] * (m_CameraTransform[2] * m_TopTransform[0]);
				Plane3D xy_plane = Plane3D(plane_xy_norm, m_TopTransform[3]);
				vector xy_intersect = xy_plane.Intersect(m_CursorRay);
				vector scale_xy_dir = xy_intersect.InvMultiply4(m_TopTransformOrthogonal);
				scale_xy_dir[1] = 0;
				scale_xy_dir[2] = 0;
				
				float scale_norm = m_TopTransformOriginal[0].Length() * (scale_xy_dir.Length() / m_DragOffset.Length());
				float scale_round = Math.Round(Math.AbsFloat(scale_norm) * 10) / 10;
				scale_round = Math.Clamp(scale_round, 0.1, 10);

				Math3D.ScaleMatrix(scale_round, scale_matrix);
				Math3D.MatrixMultiply3(scale_matrix, top_transform, top_transform);
				top_transform[3] = m_TopTransformOrthogonal[3];
				copyarray(m_TopTransform, top_transform);
				break;
			}
			case INTERACTION_Y_SCALE: {
				vector plane_y_norm = m_TopTransform[1] * (m_CameraTransform[2] * m_TopTransform[1]);
				Plane3D xz_plane = Plane3D(plane_y_norm, m_TopTransform[3]);
				vector xz_intersect = xz_plane.Intersect(m_CursorRay);
				vector scale_xz_dir = xz_intersect.InvMultiply4(m_TopTransformOrthogonal);
				scale_xz_dir[0] = 0;
				scale_xz_dir[2] = 0;
				
				float scale_norm1 = m_TopTransformOriginal[1].Length() * (scale_xz_dir.Length() / m_DragOffset.Length());
				float scale_round1 = Math.Round(Math.AbsFloat(scale_norm1) * 10) / 10;
				scale_round1 = Math.Clamp(scale_round1, 0.1, 10);

				Math3D.ScaleMatrix(scale_round1, scale_matrix);
				Math3D.MatrixMultiply3(scale_matrix, top_transform, top_transform);
				top_transform[3] = m_TopTransformOrthogonal[3];
				copyarray(m_TopTransform, top_transform);
				break;
			}
			case INTERACTION_Z_SCALE: {
				vector plane_z_norm = m_TopTransform[1] * (m_CameraTransform[2] * m_TopTransform[1]);
				Plane3D z_plane = Plane3D(plane_z_norm, m_TopTransform[3]);
				vector z_intersect = z_plane.Intersect(m_CursorRay);
				vector scale_z_dir = z_intersect.InvMultiply4(m_TopTransformOrthogonal);
				scale_z_dir[1] = 0;
				scale_z_dir[2] = 0;
				
				float scale_norm2 = m_TopTransformOriginal[0].Length() * (scale_z_dir.Length() / m_DragOffset.Length());
				float scale_round2 = Math.Round(Math.AbsFloat(scale_norm2) * 10) / 10;
				scale_round2 = Math.Clamp(scale_round2, 0.1, 10);

				Math3D.ScaleMatrix(scale_round2, scale_matrix);
				copyarray(top_transform, m_TopTransformOrthogonal);
				Math3D.MatrixMultiply3(scale_matrix, top_transform, top_transform);
				top_transform[3] = m_TopTransformOrthogonal[3];
				copyarray(m_TopTransform, top_transform);
				break;
			}
		}
	}
	
	override void RegisterInteractionClips(inout notnull map<int, ref GizmoInteractionSource> clipping_infos)
	{
		// no origin ball
		//super.RegisterInteractionClips(clipping_infos);
		
		float BOX_WIDTH_HALF = BOX_WIDTH / 2;
		float BOX_LENGTH_HALF = BOX_LENGTH / 3;
		
		// X
		clipping_infos[INTERACTION_X_SCALE] = new GizmoInteractionSource({
			Vector(BOX_LENGTH - 0.5, 0, 0) - Vector(BOX_WIDTH, BOX_WIDTH, BOX_WIDTH) * 2,
			Vector(BOX_LENGTH - 0.5, 0, 0) + Vector(BOX_WIDTH, BOX_WIDTH, BOX_WIDTH) * 2
		}, LinearColor.RED);
		
		// Y
		clipping_infos[INTERACTION_Y_SCALE] = new GizmoInteractionSource({
			Vector(0, BOX_LENGTH - 0.5, 0) - Vector(BOX_WIDTH, BOX_WIDTH, BOX_WIDTH) * 2,
			Vector(0, BOX_LENGTH - 0.5, 0) + Vector(BOX_WIDTH, BOX_WIDTH, BOX_WIDTH) * 2
		}, LinearColor.GREEN);
		
		// Z
		clipping_infos[INTERACTION_Z_SCALE] = new GizmoInteractionSource({
			Vector(0, 0, BOX_LENGTH - 0.5) - Vector(BOX_WIDTH, BOX_WIDTH, BOX_WIDTH) * 2,
			Vector(0, 0, BOX_LENGTH - 0.5) + Vector(BOX_WIDTH, BOX_WIDTH, BOX_WIDTH) * 2
		}, LinearColor.BLUE);
	}

	override string GetGizmoMesh()
	{
		return "GizmoScale";
	}
}