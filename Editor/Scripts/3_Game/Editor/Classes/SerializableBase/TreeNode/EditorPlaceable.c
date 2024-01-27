class EditorPlaceable: TreeNode
{	
	override void SetSelected(bool selected)
	{
		super.SetSelected(selected);
		
		if (selected) {			
			Raycast raycast = GetDayZGame().GetEditor().GetCamera().PerformCursorRaycast();
			
			vector matrix[4];
			Math3D.MatrixIdentity4(matrix);
			matrix[3] = raycast.Bounce.Position;
			
			Object object = Editor.CreateObject(GetUUID(), matrix);
			EditorObject editor_object = new EditorObject(UUID.Generate(), m_UUID, GetIcon(), object, EFE_DEFAULT);
			if (object) {
				EditorObject.ByObject[object] = editor_object;
			}
			
			GetDayZGame().GetEditor().Placing.Insert(editor_object);
		} else {
			GetDayZGame().GetEditor().Placing.Clear();
		}
	}
}