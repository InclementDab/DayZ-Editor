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
		super.UpdateGizmo(dt, gizmo_transform);
		
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
				if (GetEditor().IsShiftDown()) {
					cursor_intersect_xy_local[0] = Math.Round(cursor_intersect_xy_local[0] * 2.5) / 2.5;
				}
				
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
				if (GetEditor().IsShiftDown()) {
					cursor_intersect_xz_local[1] = Math.Round(cursor_intersect_xz_local[1] * 2.5) / 2.5;
				}
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
				if (GetEditor().IsShiftDown()) {
					cursor_intersect_yz_local[2] = Math.Round(cursor_intersect_yz_local[2] * 2.5) / 2.5;
				}
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