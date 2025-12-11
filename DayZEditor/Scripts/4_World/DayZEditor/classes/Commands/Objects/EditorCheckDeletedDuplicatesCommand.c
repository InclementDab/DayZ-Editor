class EditorCheckDeletedDuplicatesCommand: EditorAsyncCommand
{
	protected override void Call(Class sender, CommandArgs args)
	{
		EditorDeletedObjectMap all_deleted_objects = GetEditor().GetDeletedObjects();
		array<int> ids_to_remove = new array<int>;
		
		map<string, int> position_registry = new map<string, int>;
		int checked_count = 0;

		foreach (int id, EditorDeletedObject del_obj : all_deleted_objects) {
			if (!del_obj) continue;
			
			checked_count++;

			vector pos = del_obj.GetOriginalPosition();
			
			float x = Math.Round(pos[0] * 100) / 100;
			float y = Math.Round(pos[1] * 100) / 100;
			float z = Math.Round(pos[2] * 100) / 100;
			
			string spatial_key = string.Format("%1_%2_%3_%4", del_obj.GetType(), x, y, z);
			
			if (position_registry.Contains(spatial_key)) {
				ids_to_remove.Insert(id);
			} else {
				position_registry.Insert(spatial_key, id);
			}
		}

		if (ids_to_remove.Count() > 0) {
			string msg = string.Format("Scanned %1 deletions.\nFound %2 duplicates.\n\nRemove duplicates?", checked_count, ids_to_remove.Count());
			DialogResult result = EditorMessageBox_DEPRECATED.Show(GetName(), msg, MessageBoxButtons.YesNo);
			
			if (result == DialogResult.Yes) {
				int removed_count = 0;
				
				foreach(int delete_id : ids_to_remove) {
					EditorDeletedObject obj_to_remove = all_deleted_objects.Get(delete_id);
					
					if (obj_to_remove && GetEditor().UnhideMapObject(obj_to_remove, true)) {
						removed_count++;
					}
				}
				GetEditor().GetEditorHud().CreateNotification(string.Format("Cleaned up %1 duplicate entries.", removed_count));
			}
		} else {
			GetEditor().GetEditorHud().CreateNotification("No duplicate deletions found.");
		}
	}
	
	override string GetName() 
	{
		return "#STR_EDITOR_CMD_CHECK_DELETION_DUPLICATES";
	}

	override Symbols GetSymbol()
	{
		return Symbols.TRASH_CAN;
	}
}