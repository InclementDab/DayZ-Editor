
class EditorCOMFile: EditorFileType
{
	override EditorSaveData Import(string file, ImportSettings settings)
	{
		
		return null;
	}
	
	
	override void Export(EditorSaveData data, string file, ExportSettings settings)
	{
		FileHandle handle = OpenFile(file, FileMode.WRITE);
		if (!handle) {
			return;
		}
		
		TStringArray spawn_method = {
			"static void SpawnObject(string type, vector position, vector orientation)",
			"{",
			"    auto obj = GetGame().CreateObjectEx(type, position, ECE_SETUP | ECE_UPDATEPATHGRAPH | ECE_CREATEPHYSICS);",
			"    obj.SetPosition(position);",
			"    obj.SetOrientation(orientation);",
			"    obj.SetOrientation(obj.GetOrientation());",
			"    obj.Update();",
			"	 obj.SetAffectPathgraph(true, false);",
    		"	 if (obj.CanAffectPathgraph()) GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetGame().UpdatePathgraphRegionByObject, 100, false, obj);",
			"}\n\n",
			"void main()",
			"{"
		};
		
			
		foreach (EditorObjectData editor_object: data.EditorObjects) {
			// SpawnObject("Land_Construction_House2", "6638.935547 7.190318 6076.024414", "146.000015 0.000000 0.000000")
			spawn_method.Insert(string.Format("	SpawnObject(\"%1\", \"%2\", \"%3\");", editor_object.Type, editor_object.Position.ToString(false), editor_object.Orientation.ToString(false)));
		}
			
		spawn_method.Insert("\/\/Position, Radius (increase if you have a larger map than Chernarus)");
		spawn_method.Insert("	GetCEApi().ExportProxyData(\"7500 0 7500\", 20000);");
		spawn_method.Insert("}");
		
		PrintLines(handle, spawn_method);
	
		CloseFile(handle);
	}
		
	private void PrintLines(FileHandle handle, TStringArray lines)
	{
		foreach (string line: lines) {
			FPrintln(handle, line);	
		}
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
