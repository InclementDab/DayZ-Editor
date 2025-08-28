class EditorRotationGizmo: EditorGizmo
{
	override void UpdateGizmo(float dt, inout vector gizmo_transform[4])
	{		
		m_VisibleSortedInteractions.Insert(INTERACTION_CENTER);
		m_VisibleSortedInteractions.Insert(INTERACTION_XZ_ROTATE);
		m_VisibleSortedInteractions.Insert(INTERACTION_YZ_ROTATE);
		m_VisibleSortedInteractions.Insert(INTERACTION_XY_ROTATE);
		
		float scale = m_TopTransform[1].Length();
		vector scale_mat[3];
		Math3D.ScaleMatrix(scale, scale_mat);
				
		switch (m_InteractionIndex) {
			case INTERACTION_XZ_ROTATE: {
				Plane3D xz_plane2 = Plane3D(m_TopTransform[1], m_TopTransform[3]);
				vector xz_intersect = xz_plane2.Intersect(m_CursorRay);				
				if (vector.Distance(m_TopTransform[3], xz_intersect) > 0.001) {
					vector cursor_intersect_dir = vector.Direction(m_TopTransform[3], xz_intersect);
	
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
	
						copyarray(m_TopTransform, cursor_dir_mat);
						Math3D.MatrixMultiply3(scale_mat, m_TopTransform, m_TopTransform);
						
						copyarray(m_TopTransformOrthogonal, cursor_dir_mat);
						//Math3D.YawPitchRollMatrix(Math3D.MatrixToAngles(m_TopTransform) - m_DragRotationOffset, m_TopTransform);		
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
						Math3D.MatrixMultiply3(scale_mat, m_TopTransform, m_TopTransform);
						
						copyarray(m_TopTransformOrthogonal, cursor_yz_dir_mat);
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
						Math3D.MatrixMultiply3(scale_mat, m_TopTransform, m_TopTransform);
						
						copyarray(m_TopTransformOrthogonal, cursor_xy_dir_mat);
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
		float BOX_LENGTH_HALF = BOX_LENGTH / 2;
		
		clipping_infos[INTERACTION_XZ_ROTATE] = new GizmoInteractionSource({
			Vector(-BOX_LENGTH_HALF, BOX_WIDTH_HALF, -BOX_LENGTH_HALF) * 2, // BOX_WIDTH[0], BOX_WIDTH[2] to keep it from intersecting with others
			Vector(BOX_LENGTH_HALF, -BOX_WIDTH_HALF, BOX_LENGTH_HALF) * 2
		}, LinearColor.GREEN);
		
		clipping_infos[INTERACTION_XY_ROTATE] = new GizmoInteractionSource({
			Vector(-BOX_LENGTH_HALF, -BOX_LENGTH_HALF, BOX_WIDTH_HALF) * 2,
			Vector(BOX_LENGTH_HALF, BOX_LENGTH_HALF, -BOX_WIDTH_HALF) * 2
		}, LinearColor.BLUE);
		
		clipping_infos[INTERACTION_YZ_ROTATE] = new GizmoInteractionSource({
			Vector(BOX_WIDTH_HALF, -BOX_LENGTH_HALF, -BOX_LENGTH_HALF) * 2,
			Vector(-BOX_WIDTH_HALF, BOX_LENGTH_HALF, BOX_LENGTH_HALF) * 2
		}, LinearColor.RED);
	}

	override string GetGizmoMesh()
	{
		return "GizmoRotation";
	}
}