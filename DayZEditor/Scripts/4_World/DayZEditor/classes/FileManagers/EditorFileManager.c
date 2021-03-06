
class EditorFileType
{
	EditorSaveData Import(string file, ImportSettings settings);
	void Export(EditorSaveData data, string file, ExportSettings settings);
	
	string GetExtension();
}

enum HeightType 
{
	ABSOLUTE,
	RELATIVE
}

class ExportSettings
{
	bool Binarized = false;
	HeightType ExportHeightType;
	bool ExportSelectedOnly;
	vector ExportOffset;
	string ExportSetName;
}

class ImportSettings
{

}

enum EditorFileResult
{
	SUCCESS = 0,
	NOT_FOUND = 1,
	IN_USE = 2,
	NOT_SUPPORTED = 3,
	UNKNOWN_ERROR = 100
}


class EditorSaveData
{
	string MapName = "ChernarusPlus";
	vector CameraPosition;
	
	// Do NOT use a map here. ID is stored in EditorObjectData anyway
	ref array<ref EditorObjectData> EditorObjects = {};
	
	// Depreciated
	ref array<int> DeletedObjects = {};
	
	ref array<ref EditorDeletedObjectData> EditorDeletedObjects = {}; 
		
	void ~EditorSaveData()
	{
		delete EditorObjects;
		delete DeletedObjects;
	}
	
	static EditorSaveData CreateFromEditor(Editor editor, bool selected_only = false)
	{
		EditorSaveData save_data = new EditorSaveData();
		
		// Save world name
		save_data.MapName = GetGame().GetWorldName();
		
		// Save Camera Position
		save_data.CameraPosition = editor.GetCamera().GetPosition();
		
		// Save Objects
		EditorObjectMap placed_objects = editor.GetPlacedObjects();
		if (selected_only) {
			placed_objects = editor.GetSelectedObjects();
		}
		
		if (placed_objects) {
			foreach (EditorObject editor_object: placed_objects) {
				if (editor_object.GetType() != string.Empty) {
					save_data.EditorObjects.Insert(editor_object.GetData());
				}
			}
		}
				
		array<Object> deleted_objects = CF.ObjectManager.GetHiddenMapObjects();
		foreach (Object deleted_object: deleted_objects) {
			save_data.EditorDeletedObjects.Insert(new EditorDeletedObjectData(deleted_object.GetType(), deleted_object.GetPosition()));
		}
		
		return save_data;
	}
}


class EditorFileManager
{
	static void GetSafeFileName(out string file_name, string extension)
	{
		TStringArray file_split = {};
		file_name.Split(".", file_split);
		if (file_split.Count() == 1 || file_split[1] != extension) {
			file_name = file_split[0] + extension;
		}
	}
}