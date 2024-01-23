class EditorPlaceable: EditorNode
{	
	override void SetSelected(bool selected)
	{
		super.SetSelected(selected);
		
		if (selected) {
			vector transform[4];
			Math3D.MatrixIdentity4(transform);
			
			
			Ray ray = GetDayZGame().GetEditor().GetCamera().PerformCursorRaycast();
			transform[3] = ray.Position;
			
			vector matrix[4];
			Math3D.MatrixOrthogonalize4(matrix);
			
			EditorObject editor_object = new EditorObject(UUID.Generate(), m_UUID, GetIcon(), GetUUID(), matrix, EFE_DEFAULT);
			GetDayZGame().GetEditor().Placing.Insert(editor_object);
			
			//GetDayZGame().GetEditor().Placing[Editor.CreateObject(m_UUID, transform)] = new EditorHandData(this, matrix);
		}
	}
}