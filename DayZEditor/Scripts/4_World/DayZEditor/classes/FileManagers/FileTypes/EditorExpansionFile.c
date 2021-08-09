class EditorExpansionFile: EditorFileType
{
	override EditorSaveData Import(string file, ImportSettings settings) 
	{
		EditorLog.Trace("EditorExpansionFile::Import");
		
		FileHandle handle = OpenFile(file, FileMode.READ);
		if (!handle) {
			EditorLog.Error("File in use %1", file);
			return null;
		}
		
		EditorSaveData save_data = new EditorSaveData();
	
        string line;
        int line_size = FGets(handle, line);
		while (line_size > 0) {
			line.TrimInPlace();
						
			if (line_size != 1 && line.Substring(0, 2) != "\/\/") {
				TStringArray tokens = {};
	        	line.Split("|", tokens);  
	        	save_data.EditorObjects.Insert(EditorObjectData.Create(tokens[0], tokens[1].ToVector(), tokens[2].ToVector(), 1, EditorObjectFlags.ALL));
			}
			
			line_size = FGets(handle, line);
		}        

		CloseFile(handle);
		
		return save_data;
	}
	
	override void Export(EditorSaveData data, string file, ExportSettings settings)
	{
		EditorLog.Trace("EditorExpansionFile::Export");
		
		FileHandle handle = OpenFile(file, FileMode.WRITE);
		if (!handle) {
			EditorLog.Error("File in use %1", file);
			return;
		}
		
		foreach (EditorObjectData editor_object: data.EditorObjects) {
			// Land_Construction_House2|13108.842773 10.015385 6931.083984|-101.999985 0.000000 0.000000
			if (!editor_object.WorldObject) {
				EditorLog.Error("EditorExpansionFile::Invalid Object!");
				break;
			}
						
			string line = string.Format("%1|%2|%3", editor_object.Type, editor_object.WorldObject.GetPosition().ToString(false), editor_object.WorldObject.GetOrientation().ToString(false));
			FPrintln(handle, line);
			
			EntityAI entity = EntityAI.Cast(editor_object.WorldObject);
			if (entity) {
				string attachment_list;
				for (int i = 0; i < entity.GetInventory().GetAttachmentSlotsCount(); i++) {
					EntityAI attachment = entity.GetInventory().GetAttachmentFromIndex(i);
					if (!attachment) {
						continue;
					}
										
					attachment_list += attachment.GetType() + ",";
				}
				
				if (attachment_list[attachment_list.Length() - 1] == ",") {
					attachment_list[attachment_list.Length() - 1] = "";
				}
								
				if (attachment_list != string.Empty) {
					line = string.Format("%1.Components|%2|%3|%4", editor_object.Type, editor_object.WorldObject.GetPosition().ToString(false), editor_object.WorldObject.GetOrientation().ToString(false), attachment_list);
					FPrintln(handle, line);
				}
			}
		}
		
		CloseFile(handle);
	}
	
	override string GetExtension() 
	{
		return ".map";
	}
}
