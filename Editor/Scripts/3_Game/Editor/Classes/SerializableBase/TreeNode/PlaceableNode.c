class PlaceableNode: NamedNode
{
	vector Offset[4] = {
		"1 0 0",
		"0 1 0",
		"0 0 1",
		"0 0 0"
	};
	
	ref ObjectNode Placing;
		
	override void EUpdate(float dt)
	{
		super.EUpdate(dt);
		
		if (!Placing) {
			return;
		}
		
		Input input = GetGame().GetInput();
		EditorNode editor = EditorNode.Cast(FindAncestor(EditorNode));
		Raycast raycast = editor.GetCamera().PerformCursorRaycast(Placing.GetObject());
		if (!raycast) {
			return;
		}
		
		/*
		if (input.GetInputByID(UAVehicleTurbo).LocalPress()) {
			
		}*/
		vector raycast_orthogonal[4] = { 
			(raycast.Source.Direction * raycast.Bounce.Direction).Normalized(), 
			raycast.Bounce.Direction, 
			raycast.Source.Direction, 
			raycast.Source.Position 
		};
		
		
		//Math3D.MatrixOrthogonalize4(raycast_orthogonal);	
		
		vector rotation_mat[3];
		Math3D.MatrixIdentity3(rotation_mat);
		
		
		if (input.LocalPress_ID(UAZoomInOptics)) {
			Math3D.YawPitchRollMatrix(Vector(-15, 0, 0), rotation_mat);
			Math3D.MatrixMultiply3(Offset, rotation_mat, Offset);
		}
		
		if (input.LocalPress_ID(UAZoomOutOptics)) {
			Math3D.YawPitchRollMatrix(Vector(15, 0, 0), rotation_mat);
			Math3D.MatrixMultiply3(Offset, rotation_mat, Offset);
		}
				
		Math3D.MatrixMultiply4(raycast_orthogonal, Offset, raycast_orthogonal);
		
		//Shape.CreateMatrix(raycast_orthogonal);
		
		//m_CursorAside = m_CursorAside.Multiply3(rotation_mat);
		
		//Print(Placing.Count());
		//vector transform[4] = { m_CursorAside, raycast.Bounce.Direction, m_CursorAside * raycast.Bounce.Direction, raycast.Bounce.Position };
		Placing.SetBaseTransform(raycast_orthogonal);
	
		if (input.LocalPress_ID(UAFire)) {
			editor.InsertHistory(Placing, null);
			editor.GetLayers().Add(Placing);
			
			Placing.AddState(NodeState.ACTIVE);
			Placing.SetSynchDirty();
			Placing = null;
			
			// remove it from placing
			editor.GetHud().PlaySound(EditorSounds.PLOP);
			
			if (KeyState(KeyCode.KC_LSHIFT)) {
				OnInteract(NodeInteractType.PRESS);
			} else {
				RemoveState(NodeState.ACTIVE);
			}
			
			GetUApi().SupressNextFrame(true);
		}
		
		if (input.LocalPress_ID(UATempRaiseWeapon)) {
			delete Placing;
		}
	}
	
	override void OnStateChanged(NodeState node_state, bool state)
	{
		super.OnStateChanged(node_state, state);
	
		if (node_state.IsActive()) {	
			if (state) {
				vector matrix[4];
				Math3D.MatrixIdentity4(matrix);
				Placing = new ObjectNode(UUID.Generate(), m_UUID, Icon, LinearColor.WHITE, EditorNode.CreateObject(GetUUID(), matrix));
	
				GetUApi().SupressNextFrame(true);
			} else {
				delete Placing;
			}
		}
	}
		
	override NodeState GetStateMask()
	{
		return NodeState.ACTIVE | NodeState.VIEW_TREE | NodeState.CLIENT_AUTH | NodeState.SYNC_DIRTY;
	}

	override void OnInteract(NodeInteractType interact_type, Widget widget = null)
	{
		super.OnInteract(interact_type, widget);
		
		if (interact_type & NodeInteractType.PRESS) {
			if (!KeyState(KeyCode.KC_LSHIFT)) {
				Node.States.Clear(NodeState.ACTIVE);
			}
			
			if (KeyState(KeyCode.KC_LCONTROL)) {
				ToggleState(NodeState.ACTIVE);
			} else {
				AddState(NodeState.ACTIVE);
			}			
		}
	}
	
	override NodeInteractType GetInteractMask()
	{
		return NodeInteractType.PRESS | NodeInteractType.CONTEXT | NodeInteractType.DRAG | NodeInteractType.HOVER;
	}
}