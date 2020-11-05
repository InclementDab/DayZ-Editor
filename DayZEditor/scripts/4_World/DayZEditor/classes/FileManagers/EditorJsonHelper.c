class EditorJsonHelper<Class T>
{
	protected static ref JsonSerializer m_Serializer = new JsonSerializer;
	
	static void StringToObject( string string_data, out T data )
	{
		string error;
		if( !m_Serializer )
			m_Serializer = new JsonSerializer;
		
		if( !m_Serializer.ReadFromString( data, string_data, error ) )
			Error( error );
	}
	
	static string ObjectToString( T data )
	{
		string string_data;
		if( !m_Serializer )
			m_Serializer = new JsonSerializer;

		m_Serializer.WriteToString( data, true, string_data );
		return string_data;
	}

	static void SaveToFile(string path, T data)
	{
		FileHandle fh = OpenFile( path, FileMode.WRITE );
			
		if (fh != 0)
		{
			string jsonData;
			bool success = m_Serializer.WriteToString(data, true, jsonData);

			if( success && jsonData != string.Empty )
				FPrintln( fh, jsonData );

			Print("[EditorJsonHelper] SaveToFile() DONE --> " + path);
			CloseFile( fh );
		}else{
			EditorLog.Error("[EditorJsonHelper] SaveToFile() File could not be created at %1", path);
		}
	}

	static void LoadFromFile(string path, out T data)
	{
		if ( !FileExist(path) )
		{
			EditorLog.Error("File not found %1", path);
			return;
		}

		FileHandle fh = OpenFile( path, FileMode.READ );
		string jsonData;
		string error;

		if (fh != 0)
		{
			string line;
			while( FGets(fh, line) > 0 )
			{
				jsonData = jsonData + "\n" + line;
			}

			bool success = m_Serializer.ReadFromString(data, jsonData, error);
			if(error != string.Empty || !success)
			{
				EditorLog.Error("[EditorJsonHelper] ERROR Parsing --> %1", error);
			}else{
				Print("[EditorJsonHelper] Loaded file: " + path + " ( Files over 500MB~ might have issues sometimes )");
			}
			CloseFile(fh);
		}
	}
};