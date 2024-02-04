class CursorToggle: CommandNode
{
	protected vector m_CursorAside = vector.Aside;
	
	override void Update(float timeslice)
	{
		Input input = GetGame().GetInput();
		EditorNode editor = GetEditor();
		EditorCamera camera = editor.GetCamera();
										
		if (input.LocalPress_ID(UAUIBack)) {
			TreeNode.StateMachine.RemoveAllStates(TreeNodeState.CONTEXT);
			TreeNode.StateMachine.RemoveAllStates(TreeNodeState.ACTIVE);
		}
		
		if (input.LocalPress_ID(UAFire) || input.LocalPress_ID(UAUIBack)) {			
			
			if (!GetWidgetUnderCursor()) {
				// reassigning because were gonna fuck with this statemachine array
				array<TreeNode> nodes = {};
				nodes.Copy(TreeNode.StateMachine[TreeNodeState.ACTIVE]);
				foreach (TreeNode node_to_deselect: nodes) {
					if (node_to_deselect && node_to_deselect.GetInteractType() == TreeNodeInteract.PRESS) {
						node_to_deselect.RemoveState(TreeNodeState.ACTIVE);
					}
				}
				
				TreeNode.StateMachine.RemoveAllStates(TreeNodeState.CONTEXT);
			}		
		}
		
		foreach (string uuid, TreeNode node1: editor.GetPlacing().Children) {
			ObjectNode object_node = ObjectNode.Cast(node1);
			if (!object_node) {
				continue;
			}
			
			Raycast raycast = camera.PerformCursorRaycast(object_node.GetObject());
			if (!raycast) {
				continue;
			}
			
			vector camera_orthogonal[4] = { raycast.Source.Direction * raycast.Bounce.Direction, raycast.Bounce.Direction, raycast.Source.Direction, raycast.Source.Position };
			Math3D.MatrixOrthogonalize4(camera_orthogonal);	
			
			vector rotation_mat[3];
			Math3D.MatrixIdentity3(rotation_mat);
			if (input.LocalPress_ID(UAZoomInOptics)) {
				Math3D.YawPitchRollMatrix(Vector(-15, 0, 0), rotation_mat);
			}
			
			if (input.LocalPress_ID(UAZoomOutOptics)) {
				Math3D.YawPitchRollMatrix(Vector(15, 0, 0), rotation_mat);
			}
			
			Math3D.MatrixMultiply3(camera_orthogonal, rotation_mat, camera_orthogonal);
			
			//Shape.CreateMatrix(camera_orthogonal);
			
			m_CursorAside = m_CursorAside.Multiply3(rotation_mat);
			
			//Print(Placing.Count());
			vector transform[4] = { m_CursorAside, raycast.Bounce.Direction, m_CursorAside * raycast.Bounce.Direction, raycast.Bounce.Position };
			object_node.SetBaseTransform(transform);
		}
		
		if (input.LocalPress_ID(UAZoomIn)) {
			if (GetGame().GetUIManager().IsCursorVisible()) {				
				vector camera_position = camera.GetCursorRay().GetPoint(1000.0);
				Raycast raycast2 = camera.PerformCursorRaycast();
				if (raycast2) {
					vector current_position = camera.GetPosition();
					float y_height = current_position[1] - GetGame().SurfaceY(current_position[0], current_position[2]);
					camera_position = raycast2.Bounce.GetPoint(y_height);
				}
								
				camera.SetPosition(camera_position);
				camera.Update();
			} else {
				camera.FieldOfView = GameConstants.DZPLAYER_CAMERA_FOV_EYEZOOM;
			}
		}
		
		if (input.LocalRelease_ID(UAZoomIn)) { 
			camera.FieldOfView = 1.0;
		}
	}
	
	override void OnStateChanged(TreeNodeState state, TreeNodeState total_state)
	{
		super.OnStateChanged(state, total_state);
	
		if (GetEditor().GetHud()) {
			GetEditor().GetHud().ClearCursor();
		}
		
		GetGame().GetUIManager().ShowCursor(total_state.IsActive());
	}
	
	override TreeNodeInteract GetInteractType()
	{
		return TreeNodeInteract.TOGGLE;
	}
}