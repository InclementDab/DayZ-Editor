class EditorCheckDuplicatesCommand: EditorAsyncCommand
{
	protected override void Call(Class sender, CommandArgs args)
	{
		EditorObjectMap duplicate_objects();
		
		// prioritize selected objects first, else use all placed objects
		EditorObjectMap editor_objects = GetEditor().GetSelectedObjects();
		if (editor_objects.Count() == 0) {
			editor_objects = GetEditor().GetPlacedObjects();
		}
		
		// Bucket Sort by Type
		map<string, ref array<EditorObject>> buckets = new map<string, ref array<EditorObject>>();
		
		foreach (int id, EditorObject editor_object: editor_objects) {
			if (!editor_object) continue;

			string type = editor_object.GetType();
			if (!buckets.Contains(type)) {
				buckets[type] = new array<EditorObject>();
			}
			buckets[type].Insert(editor_object);
		}

		// Iterate through buckets to find duplicates
		foreach (string keyType, array<EditorObject> bucket_list : buckets) {
			int count = bucket_list.Count();
			if (count < 2) continue; // No duplicates possible in this bucket

			for (int i = 0; i < count; i++) {
				EditorObject world_object = bucket_list[i];
				
				// we can skip checking it against others if already marked as duplicate
				if (duplicate_objects.Contains(world_object.GetID())) continue;

				vector world_pos = world_object.GetPosition();

				for (int j = i + 1; j < count; j++) {
					EditorObject found_object = bucket_list[j];
					
					// If target is already marked, skip
					if (duplicate_objects.Contains(found_object.GetID())) continue;

					// Position Check (0.25m tolerance / 25cm) (Distance squared for performance)
					if (vector.DistanceSq(world_pos, found_object.GetPosition()) > 0.0625) {
						continue;
					}
				
					duplicate_objects.InsertEditorObject(found_object);
				}
			}
		}
		
		if (duplicate_objects.Count() == 0) {
			EditorMessageBox_DEPRECATED.Show(GetName(), "No duplicate objects found in search", MessageBoxButtons.OK);
			EditorLog.Info("No duplicate objects found!");
			return;
		}
		
		EditorLog.Info("%1 duplicate objects found in search", duplicate_objects.Count().ToString());
		
		DialogResult result = EditorMessageBox_DEPRECATED.Show(GetName(), string.Format("%1 duplicate objects found in search, delete?", duplicate_objects.Count()), MessageBoxButtons.YesNo);
		if (result != DialogResult.Yes) {
			return;
		}
		
		GetEditor().DeleteObjects(duplicate_objects);
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_CMD_CHECK_DUPLICATES";
	}

	override Symbols GetSymbol()
	{
		return Symbols.RECTANGLE_XMARK;
	}
}