static EditorFileResult ExportTBData(ref EditorObjectDataMap data, string filename, ExportSettings export_settings)
{
	FileHandle handle = OpenFile(filename, FileMode.WRITE);
	
	if (!handle) {
		return EditorFileResult.IN_USE;
	}
	
	vector terrainbuilder_offset = Vector(200000, 0, 0);
	foreach (EditorObjectData editor_object: data) {
		// "construction_house2";206638.935547;6076.024414;146.000015;0.000000;0.000000;1.000000;
		// Name, X, Y, Yaw, Pitch, Roll, Scale, Relative Height										
		//if (height_type == HeightType.RELATIVE)
		//position[1] = position[1] - GetGame().SurfaceY(position[0], position[2]);

		vector position = editor_object.Position + terrainbuilder_offset;
		vector orientation = editor_object.Orientation;
		string line = string.Format("\"%1\";%2;%3;%4;%5;%6;%7;%8;", GetGame().GetModelName(editor_object.Type), position[0], position[2], orientation[0], orientation[1], orientation[2], editor_object.GetScale(), position[1]);
		FPrintln(handle, line);
	}
	
	if (handle) {
		CloseFile(handle);
	}
	return EditorFileResult.SUCCESS;
}