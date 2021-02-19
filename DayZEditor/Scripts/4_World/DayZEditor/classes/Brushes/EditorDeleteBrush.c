class DeleteBrush: EditorBrush
{	
	override void DuringMouseDown(vector position)
	{
		vector surface_normal = GetGame().SurfaceGetNormal(position[0], position[2]);
		vector contact_pos, contact_dir;
		int component;
		
		array<Object> objects = {};		
		array<CargoBase> cargos = {};
		GetGame().GetObjectsAtPosition(position, EditorBrush.GetRadius() / 2, objects, cargos);
		//DayZPhysics.RaycastRV(position - surface_normal * 5, position + surface_normal * 500, contact_pos, contact_dir, component, results, null, null, false, false, 0, EditorBrush.GetRadius() / 2, CollisionFlags.ALLOBJECTS);
		//GetEditor().ClearSelection();
		
		// todo this is deleting one at a time. use DeleteObjects smile :)
		foreach (Object r: objects) {
			EditorObject eo = GetEditor().GetEditorObject(r);
			if (eo) {
				GetEditor().DeleteObject(eo);
			} else {
				GetEditor().HideMapObject(r);
			}
		}		
	}
}