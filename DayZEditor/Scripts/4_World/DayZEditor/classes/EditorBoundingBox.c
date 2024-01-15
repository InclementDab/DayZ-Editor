class EditorBoundingBox: Managed
{
	static void Create(notnull Object object)
	{
		vector size = ObjectGetSize(object);
		vector clip_info[2];
		object.ClippingInfo(clip_info);
		vector position = AverageVectors(clip_info[0], clip_info[1]);
		
		vector line_centers[12]; 
		vector line_verticies[8];
				
		line_verticies[0] = clip_info[0];
		line_verticies[1] = Vector(clip_info[0][0], clip_info[0][1], clip_info[1][2]);
		line_verticies[2] = Vector(clip_info[1][0], clip_info[0][1], clip_info[1][2]);
		line_verticies[3] = Vector(clip_info[1][0], clip_info[0][1], clip_info[0][2]);		
		line_verticies[4] = Vector(clip_info[1][0], clip_info[1][1], clip_info[0][2]);
		line_verticies[5] = clip_info[1];
		line_verticies[6] = Vector(clip_info[0][0], clip_info[1][1], clip_info[1][2]);
		line_verticies[7] = Vector(clip_info[0][0], clip_info[1][1], clip_info[0][2]);
				
		line_centers[0] = AverageVectors(line_verticies[0], line_verticies[1]);
		line_centers[1] = AverageVectors(line_verticies[0], line_verticies[3]);
		line_centers[2] = AverageVectors(line_verticies[0], line_verticies[7]);
		line_centers[3] = AverageVectors(line_verticies[4], line_verticies[7]);
		line_centers[4] = AverageVectors(line_verticies[6], line_verticies[7]);
		
		line_centers[5] = AverageVectors(line_verticies[1], line_verticies[2]);
		line_centers[6] = AverageVectors(line_verticies[1], line_verticies[6]);
		line_centers[7] = AverageVectors(line_verticies[3], line_verticies[2]);
		line_centers[8] = AverageVectors(line_verticies[3], line_verticies[4]);
		
		line_centers[9] = AverageVectors(line_verticies[5], line_verticies[2]);
		line_centers[10] = AverageVectors(line_verticies[5], line_verticies[4]);		
		line_centers[11] = AverageVectors(line_verticies[5], line_verticies[6]);
		
		for (int i = 0; i < 12; i++) {
			vector transform[4];			
			transform[3] = line_centers[i];
			
			for (int j = 0; j < 3; j++) {
				transform[j][j] = ((position[j] == line_centers[i][j]) * size[j]) + BOUNDING_BOX_THICKNESS;						
			}
			 
			Object bbox_line = GetGame().CreateObjectEx("BoundingBoxBase", line_centers[i], ECE_LOCAL);
			bbox_line.SetTransform(transform);
			bbox_line.Update();	
			
			object.AddChild(bbox_line, -1, true);
			object.Update();
		}
	}
	
	static void Destroy(notnull Object object)
	{
		Object child = Object.Cast(object.GetChildren());
		while (child) {
			if (child.GetType() == "BoundingBoxBase") {
				child.Delete();
			}
			
			// technically objects arent deleted instantly like you think they are. so this is fine
			child = Object.Cast(child.GetSibling());
		}
	}
}