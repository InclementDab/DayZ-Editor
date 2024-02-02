class ObjectViewWorld: ObjectView
{
	override void Update(float dt)
	{
		vector base_position = m_ObjectNode.GetBasePoint();
		
		Raycast raycast = m_ObjectNode.GetEditor().GetCamera().PerformCursorRaycast(m_ObjectNode.GetObject());
				
		// Dragging
		if (m_ObjectNode.HasState(TreeNodeState.DRAGGING) && raycast) {
			//raycast.Debug();
			//Shape.CreateArrow(m_StartPosition.Bounce.Position, raycast.Bounce.Position, 1, COLOR_BLACK, ShapeFlags.ONCE);
			
			Input input = GetGame().GetInput();
			vector transform[4];
			m_ObjectNode.GetBaseTransform(transform);
				
			if (KeyState(KeyCode.KC_LSHIFT)) {
				
				Plane face = m_ObjectNode.GetBoundingFace(ETransformationAxis.BOTTOM);
				face.Debug("Cursor intersection", transform);
				
				vector point = face.Intersect(raycast.Source, transform);
				GetDayZGame().DebugDrawText("Intersection", point, 1);
				
				vector new_forward = (point - transform[3]).Normalized();
				vector aside = (transform[1] * new_forward).Normalized();
				
				transform = { aside, transform[1], aside * transform[1], transform[3] };
				
				
				Shape.CreateMatrix(transform);
				
				//Math3D.MatrixOrthogonalize3(transform);
				//Print(transform);
				m_ObjectNode.SetBaseTransform(transform);
				
				//vector p1 = Vector(2, 0, 2).Multiply4(transform);
				//vector p2 = Vector(-2, 0, -2).Multiply4(transform);
				
				//Shape.Create(ShapeType.BBOX, COLOR_GREEN, ShapeFlags.WIREFRAME | ShapeFlags.ONCE | ShapeFlags.TRANSP | ShapeFlags.ADDITIVE, p1, p2);
				
			
			}
			
						// Held distance placing
			else if (KeyState(KeyCode.KC_LMENU)) {
				Debug.DrawSphere(raycast.Source.Position, vector.Distance(raycast.Source.Position, transform[3]), COLOR_RED, ShapeFlags.ADDITIVE | ShapeFlags.WIREFRAME | ShapeFlags.ONCE);
				
				vector v3 = vector.Up * raycast.Source.Direction;
				
				DbgUI.Text(v3.ToString());
				Shape.CreateArrow(raycast.Source.Position + raycast.Source.Direction,  raycast.Source.Position + raycast.Source.Direction + v3, 0.5, COLOR_BLUE, ShapeFlags.ONCE);
				
				float dist_z = vector.Dot(((raycast.Source.Position - transform[3]) * vector.Up), v3) / v3.LengthSq();
				DbgUI.Text(vector.Dot(((raycast.Source.Position - transform[3]) * vector.Up), v3).ToString());
				DbgUI.Text(dist_z.ToString());
				
				transform = { transform[0], transform[1], transform[2], raycast.Source.Position + raycast.Source.Direction * dist_z };
				m_ObjectNode.SetBaseTransform(transform);
			} 
			
			// Any distance placing
			else {
				float delta_y = transform[3][1] - raycast.Bounce.Position[1];
				
				
				transform = { m_CursorAside, raycast.Bounce.Direction, m_CursorAside * raycast.Bounce.Direction, raycast.Bounce.Position };				
				m_ObjectNode.SetBaseTransform(transform);
			}
		}
		
		base_position = GetGame().GetScreenPos(m_ObjectNode.GetBasePoint());
		m_LayoutRoot.SetPos(base_position[0] - 12.5, base_position[1] - 12.5);
		m_LayoutRoot.Show(base_position[2] > 0);
	}
}