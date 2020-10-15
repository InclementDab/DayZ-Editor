
class EditorTerrainBuilderFile: EditorFileType
{
	override void Export(EditorSaveData data, string file, ExportSettings settings)
	{
		FileHandle handle = OpenFile(file, FileMode.WRITE);
		if (!handle) {
			EditorLog.Error("File in use %1", file);
			return;
		}
		
		vector terrainbuilder_offset = Vector(200000, 0, 0);
		foreach (EditorObjectData editor_object: data.EditorObjects) {
			// "construction_house2";206638.935547;6076.024414;146.000015;0.000000;0.000000;1.000000;
			// Name, X, Y, Yaw, Pitch, Roll, Scale, Relative Height			
			vector position = editor_object.BottomCenter;			
			//if (height_type == HeightType.RELATIVE)
			position[1] = position[1] - GetGame().SurfaceY(position[0], position[2]);
			
			string type = editor_object.Type;
			string model_name;
			if (type.Contains("Land_")) {
				type.Replace("Land_", "");
				model_name = type;
			} else {
				model_name = GetGame().GetModelName(type);
			}
			
			EditorLog.Debug("Exporting %1 with model of %2", editor_object.Type, model_name);
			
			vector orientation = editor_object.Orientation;
			position += terrainbuilder_offset;
			string line = string.Format("\"%1\";%2;%3;%4;%5;%6;%7;%8;", model_name, position[0], position[2], orientation[0], orientation[2], orientation[1], editor_object.Scale, position[1]);
			FPrintln(handle, line);
		}
		
		CloseFile(handle);
	}
	
	override string GetExtension() {
		return ".txt";
	}
}


