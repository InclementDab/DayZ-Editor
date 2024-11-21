class EditorInitFile : EditorFileType
{
	static const int FILE_READ_SIZE = 2048;

	override EditorSaveData Import(string file, ImportSettings settings)
	{
		FileHandle handle = OpenFile(file, FileMode.READ);
		if (!handle)
		{
			EditorLog.Error("File in use %1", file);
			return null;
		}

		EditorSaveData save_data = new EditorSaveData();
		string file_contents;
		int read_size = FILE_READ_SIZE;
		while (read_size > 0)
		{
			string read_contents;
			read_size = ReadFile(handle, read_contents, FILE_READ_SIZE);
			file_contents += read_contents;
		}

		array<string> file_contents_split = { };
		file_contents.Split("\n", file_contents_split);
		foreach (string line: file_contents_split) {

			// Bit of a hacky way of doing this
			// Other idea is to actually run the 'main' script and then enumerate all the spawned objects,
			// then add them to the Editor
			line.TrimInPlace();
			if (line.Contains("SpawnObject") && !line.Contains("void") && !line.Contains("static"))
			{

				TStringArray tokens = { };
				line.Split("\"", tokens);

				if (line.Length() < 5)
				{
					EditorLog.Error("Invalid SpawnObject data found! %1", line);
				}

				/*
				tokens[1]; // Building
	        	tokens[3]; // Position
	        	tokens[5]; // Orientation
				tokens[7]; // Scale
				*/

				float scale = 1.0;
				if (tokens.Count() > 6)
				{
					scale = tokens[7].ToFloat();
				}

				// [11/16/24] dab: bugfix for inconsistent DayZ crap. the other end of this bugfix is in EditorObject.Update
				string type = tokens[1];
				vector position = tokens[3].ToVector();
				vector orientation = tokens[5].ToVector();

				if (type.Contains(".p3d"))
				{
					// Because DayZ is inconsistent, we need to have these checks
					Object p3d_test_object = GetGame().CreateStaticObjectUsingP3D(type, vector.Zero, vector.Zero, 1.0, true);
					if (p3d_test_object)
					{
						orientation = orientation * Math.RAD2DEG;
						position = position + p3d_test_object.GetBoundingCenter();
						GetGame().ObjectDelete(p3d_test_object);
					}
				}


				save_data.EditorObjects.Insert(EditorObjectData.Create(type, position, orientation, scale, EFE_DEFAULT));
			}
		}

		CloseFile(handle);

		return save_data;
	}

	override void Export(EditorSaveData data, string file, ExportSettings settings)
	{
		if (!CopyFile("DayZEditor/scripts/data/Defaults/init.c", file))
		{
			EditorLog.Error("Failed to copy file %1", file);
			return;
		}

		FileHandle handle = OpenFile(file, FileMode.APPEND);
		if (!handle)
		{
			return;
		}

		TStringArray spawn_method = { };
		spawn_method.Insert("\n\n\/\/ Paste anything below this line into the bottom of your 'void main()' function");

		if (data.EditorObjects.Count() > 0)
		{
			spawn_method.Insert("\n\n\/\/ Created Objects");
		}
		foreach (EditorObjectData editor_object: data.EditorObjects) {
			string itype = editor_object.Type;
			itype.Replace("\\", "/");
			spawn_method.Insert(string.Format("SpawnObject(\"%1\", \"%2\", \"%3\", %4);", itype, editor_object.Position.ToString(false), editor_object.Orientation.ToString(false), editor_object.Scale));
		}

		spawn_method.Insert("\n\n\/\/ Uncomment if you want to export loot from newly added buildings");
		spawn_method.Insert("\/\/ Position, Radius (increase if you have a larger map than Chernarus)");
		spawn_method.Insert("\/\/ GetCEApi().ExportProxyData(Vector(7500, GetGame().SurfaceY(7500, 7500), 7500), 20000);");


		foreach (string line: spawn_method) {
			FPrintln(handle, line);
		}

		CloseFile(handle);
	}

	override string GetExtension()
	{
		return ".c";
	}

	override void GetValidExtensions(notnull inout array<ref Param2<string, string>> valid_extensions)
	{
		super.GetValidExtensions(valid_extensions);
		valid_extensions.Insert(new Param2<string, string>("Text File", "*.txt"));
		valid_extensions.Insert(new Param2<string, string>("DayZ Server Init", "*.c"));
	}
}
