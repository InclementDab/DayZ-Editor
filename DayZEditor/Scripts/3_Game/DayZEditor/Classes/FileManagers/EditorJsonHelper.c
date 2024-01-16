class EditorJsonLoader<Class T>
{
	protected static ref JsonSerializer m_Serializer = new JsonSerializer;
	
	static void StringToObject(string string_data, out T data)
	{
		string error;
		if (!m_Serializer)
			m_Serializer = new JsonSerializer;
		
		if (!m_Serializer.ReadFromString(data, string_data, error)) {
			EditorLog.Error(error);
		}
	}
	
	static string ObjectToString(T data)
	{
		string string_data;
		if (!m_Serializer)
			m_Serializer = new JsonSerializer;

		if (!m_Serializer.WriteToString(data, true, string_data)) {
			EditorLog.Error("EditorJsonLoader::ObjectToString Could not stringify %1", data.ToString());
			return string.Empty;
		}
		
		return string_data;
	}

	static void SaveToFile(string path, T data)
	{
		FileHandle fh = OpenFile(path, FileMode.WRITE);
			
		if (!fh) {
			EditorLog.Error("EditorJsonLoader::SaveToFile File could not be created at %1", path);
			return;
		} 
		
		string jsonData;
		bool success = m_Serializer.WriteToString(data, true, jsonData);

		if (success && jsonData != string.Empty) {
			FPrintln(fh, jsonData);
		}

		EditorLog.Info("EditorJsonLoader::SaveToFile Complete: " + path);
		CloseFile(fh);
	}

	static void LoadFromFile(string path, out T data)
	{
		if (!FileExist(path)) {
			EditorLog.Error("EditorJsonLoader::LoadFromFile File not found %1", path);
			return;
		}

		FileHandle fh = OpenFile(path, FileMode.READ);
		string jsonData;
		string error;

		if (!fh) {
			EditorLog.Error("EditorJsonLoader::LoadFromFile File could not be opened %1", path);
			return;
		}
		
		string line;
		while (FGets(fh, line) > 0) {
			jsonData = jsonData + "\n" + line;
		}

		bool success = m_Serializer.ReadFromString(data, jsonData, error);
		
		if (error != string.Empty || !success) {
			EditorLog.Error("EditorJsonLoader::LoadFromFile ERROR Parsing %1", error);
			return;
		}
		
		EditorLog.Debug("EditorJsonLoader::LoadFromFile Loaded file: " + path + " ( Files over 500MB~ might have issues sometimes )");
		CloseFile(fh);
	}
};