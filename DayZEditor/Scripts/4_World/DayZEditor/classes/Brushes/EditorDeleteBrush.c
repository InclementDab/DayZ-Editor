class DeleteBrush: EditorBrush
{	
	override void DuringMouseDown(vector position)
	{
		vector surface_normal = GetGame().SurfaceGetNormal(position[0], position[2]);
		vector contact_pos, contact_dir;
		int component;
		
		array<Object> objects = {};		
		array<CargoBase> cargos = {};
		GetGame().GetObjectsAtPosition(position, EditorBrush.BrushRadius / 2, objects, cargos);
		//DayZPhysics.RaycastRV(position - surface_normal * 5, position + surface_normal * 500, contact_pos, contact_dir, component, results, null, null, false, false, 0, EditorBrush.GetRadius() / 2, CollisionFlags.ALLOBJECTS);
		//GetEditor().ClearSelection();
		
		array<EditorObject> editor_objects = {};
		array<Object> deleted_objects = {};
		foreach (Object r: objects) {
			EditorObject eo = GetEditor().GetEditorObject(r);
			if (eo) {
				editor_objects.Insert(eo);
			} else {
				if (GetEditor().CanHideMapObject(r.GetType())) {
					GetGame().ObjectDelete(r);
					deleted_objects.Insert(r);
				}
			}
		}	
		
		if (editor_objects.Count() > 0) {
			GetEditor().DeleteObjects(editor_objects);
		}
		
		if (deleted_objects.Count() > 0) {
			GetEditor().HideMapObjects(deleted_objects);
		}
	}
}