class DeleteBrush : EditorBrush
{
	override void DuringMouseDown(vector position)
	{
		array<Object> objects = { };
		GetGame().GetObjectsAtPosition3D(position, BrushRadius, objects, null);

		EditorObjectMap editorObjects = new EditorObjectMap();
		array<Object> deleted_objects = { };

		foreach (Object object : objects) {
			if (!object) continue;

			if (GetDayZGame().GetSuppressedObjectManager().IsSuppressed(object))
			{
				continue;
			}

			EditorObject eo = GetEditor().GetEditorObject(object);
			if (eo)
			{
				editorObjects.InsertEditorObject(eo);
			}
			else
			{
				deleted_objects.Insert(object);
			}
		}

		if (editorObjects.Count() > 0)
		{
			GetEditor().DeleteObjects(editorObjects);
		}

		if (deleted_objects.Count() > 0)
		{
			GetEditor().HideMapObjects(deleted_objects);
		}
	}
}