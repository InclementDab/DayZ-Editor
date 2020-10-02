class COMImportData
{
	string name;
	ref array<ref Param3<string, vector, vector>> m_SceneObjects;
	
	void COMImportData()
	{
		m_SceneObjects = new array<ref Param3<string, vector, vector>>();
	}
}

static FileDialogResult ExportCOMData(ref EditorObjectDataSet data, string filename)
{
	FileHandle handle = OpenFile(filename, FileMode.WRITE);
	
	if (!handle) {
		return FileDialogResult.IN_USE;
	}
	
	foreach (EditorObjectData editor_object: data) {
		// SpawnObject("Land_Construction_House2", "6638.935547 7.190318 6076.024414", "146.000015 0.000000 0.000000")
		string line = string.Format("SpawnObject(\"%1\", \"%2\", \"%3\");", editor_object.Type, editor_object.GetPosition().ToString(false), editor_object.GetPosition().ToString(false));
		FPrintln(handle, line);
	}
	
	return FileDialogResult.SUCCESS;
}