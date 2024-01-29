class PlaceableNode: ToolNode
{	
	protected vector m_CursorNormal = vector.Aside;
	protected ref ObjectNode m_ObjectNode;
	
	override bool CreateContextMenu(inout ObservableCollection<ref ScriptView> list_items)
	{
		Editor editor = GetDayZGame().GetEditor();
		list_items.Insert(new CommandMenuItem(editor.GetCommand("Cut")));
		list_items.Insert(new CommandMenuItem(editor.GetCommand("Copy")));
		list_items.Insert(new CommandMenuItem(editor.GetCommand("Paste")));
		list_items.Insert(new CommandMenuDivider());
		
		return true;
	}
	
	override void OnSelectionChanged(bool state)
	{
		super.OnSelectionChanged(state);
		
		vector matrix[4];
		Math3D.MatrixIdentity4(matrix);
		if (state) {
			m_ObjectNode = new ObjectNode(UUID.Generate(), m_UUID, GetIcon(), Editor.CreateObject(GetUUID(), matrix), EFE_DEFAULT);
			GetUApi().SupressNextFrame(true);
		} else {
			//delete m_ObjectNode;
		}
	}
	
	override bool Update(float dt, Raycast raycast)
	{
		Input input = GetGame().GetInput();
		if (!m_ObjectNode) {
			return true;
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
		
		m_CursorNormal = m_CursorNormal.Multiply3(rotation_mat);
		
		//Print(Placing.Count());
		vector transform[4] = { m_CursorNormal, raycast.Bounce.Direction, m_CursorNormal * raycast.Bounce.Direction, raycast.Bounce.Position };
		m_ObjectNode.SetBaseTransform(transform);
				
		if (input.LocalPress_ID(UAFire)) {
			GetEditor().InsertHistory(string.Format("Undo Place %1", m_ObjectNode.GetUUID()), Symbols.CLOCK_ROTATE_LEFT, m_ObjectNode, null);
			GetEditor()[Editor.EDITED_OBJECTS]["PlacedObjects"].Add(m_ObjectNode);
			GetEditor()[Editor.EDITED_OBJECTS]["PlacedObjects"].Synchronize();
			m_ObjectNode = null;
			// remove it from placing
			GetEditor().PlaySound(EditorSounds.PLOP);
		}
				
		return true;
	}
}