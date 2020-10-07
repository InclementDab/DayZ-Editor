
class EditorCOMFile: EditorFileType
{
	override void Export(EditorSaveData data, string file, ExportSettings settings)
	{
		FileHandle handle = OpenFile(file, FileMode.WRITE);
		if (!handle) {
			return;
		}
		
		foreach (EditorObjectData editor_object: data.EditorObjects) {
			// SpawnObject("Land_Construction_House2", "6638.935547 7.190318 6076.024414", "146.000015 0.000000 0.000000")
			string line = string.Format("SpawnObject(\"%1\", \"%2\", \"%3\");", editor_object.Type, editor_object.Position.ToString(false), editor_object.Orientation.ToString(false));
			FPrintln(handle, line);
		}
		
	
		CloseFile(handle);
	}
	
	override string GetExtension() {
		return ".c";
	}
}

class COMImportData
{
	string name;
	ref array<ref Param3<string, vector, vector>> m_SceneObjects;
	
	void COMImportData()
	{
		m_SceneObjects = new array<ref Param3<string, vector, vector>>();
	}
}
