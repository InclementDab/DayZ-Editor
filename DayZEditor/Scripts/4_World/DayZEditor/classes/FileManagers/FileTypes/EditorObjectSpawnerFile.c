class EditorObjectSpawnerFile : EditorFileType
{

	static string ToRoundString(float value, int decimals)
	{
		int multiplier = Math.Pow(10, decimals);
		int integerPart = value;
		int decimalPart = (Math.Floor(Math.AbsFloat((value - integerPart)) * multiplier)); // Abs needed, normalize close to 0 might still return a small negative which floors to -1
		string decimalString


		if (decimalPart != 0)
		{
			decimalString = ".";
			for (int i = 0; i < decimals - decimalPart.ToString().Length(); i++)
			{
				decimalString += "0";
			}
			decimalString += decimalPart.ToString();
		}
		if (value < 0 && value > -1)
		{
			string sign = "-";
		}
		return sign + integerPart.ToString() + decimalString;
	}
	override void Export(EditorSaveData data, string file, ExportSettings settings)
	{
		EditorLog.Trace("EditorObjectSpawnerFile::Export");

		FileHandle handle = OpenFile(file, FileMode.WRITE);
		if (!handle)
		{
			EditorLog.Error("File in use %1", file);
			return;
		}
		string json = "{\n    \"Objects\": [\n"

		int i = 0;
		foreach (EditorObjectData object_data: data.EditorObjects) {
			i++;
			vector pos = object_data.Position;
			string dpos[3] = { ToRoundString(pos[0], 3), ToRoundString(pos[1], 3), ToRoundString(pos[2], 3) }

			vector ypr = object_data.Orientation;
			string dypr[3] = { ToRoundString(Math.NormalizeAngle(ypr[0]), 3), ToRoundString(Math.NormalizeAngle(ypr[1]), 3), ToRoundString(Math.NormalizeAngle(ypr[2]), 3) }

			float scale = Math.Round(object_data.Scale * 1000) / 1000;
			json += string.Format("        {\n            \"name\": \"%1\",\n", object_data.Type);
			json += string.Format("            \"pos\": [%1, %2, %3],\n", dpos[0], dpos[1], dpos[2]);
			json += string.Format("            \"ypr\": [%1, %2, %3]", dypr[0], dypr[1], dypr[2]);
			// Scale defaults to 1 when not specified 
			if (scale != 1)
			{
				json += string.Format(",\n            \"scale\": %1", scale);
			}
			// enableCEPersistency default to false, needs to be true only for loot items 
			if (GetGame().IsKindOf(object_data.Type, "Inventory_Base") || GetGame().IsKindOf(object_data.Type, "Weapon_Base"))
			{
				json += string.Format(",\n            \"enableCEPersistency\": 1");
			}

			json += string.Format("\n        }");
			if (i != data.EditorObjects.Count())
			{
				json += ",\n";
			}
		}
		json += "\n    ]\n}"


		FPrintln(handle, json);
		CloseFile(handle);
	}

	override EditorSaveData Import(string file, ImportSettings settings)
	{
		EditorLog.Trace("EditorObjectSpawnerFile::Import");
		EditorSaveData save_data = new EditorSaveData();
		ObjectSpawnerJson import_data = new ObjectSpawnerJson();

		JsonFileLoader<ObjectSpawnerJson>.JsonLoadFile(file, import_data);
		foreach (ITEM_SpawnerObject scene_object: import_data.Objects) {
			if (!scene_object.scale)
			{
				scene_object.scale = 1

			}
			save_data.EditorObjects.Insert(EditorObjectData.Create(scene_object.name, Vector(scene_object.pos[0], scene_object.pos[1], scene_object.pos[2]), Vector(scene_object.ypr[0], scene_object.ypr[1], scene_object.ypr[2]), scene_object.scale, EditorObjectFlags.ALL));
		}

		return save_data;
	}
	override string GetExtension()
	{
		return ".json";
	}
}