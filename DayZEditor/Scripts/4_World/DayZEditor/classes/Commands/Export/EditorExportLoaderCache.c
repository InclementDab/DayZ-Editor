class EditorExportLoaderCache: EditorAsyncCommand
{
	protected override void Call(Class sender, CommandArgs args)
	{
		if (!CreateLoaderCache(GetCacheFileName())) {
			EditorMessageBox.Show("Error", "Failed to export cache", MessageBoxButtons.OK);
			return;
		}
		
		EditorMessageBox.Show("Done", "Loader cache exported to " + GetCacheFileName(), MessageBoxButtons.OK);
	}
	
	bool CreateLoaderCache(string file_name)
	{
		// Adds all map objects to the WorldObjects array
		map<int, vector> world_cache = new map<int, vector>();
		array<Object> objects = {};
		array<CargoBase> cargos = {};
		GetGame().GetObjectsAtPosition(vector.Zero, 100000, objects, cargos);
		
		foreach (Object o: objects) {
			world_cache.Insert(o.GetID(), o.GetPosition());
		}
		
		FileSerializer serializer = new FileSerializer();
		if (!serializer.Open(file_name, FileMode.WRITE)) {
			Error("EditorLoader: Error Opening Cache");
			return false;
		}
		
		if (!serializer.Write(world_cache)) {
			Error("EditorLoader: Error Writing Cache");
			return false;
		}
		
		serializer.Close();
		
		return FileExist(file_name);
	}
	
	string GetCacheFileName()
	{
		string world_name;
		GetGame().GetWorldName(world_name);
		world_name.ToLower();
		
		return string.Format("$profile:%1.cache", world_name);
	}
	
	override string GetName()
	{
		return "#STR_EDITOR_CMD_EXPORT_LOADER_CACHE";
	}
}