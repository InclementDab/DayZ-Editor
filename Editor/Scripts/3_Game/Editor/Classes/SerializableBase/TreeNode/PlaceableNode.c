class PlaceableNode: NamedNode
{
	vector Offset[4] = {
		"1 0 0",
		"0 1 0",
		"0 0 1",
		"0 0 0"
	};
	
	ref ObjectNode Placing;
	
	protected vector m_ModifierStart;
	
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
		
		if (input.LocalPress_ID(UAVehicleTurbo)) {
			
			return;
		}
		
		if (input.LocalPress_ID(UATempRaiseWeapon)) {
			if (!KeyState(KeyCode.KC_LCONTROL)) {
				RemoveState(NodeState.ACTIVE);
				return;
			}
			
			m_ModifierStart = raycast.Bounce.Position;
			return;
		}
		
		if (input.LocalValue_ID(UATempRaiseWeapon)) {
			
		}
		
		if (input.LocalRelease_ID(UATempRaiseWeapon)) {
			Offset[3] = raycast.Bounce.Position - m_ModifierStart;
			return;
		}
		
		vector transform[4] = {
			"1 0 0",
			"0 1 0",
			"0 0 1", 
			raycast.Bounce.Position
		};
	
		vector rotation_mat[4] = {
			"1 0 0",
			"0 1 0",
			"0 0 1",
			"0 0 0"
		};
		float rotation_y = 21 * (input.LocalValue_ID(UAZoomInOptics) + input.LocalValue_ID(UAZoomOutOptics) * -1);
		Math3D.YawPitchRollMatrix(Vector(rotation_y, 0, 0), rotation_mat);
		Math3D.MatrixMultiply4(rotation_mat, Offset, Offset);

		Math3D.MatrixMultiply4(transform, Offset, transform);
		
		Shape.CreateMatrix(transform);
		
		//m_CursorAside = m_CursorAside.Multiply3(rotation_mat);
		
		//Print(Placing.Count());
		//vector transform[4] = { m_CursorAside, raycast.Bounce.Direction, m_CursorAside * raycast.Bounce.Direction, raycast.Bounce.Position };

		Placing.SetBaseTransform(transform);
	
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
			} else {
				delete Placing;
			}
		}
	}
		
	override NodeState GetStateMask()
	{
		return NodeState.ACTIVE | NodeState.CLIENT_AUTH | NodeState.SYNC_DIRTY;
	}

	override void OnInteract(NodeInteractType interact_type, Widget widget = null)
	{
		super.OnInteract(interact_type, widget);
		
		if (interact_type & NodeInteractType.PRESS) {
			if (!KeyState(KeyCode.KC_LSHIFT)) {
				
				// Causes freeze crash
				//Node.ClearStates(NodeState.ACTIVE);
			}
			
			if (KeyState(KeyCode.KC_LCONTROL)) {
				ToggleState(NodeState.ACTIVE);
			} else {
				AddState(NodeState.ACTIVE);
			}		
			
			GetUApi().SupressNextFrame(true);	
		}
	}
	
	override NodeInteractType GetInteractMask()
	{
		return NodeInteractType.PRESS | NodeInteractType.CONTEXT | NodeInteractType.DRAG | NodeInteractType.HOVER;
	}
}