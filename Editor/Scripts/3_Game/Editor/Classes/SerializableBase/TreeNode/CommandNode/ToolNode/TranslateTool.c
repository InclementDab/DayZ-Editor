class TranslateTool: ToolNode
{
	protected vector m_CursorAside = vector.Aside;
	protected ref Raycast m_StartPosition;
	
	override bool Update(float dt, Raycast raycast)
	{
		Input input = GetGame().GetInput();
		array<TreeNode> selected_nodes = GetEditor().GetSelectedNodes();
		
		if (input.LocalPress_ID(UAFire) && selected_nodes.Count() > 0) {
			m_StartPosition = raycast;
			return false;
		}
		
		if (input.LocalHold_ID(UAFire) && selected_nodes.Count() > 0) {
			foreach (TreeNode selected_node: selected_nodes) {
				ObjectNode editor_object_cast = ObjectNode.Cast(selected_node);
				if (!editor_object_cast) {
					continue;
				}
										
				vector transform[4];
				editor_object_cast.GetBaseTransform(transform);
				
				// Held distance placing
				if (KeyState(KeyCode.KC_LMENU)) {
					Debug.DrawSphere(raycast.Source.Position, vector.Distance(raycast.Source.Position, transform[3]), COLOR_RED, ShapeFlags.ADDITIVE | ShapeFlags.WIREFRAME | ShapeFlags.ONCE);
					
					vector v3 = transform[1] * raycast.Source.Direction;					
					float dist_z = vector.Dot(((raycast.Source.Position - transform[3]) * transform[1]), v3) / v3.LengthSq();

					float d1 = vector.Dot(vector.Up, raycast.Source.Direction);
					vector x = Math.Cos(d1) * (raycast.Source.Position - transform[3]);
					
					vector pos = raycast.Source.Position + raycast.Source.Direction * dist_z;
					
					transform = { transform[0], transform[1], transform[2], pos };
					editor_object_cast.SetBaseTransform(transform);
				} 
				

				if (KeyState(KeyCode.KC_LSHIFT)) {
					
					vector new_forward = vector.Direction(transform[3], raycast.Bounce.Position).Normalized();
					
					transform = { (transform[1] * new_forward).Normalized(), transform[1], new_forward, transform[3] };
					
					editor_object_cast.SetBaseTransform(transform);
					/*
					vector p1 = Vector(2, 0, 2).Multiply4(transform);
					vector p2 = Vector(-2, 0, -2).Multiply4(transform);
					
					Shape.Create(ShapeType.BBOX, COLOR_GREEN, ShapeFlags.WIREFRAME | ShapeFlags.ONCE | ShapeFlags.TRANSP | ShapeFlags.ADDITIVE, p1, p2);
					
					float d = 1;
					float t = -(vector.Dot(vector.Up, raycast.Source.Position) + transform[3][1]) / vector.Dot(vector.Up, raycast.Source.Direction);
					
					vector result = raycast.Source.Position + raycast.Source.Direction * t;
					
					*/
				}
				
				
				// Any distance placing
				else {
					transform = { m_CursorAside, raycast.Bounce.Direction, m_CursorAside * raycast.Bounce.Direction, raycast.Bounce.Position };
					editor_object_cast.SetBaseTransform(transform);
					return false;
				}
			}
		}

		if (input.LocalRelease_ID(UAFire) && selected_nodes.Count() > 0) {
			foreach (TreeNode selected_node_synch: selected_nodes) {
				ObjectNode editor_object_sync = ObjectNode.Cast(selected_node_synch);
				if (!editor_object_sync) {
					continue;
				}
				
				editor_object_sync.Synchronize();
			}
			
			return false;
		}			
		
				
		return true;
	}
	
	override array<string> GetXorSelections()
	{
		return { "Scale", "Rotate" };
	}
}