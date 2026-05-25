class EditorWorldObject: Managed
{
	protected vector m_LineCenters[12]; 
	protected vector m_LineVerticies[8];
	protected vector m_BoundingCenter;
	protected vector m_VectorBasePoint;
	protected vector m_ClippingCenter;
	
	protected EntityAI m_WorldEntity;
	protected Object m_WorldObject;
		
	protected ref array<ref EditorSnapPoint> m_SnapPoints = {};
	protected ref array<ref EditorSnapPlane> m_SnapPlanes = {};
	
	Object GetWorldObject()
	{
		return m_WorldObject;
	}

	void ~EditorWorldObject()
	{
		GetGame().ObjectDelete(m_WorldObject);
	}
	
	void SetWorldObject(notnull Object object)
	{
		m_WorldObject = object;
		m_WorldEntity = EntityAI.Cast(m_WorldObject);
		
		if (!m_WorldObject) {
			EditorLog.Warning("SetWorldObject was null!");
			return;
		}
		
		vector clip_info[2];
		ClippingInfo(clip_info);
		m_LineVerticies[0] = clip_info[0];
		m_LineVerticies[1] = Vector(clip_info[0][0], clip_info[0][1], clip_info[1][2]);
		m_LineVerticies[2] = Vector(clip_info[1][0], clip_info[0][1], clip_info[1][2]);
		m_LineVerticies[3] = Vector(clip_info[1][0], clip_info[0][1], clip_info[0][2]);		
		m_LineVerticies[4] = Vector(clip_info[1][0], clip_info[1][1], clip_info[0][2]);
		m_LineVerticies[5] = clip_info[1];
		m_LineVerticies[6] = Vector(clip_info[0][0], clip_info[1][1], clip_info[1][2]);
		m_LineVerticies[7] = Vector(clip_info[0][0], clip_info[1][1], clip_info[0][2]);
				
		m_LineCenters[0] = AverageVectors(m_LineVerticies[0], m_LineVerticies[1]);
		m_LineCenters[1] = AverageVectors(m_LineVerticies[0], m_LineVerticies[3]);
		m_LineCenters[2] = AverageVectors(m_LineVerticies[0], m_LineVerticies[7]);
		m_LineCenters[3] = AverageVectors(m_LineVerticies[4], m_LineVerticies[7]);
		m_LineCenters[4] = AverageVectors(m_LineVerticies[6], m_LineVerticies[7]);
		
		m_LineCenters[5] = AverageVectors(m_LineVerticies[1], m_LineVerticies[2]);
		m_LineCenters[6] = AverageVectors(m_LineVerticies[1], m_LineVerticies[6]);
		m_LineCenters[7] = AverageVectors(m_LineVerticies[3], m_LineVerticies[2]);
		m_LineCenters[8] = AverageVectors(m_LineVerticies[3], m_LineVerticies[4]);
		
		m_LineCenters[9] = AverageVectors(m_LineVerticies[5], m_LineVerticies[2]);
		m_LineCenters[10] = AverageVectors(m_LineVerticies[5], m_LineVerticies[4]);		
		m_LineCenters[11] = AverageVectors(m_LineVerticies[5], m_LineVerticies[6]);
		
		vector base_point = AverageVectors(AverageVectors(m_LineVerticies[0], m_LineVerticies[1]), AverageVectors(m_LineVerticies[2], m_LineVerticies[3]));
		m_VectorBasePoint = base_point;
		m_BoundingCenter = m_WorldObject.GetBoundingCenter();
		m_ClippingCenter = AverageVectors(clip_info[0], clip_info[1]);
		
		/*
		for (int i = 0; i < 12; i++) {
			EditorSnapPoint snap_point = new EditorSnapPoint(this);
			Math3D.MatrixIdentity4(snap_point.Transform);
			snap_point.Transform[3] = m_LineCenters[i];
			m_SnapPoints.Insert(snap_point);
		}*/
						
		// Todo: For snapping planes. this algorithm isnt functional. and im sleepy hungry n high
		vector mat[3] = {
			"1 0 0",
			"0 1 0"
			"0 0 1",
		};
		for (int j = 0; j < 6; j++) {            
            int k = j / 3;
            int l = j % 3;
												
			vector box_plane_clip = clip_info[k];
            vector box_plane_position = box_plane_clip;// + m_BoundingCenter;// + Position; // may need to be center of clip                            
            vector test_clipping[2];
            copyarray(test_clipping, clip_info);
			
            if (j >= 3) {
                test_clipping[1][l] = test_clipping[0][l];
            } else {
                test_clipping[0][l] = test_clipping[1][l];
            }
                            
            for (int m = 0; m < 3; m++) {
                if (l != m) {                    
                    box_plane_position[m] = 0;//m_BoundingCenter[m]; //Position[m];
                }
            }
						
			vector box_plane_normal = mat[l] * ((k * 2) - 1);
			EditorSnapPlane plane = new EditorSnapPlane(this, new Plane(box_plane_position, box_plane_normal), test_clipping);
			m_SnapPlanes.Insert(plane);
		}
	}
	
	void ClippingInfo(out vector clip_info[2]) 
	{ 
		vector min, max;
		if (m_WorldObject.IsItemBase()) {
			m_WorldObject.GetActionComponentMinMax(m_WorldObject.GetViewGeometryLevel(), 0, min, max);
			clip_info = { min, max };
		} else {
			m_WorldObject.ClippingInfo(clip_info); 
		}		
	}
	
	void GetTransform(out vector mat[4]) 
	{ 
		if (m_WorldObject) {
			m_WorldObject.GetTransform(mat); 
		}
	}
	
	void SetTransform(vector mat[4])
	{		
		if (m_WorldObject) {
			m_WorldObject.SetTransform(mat);
		}
	}
	
	array<ref EditorSnapPoint> GetSnapPoints()
	{
		return m_SnapPoints;
	}
	
	array<ref EditorSnapPlane> GetSnapPlanes()
	{
		return m_SnapPlanes;
	}
	
	EntityAI GetWorldEntity()
	{
		return m_WorldEntity;
	}
	
	vector GetClippingCenter()
	{
		return m_ClippingCenter;
	}
	
	static Object CreateObject(string type, vector position = "0 0 0", vector orientation = "0 0 0", float scale = 1)
	{
		if (type == string.Empty) {
			return null;
		}

		Object object;
		if (type.Contains(".p3d")) {
			string formatted_path = SystemPath.Format(type);
			formatted_path.Replace("\\", "/"); // static object creation likes this
			if (formatted_path[0] == "/") {
				formatted_path = formatted_path.Substring(1, formatted_path.Length() - 1);
			}
			
			object = GetGame().CreateStaticObjectUsingP3D(formatted_path, position, orientation, scale, true);
		}
		else
		{
			object = GetGame().CreateObjectEx(type, position, ECE_LOCAL | ECE_SETUP | ECE_UPDATEPATHGRAPH | ECE_CREATEPHYSICS | ECE_NOLIFETIME | ECE_NOPERSISTENCY_CHAR | ECE_NOPERSISTENCY_WORLD | ECE_INITAI);
		}

		if (!object)
		{
			EditorLog.Warning("EditorWorldObject: Invalid Object %1", type);
			return null;
		}

		object.SetPosition(position);
		object.SetOrientation(orientation);
		object.SetScale(scale);
		object.SetFlags(EntityFlags.VISIBLE, true);
		object.Update();
		
		// Needed for AI Placement			
		EntityAI entity_ai;
		if (Class.CastTo(entity_ai, object))
		{
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(entity_ai.DisableSimulation, 15, 0, true);
			//entity_ai.DisableSimulation(true);

			// weeeeeeee
			if (GetEditor().GetSettings().SpawnItemsWithAttachments && (entity_ai.GetInventory().GetCargo() || entity_ai.GetInventory().GetAttachmentSlotsCount() > 0))
			{
				entity_ai.OnDebugSpawn();
			}
		}

		return object;
	}
}