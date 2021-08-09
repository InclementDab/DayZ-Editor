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
				
				string type = tokens[0];
				string trader_type;
				// setting up for processing after the fact
				if (type.Contains(".")) {
					TStringArray tsplit = {};
					type.Split(".", tsplit);
					type = tsplit[0];
					trader_type = tsplit[1];
				}
				
				EditorObjectData data = EditorObjectData.Create(type, tokens[1].ToVector(), tokens[2].ToVector(), 1, EditorObjectFlags.ALL);
				
				if (trader_type != string.Empty) {
					data.Parameters["ExpansionTraderType"] = new Param1<string>(trader_type);
				}
				
				if (tokens[3] != string.Empty) {
					tokens[3].Split(",", data.Attachments);
				}
				
				save_data.EditorObjects.Insert(data);
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
		
		string attachment_list;
		foreach (EditorObjectData editor_object: data.EditorObjects) {
			// Land_Construction_House2|13108.842773 10.015385 6931.083984|-101.999985 0.000000 0.000000
			if (!editor_object.WorldObject) {
				EditorLog.Error("EditorExpansionFile::Invalid Object!");
				break;
			}
						
			
			string line;
			EntityAI entity = EntityAI.Cast(editor_object.WorldObject);
			if (entity) {
				array<EntityAI> attachments = {};				
				entity.GetInventory().EnumerateInventory(InventoryTraversalType.INORDER, attachments);
				foreach (EntityAI attachment: attachments) {
					if (!entity.GetInventory().HasAttachment(attachment)) {
						continue;
					}
										
					attachment_list += attachment.GetType() + ",";
				}
				
				if (attachment_list.Length() > 0 && attachment_list[attachment_list.Length() - 1] == ",") {
					attachment_list[attachment_list.Length() - 1] = "";
				}
			}
			
			// traders
			Param1<string> trader_param = Param1<string>.Cast(editor_object.Parameters["ExpansionTraderType"]);
			if (attachment_list != string.Empty && trader_param) {				
				line = string.Format("%1.%5|%2|%3|%4", editor_object.Type, editor_object.WorldObject.GetPosition().ToString(false), editor_object.WorldObject.GetOrientation().ToString(false), attachment_list, trader_param.param1);
			} else {
				line = string.Format("%1|%2|%3", editor_object.Type, editor_object.WorldObject.GetPosition().ToString(false), editor_object.WorldObject.GetOrientation().ToString(false));
			}
			
			attachment_list = string.Empty;
			FPrintln(handle, line);
		}
		
		CloseFile(handle);
	}
	
	override string GetExtension() 
	{
		return ".map";
	}
}
