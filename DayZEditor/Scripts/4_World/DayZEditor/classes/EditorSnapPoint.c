class EditorSnapPoint: ScriptedEntity
{
	static const float RESOLUTION = 0.02;
	
	void EditorSnapPoint()
	{
		SetCollisionSphere(0.125);
	}
	
	bool IsOccupied()
	{
		array<EditorSnapPoint> results = {};
		vector size = Vector(RESOLUTION * 3, RESOLUTION * 3, RESOLUTION * 3);
		array<EntityAI> entities = {};
		DayZPlayerUtils.SceneGetEntitiesInBox(GetWorldPosition() - size, GetWorldPosition() + size, entities);
		foreach (EntityAI entity: entities) {			
			EditorSnapPoint snap_point = EditorSnapPoint.Cast(entity);	
			if (!snap_point || snap_point == this) {
				continue;
			}
			
			if (snap_point.GetParent() == GetParent() || !snap_point.GetParent() || !GetParent()) {
				continue;
			}
						
			return true;
		}
		
		return false;
	}
	
	//@ this function is carrying HARD
	array<EditorSnapPoint> GetAttachments()
	{
		array<EditorSnapPoint> results = {};
		vector size = Vector(RESOLUTION * 3, RESOLUTION * 3, RESOLUTION * 3);
		array<EntityAI> entities = {};
		DayZPlayerUtils.SceneGetEntitiesInBox(GetWorldPosition() - size, GetWorldPosition() + size, entities);
		foreach (EntityAI entity: entities) {
			EditorSnapPoint snap_point = EditorSnapPoint.Cast(entity);
			if (!snap_point || snap_point == this) {
				continue;
			}
			
			if (results.Find(snap_point) != -1) {
				continue;
			}
			
			if (!snap_point.GetParent() || snap_point.GetParent() == GetParent()) {
				continue;
			}
			
			results.Insert(snap_point);
		}
		
		return results;
	}
		
	float Dot(notnull EditorSnapPoint snap_point)
	{
		vector snap_point_world_transform[4];
		snap_point.GetTransform(snap_point_world_transform);
		
		vector world_transform[4];
		GetTransform(world_transform);
		
		float dot_value = vector.Distance(snap_point_world_transform[3], world_transform[3]) / 2.0;
		for (int i = 0; i < 3; i++) {
			dot_value += 1 - vector.Dot(snap_point_world_transform[i], world_transform[i]);
		}		
		
		return dot_value;
	}

	vector GetWorldForward()
	{
		vector mat[4];
		GetParent().GetTransform(mat);
		return mat[2];
	}
	
	vector GetWorldUp()
	{
		vector mat[4];
		GetParent().GetTransform(mat);
		return mat[1];
	}
		
	void SnapTo(notnull EditorSnapPoint anchor_point, out vector mat[4], bool reverse)
	{		
		vector anchor_world_transform[4];
		anchor_point.GetTransform(anchor_world_transform);
		Shape.CreateMatrix(anchor_world_transform);	
		vector ident[4] = {
			"1 0 0",
			"0 1 0",
			"0 0 1",
			vector.Zero
		};
		
		// Flipping the matrix first
		vector matrix_reverser[4];
		Math3D.YawPitchRollMatrix(Vector(0, 180 + reverse * 180, 180 + reverse * 180), matrix_reverser);
		
		vector coupling_matrix[4];
		Math3D.MatrixMultiply4(anchor_world_transform, matrix_reverser, coupling_matrix);
		
		vector source_inverse[4];	
		
		vector matrix[4];
		GetLocalTransform(matrix);
		
		Math3D.MatrixInvMultiply4(matrix, ident, source_inverse);
		Math3D.MatrixMultiply4(coupling_matrix, source_inverse, mat);
	}
		
	static array<EditorSnapPoint> GetInBox(vector position, float size)
	{
		vector max = position + Vector(size, size, size);
		vector min = position - Vector(size, size, size);
		
		array<EntityAI> entities = {};
		DayZPlayerUtils.SceneGetEntitiesInBox(min, max, entities);
		array<EditorSnapPoint> results = {};
		foreach (EntityAI entity: entities) {
			EditorSnapPoint result = EditorSnapPoint.Cast(entity);
			if (result && result.GetParent()) {
				results.Insert(result);
			}
		}
		
		return results;
	}
}
