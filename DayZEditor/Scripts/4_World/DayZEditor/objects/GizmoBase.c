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
		bool doSort = 0;
		float colorArr[4];
		int bias = 0;
		
#ifdef DIAG_DEVELOPER
		bool debug_collisions = 0;
		//GetDayZGame().ReloadShape(m_Gizmo);
#endif
		
		m_AllSelectedObjects = GetEditor().GetSelectedObjectsOrdered();
		m_TopSelectedObject = m_AllSelectedObjects[m_AllSelectedObjects.Count() - 1];
		if (!m_TopSelectedObject || m_AllSelectedObjects.Count() == 0) {
			//m_InteractionIndex = -1;
			//m_DragOffset = vector.Zero;
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
		
#ifdef DIAG_DEVELOPER
		foreach (int debug_interaction_index, GizmoInteractionSource debug_clip_info: m_InteractionCollisions) {
			LinearColor dbg_color = debug_clip_info.DefaultColor;
		
			if (m_InteractionIndex == debug_interaction_index) {
				dbg_color = LinearColor.YELLOW;
			}
			
			if (collide_index == debug_interaction_index) {
				dbg_color = LinearColor.ORANGE;
			}
			
			if (debug_collisions) {
				Shape s = Shape.Create(ShapeType.BBOX, dbg_color, ShapeFlags.TRANSP | ShapeFlags.ONCE | ShapeFlags.ADDITIVE, debug_clip_info.Clipping[0], debug_clip_info.Clipping[1]);
				s.SetMatrix(m_TopTransformScaledToGizmo);
			}
		}
#endif
	
		if (m_InteractionIndex == -1 && interact_input.LocalPress() && !GetWidgetUnderCursor()) {
			m_DragOffset = collision_hit.InvMultiply4(m_TopTransformOrthogonal);
			m_InteractionIndex = collide_index;
			copyarray(m_TopTransformOriginal, top_transform);

			// Register rewinds
			m_RewindAction = new EditorAction("SetTransform", "SetTransform");
			foreach (EditorObject selected_rewind_object: m_AllSelectedObjects) {
				m_RewindAction.InsertUndoParameter(selected_rewind_object.GetTransformArray());
			}
		}

		if (interact_input.LocalRelease() && m_InteractionIndex != -1) {
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
		
		foreach (int interaction_index_color, GizmoInteractionSource clip_info_color: m_InteractionCollisions) {
			
			if (!clip_info_color) {
				continue;
			}
			
			LinearColor color = clip_info_color.DefaultColor;
			if (interaction_index_color == collide_index && m_InteractionIndex == -1) {
				color = LinearColor.YELLOW;
			}
			
			if (interaction_index_color == m_InteractionIndex) {
				color = LinearColor.ORANGE;
			}
			
			m_Gizmo.SetObjectMaterial(interaction_index_color, "");
			m_Gizmo.SetObjectTexture(interaction_index_color, string.Format("#(argb,8,8,3)color(%1,%2,%3,1.000,co)", color.GetRed() / 255.0, color.GetGreen() / 255.0, color.GetBlue() / 255.0));
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