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