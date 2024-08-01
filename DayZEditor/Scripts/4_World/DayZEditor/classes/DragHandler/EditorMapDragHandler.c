class EditorMapDragHandler: EditorDragHandler
{
	protected override void OnDragging(notnull EditorObject target, notnull array<EditorObject> additional_drag_targets)
	{
		vector transform[4];
		target.GetTransform(transform);

		if (!GetEditor().GetEditorHud().IsMapVisible()) {
			OnDragFinish();
			return;
		}

		int mouse_x, mouse_y;
		GetMousePos(mouse_x, mouse_y);
		vector map_world_pos = GetEditor().GetEditorHud().EditorMapWidget.ScreenToMap(Vector(mouse_x, mouse_y, 0));
		transform[3] = map_world_pos;
		
		if (GetEditor().GroundMode) {
			vector target_pos = target.GetPosition();
			transform[3][1] = GetGame().SurfaceY(map_world_pos[0], map_world_pos[2]) + target_pos[1] - GetGame().SurfaceY(target_pos[0], target_pos[2]);
		} else {
			transform[3][1] = GetGame().SurfaceY(map_world_pos[0], map_world_pos[2]) + target.GetSize()[1] / 2;
		}

		target.SetTransform(transform);
	}
}