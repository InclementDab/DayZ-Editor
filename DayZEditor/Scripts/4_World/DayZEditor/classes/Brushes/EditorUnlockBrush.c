//[RegisterBrush(EditorUnlockBrush, "#STR_EDITOR_UNLOCK")]
class EditorUnlockBrush: EditorBrush
{
	override void DuringMouseDown(vector position)
	{
		vector surface_normal = GetGame().SurfaceGetNormal(position[0], position[2]);
		vector contact_pos, contact_dir;
		int component;
		
		array<Object> objects = {};		
		array<CargoBase> cargos = {};
		GetGame().GetObjectsAtPosition(position, EditorBrush.BrushRadius / 2, objects, cargos);
		
		EditorObjectMap editor_objects();
		array<Object> deleted_objects = {};
		foreach (Object r: objects) {
			EditorObject eo = GetEditor().GetEditorObject(r);
			if (eo) {
				editor_objects.InsertEditorObject(eo);
			}
		}	
		
		if (editor_objects.Count() > 0) {
			GetEditor().UnlockObjects(editor_objects);
		}
	}
}

//[RegisterBrush(EditorLockBrush, "#STR_EDITOR_LOCK")]
class EditorLockBrush: EditorBrush
{
	override void DuringMouseDown(vector position)
	{
		vector surface_normal = GetGame().SurfaceGetNormal(position[0], position[2]);
		vector contact_pos, contact_dir;
		int component;
		
		array<Object> objects = {};		
		array<CargoBase> cargos = {};
		GetGame().GetObjectsAtPosition(position, EditorBrush.BrushRadius / 2, objects, cargos);
		
		EditorObjectMap editor_objects();
		array<Object> deleted_objects = {};
		foreach (Object r: objects) {
			EditorObject eo = GetEditor().GetEditorObject(r);
			if (eo) {
				editor_objects.InsertEditorObject(eo);
			}
		}	
		
		if (editor_objects.Count() > 0) {
			GetEditor().LockObjects(editor_objects);
		}
	}
}