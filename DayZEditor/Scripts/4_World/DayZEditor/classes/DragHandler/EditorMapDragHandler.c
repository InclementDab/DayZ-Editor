class EditorMapDragHandler: EditorDragHandler
{
	override void OnDragging(out vector transform[4], notnull EditorObject target)
	{		
		vector pos = Editor.CurrentMousePosition;
		transform[3] = pos;
		
		if (GetEditor().GroundMode) {
			vector target_pos = target.GetPosition();
			transform[3][1] = GetGame().SurfaceY(pos[0], pos[2]) + target_pos[1] - GetGame().SurfaceY(target_pos[0], target_pos[2]);
		} else {
			transform[3][1] = GetGame().SurfaceY(pos[0], pos[2]) + target.GetSize()[1] / 2;
		}
	}
}