class EditorSpawnPresetFile: EditorFileType
{
	override Managed Import(string file) 
	{ 
		PlayerSpawnPreset preset;
		string error;
		if (!JsonFileLoader<PlayerSpawnPreset>.LoadFile(file, preset, error)) {
			EditorLog.Warning(error);
			return null;
		}

		return preset; 
	}
	
	override void Export(Managed data, string file)
	{
		PlayerSpawnPreset spawn_preset = PlayerSpawnPreset.Cast(data);
		if (!spawn_preset) {
			return;
		}

		string error;
		if (!JsonFileLoader<PlayerSpawnPreset>.SaveFile(file, spawn_preset, error)) {
			EditorLog.Warning(error);
			return;
		}
	}
	
	override void GetValidExtensions(notnull inout array<ref Param2<string, string>> valid_extensions)
	{
		super.GetValidExtensions(valid_extensions);
		valid_extensions.Insert(new Param2<string, string>("Spawn Preset", "*.json"));
	}
	
	override string GetExtension()
	{
		return ".json";
	}
}