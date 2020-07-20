

typedef ref array<ref vector>> UndeleteableVectorArray;
typedef ref array<string>> UndeleteableStringArray;

typedef ref map<int, ref EditorObject> EditorObjectSet;


class Editor: Managed
{
	private ref UIManager 					m_UIManager; 	
		
	static ref EditorHologram 				ObjectInHand;
	static Object							ObjectUnderCursor = null;
	static EditorObject 					EditorObjectUnderCursor = null;
	
	static ref set<ref EditorObjectLink>	SessionCache;
	static ref EditorObjectSet 				CopyCache;
	
	
	static vector CurrentMousePosition;
	static bool IsDragging = false;
		
	static ref EditorObjectSet				SelectedObjects;
	static ref EditorObjectSet 				EditorObjects;
	
	static ref set<ref EditorAction> 		EditorActionStack;
	
	static ref EditorEvents 				EditorEventHandler;
	
	static ref Widget 						EditorUIWidget;
	
	static ref EditorUI						ActiveEditorUI;	
	static EditorCamera						ActiveCamera;
	

	
	void Editor()
	{
		Print("Editor");		
		EditorEventHandler 			= new EditorEvents();
		EditorObjects 				= new EditorObjectSet();
		SelectedObjects				= new EditorObjectSet();
		SessionCache 				= new set<ref EditorObjectLink>();
		EditorActionStack 			= new set<ref EditorAction>();
		
		
		m_UIManager = GetGame().GetUIManager();
		
		ActiveEditorUI = new EditorUI();
		EditorUIWidget = GetGame().GetWorkspace().CreateWidgets(layout_dir + "Editor.layout");
		EditorUIWidget.GetScript(ActiveEditorUI);
	
		ActiveCamera = GetGame().CreateObject("EditorCamera", Vector(0, 50, 0), false);
		ActiveCamera.SetActive(true);
		
		ActiveEditorUI.GetMapWidget().AddChild(ActiveCamera.GetMapMarker());
		
		LoadPlacedObjects();
		EditorSettings.Load();
				
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Insert(Update);
	}
		
	void ~Editor()
	{
		Print("~Editor");
		GetGame().GetUpdateQueue(CALL_CATEGORY_GUI).Remove(Update);
	}
	
	static void InsertAction(EditorAction target)
	{

		for (int i = 0; i < EditorActionStack.Count(); i++) 
			if (EditorActionStack[i].was_undone) 
				delete EditorActionStack[i];
		
		EditorActionStack.InsertAt(target, 0);
	}

	

	
	void Undo()
	{
		Print("Editor::Undo");
		foreach (EditorAction action: EditorActionStack) {
			if (!action.was_undone) {
				action.CallUndo();
				return;
			}
		}	
	}
	
	void Redo()
	{
		Print("Editor::Redo");

		
		for (int i = EditorActionStack.Count() - 1; i >= 0; i--) {
			EditorAction action = EditorActionStack.Get(i);
			if (action == null) continue;
			if (action.was_undone) {
				action.CallRedo();
				return;
			}
		}
	}
	
	
	
	void Cut()
	{
		EditorAction action = new EditorAction("Create", "Delete");
		CopyCache = new EditorObjectSet();
		foreach (int id, EditorObject selected_object: SelectedObjects) {
			CopyCache.Insert(id, selected_object);
			GetGame().ObjectDelete(selected_object);
		} 
		
		InsertAction(action);
		
	}
	
	void Copy()
	{
		CopyCache = new EditorObjectSet();
		foreach (int id, EditorObject selected_object: SelectedObjects) {
			CopyCache.Insert(id, selected_object);			
		}
	}
	
	void Paste()
	{
		ClearSelections();
		vector avg_position;
		foreach (EditorObject copy_object: CopyCache) {
			avg_position += copy_object.GetPosition();
		}
		
		avg_position[0] = avg_position[0] / CopyCache.Count();
		avg_position[1] = avg_position[1] / CopyCache.Count();
		avg_position[2] = avg_position[2] / CopyCache.Count();
		
		foreach (EditorObject editor_object: CopyCache) {
			EditorObject result = CreateObject(editor_object.GetType(), avg_position - editor_object.GetPosition() + CurrentMousePosition, editor_object.GetOrientation());
			result.Select();
		}
	}	

	
	
	static EditorObject EditorObjectFromObject(Object target)
	{
		foreach (EditorObject editor_object: EditorObjects) {
			if (editor_object.GetObject() == target) {
				return editor_object;
			}
		}
		
		return null;
	}
	
	
	bool exit_condition = false;
	void Update() 
	{
		set<Object> obj = new set<Object>();
		int x, y;
		GetMousePos(x, y);
		CurrentMousePosition = MousePosToRay(obj);
		
		if (IsPlacing()) {
			if (ActiveEditorUI.IsMapOpen()) {
				vector pos = ActiveEditorUI.GetMapWidget().ScreenToMap(Vector(x, y, 0));
				pos[1] = GetGame().SurfaceY(pos[0], pos[2]);
				ObjectInHand.SetProjectionPosition(pos);
				ObjectInHand.SetProjectionOrientation(GetGame().SurfaceGetNormal(pos[0], pos[2]));
				
			} else {
				vector v = MousePosToRay(obj, ObjectInHand.GetProjectionEntity());
				vector size = ObjectGetSize(ObjectInHand.GetProjectionEntity());
				v[1] = v[1] + size[1]/2;
				ObjectInHand.SetProjectionPosition(v);
				ObjectInHand.SetProjectionOrientation(GetGame().SurfaceGetNormal(v[0], v[2]));
			}
			
		} else {
			
			Object target = obj.Get(0);
			
			if (target != null) {
				if (target != ObjectUnderCursor) {
					if (ObjectUnderCursor != null) OnMouseExitObject(ObjectUnderCursor, x, y);
					OnMouseEnterObject(target, x, y);
					ObjectUnderCursor = target;
				}
				
			} else if (ObjectUnderCursor != null) {
				exit_condition = OnMouseExitObject(ObjectUnderCursor, x, y);
				ObjectUnderCursor = null;
			}			
		}
	}
	
	
	
	bool OnMouseEnterObject(Object target, int x, int y)
	{
		//Print("Editor::OnMouseEnterObject");
		EditorObjectUnderCursor = EditorObjectFromObject(target);
		if (EditorObjectUnderCursor != null) {
			return EditorObjectUnderCursor.OnMouseEnter(x, y);
		}
		
		
		return true;
		
	}
	
	bool OnMouseExitObject(Object target, int x, int y)
	{
		if (EditorObjectUnderCursor != null)
			EditorObjectUnderCursor.OnMouseLeave(x, y);
		EditorObjectUnderCursor = null;
		
		return true;
		
	}
	
	
	// can we refactor this?
	static void CreateObjectInHand(string name)
	{
		EntityAI obj = GetGame().CreateObject(name, "0 0 0");
		set<Object> o;
		vector v =  MousePosToRay(o);
		vector size = ObjectGetSize(obj);
		v[1] = v[1] + size[1]/2;
		
		ObjectInHand = new EditorHologram(null, v, obj);
		
		GetGame().ObjectDelete(obj);
	}
	
	
	
	static EditorObject CreateObject(string name, vector position, vector orientation)
	{
		Print("Editor::CreateObject");
	
		
		EditorObject editor_object = GetGame().CreateObjectEx("EditorObject", position, ECE_KEEPHEIGHT|ECE_NOSURFACEALIGN|ECE_TRACE);		
		editor_object.SetObject(name);
		editor_object.SetOrientation(orientation);
		editor_object.CreateBoundingBox();
		editor_object.Update();
		
		EditorObjectLink link = new EditorObjectLink(editor_object);
		SessionCache.Insert(link);
		
		Editor.EditorObjects.Insert(editor_object.GetID(), editor_object);
		
		EditorEventHandler.ObjectCreateInvoke(null, editor_object);
		
		return editor_object;
	}
	
	
	

	static void ClearSelections()
	{
		Print("Editor::ClearSelections");		
		foreach (EditorObject editor_object: EditorObjects) {
			editor_object.Deselect();
		}
	}
	
	void DeleteObjects(EditorObjectSet target)
	{
		Print("Editor::DeleteObject");
	
		EditorAction action = new EditorAction("Create", "Delete");
		foreach (EditorObject selected_object: target) {
			EditorObjects.Remove(selected_object.GetID());
			GetGame().ObjectDelete(selected_object);
		}
		
		
		InsertAction(action);
	}

	
	
	
	bool OnMouseButtonDown(int x, int y, int button)
	{
		Input input = GetGame().GetInput();
		if (button == 2) {
			
			if (input.LocalValue("UAWalkRunTemp")) {
				set<Object> oop;
				vector v = MousePosToRay(oop);
				LightingBolt.CreateLightning(v, 1);		
			} else {				
				set<Object> o;
				vector pos = MousePosToRay(o);
				vector current = ActiveCamera.GetPosition();
				pos[1] = current[1];
				ActiveCamera.SetPosition(pos);
			}
	
			
		}
		
		return true;
	}
	
	bool OnKeyPress(int key) 
	{
		Input input = GetGame().GetInput();
		switch (key) {

			case KeyCode.KC_DELETE: {
				DeleteObjects(SelectedObjects);
				ClearSelections();
				return true;
			}

			
			case KeyCode.KC_Z: {
				if (input.LocalValue("UAWalkRunTemp")) {
					Undo();
					return true;
				}
				break;
			}
			
			case KeyCode.KC_Y: {
				if (input.LocalValue("UAWalkRunTemp")) {
					Redo();
					return true;
				}
				break;
			}
			
			case KeyCode.KC_A: {
				if (input.LocalValue("UAWalkRunTemp")) {
					ClearSelections();
					foreach (EditorObject editor_object: EditorObjects) 
						editor_object.Select();
					return true;
				}
				break;
			}
			
			case KeyCode.KC_X: {
				if (input.LocalValue("UAWalkRunTemp")) {
					Cut();
					return true;
				}
				break;
			}
			
			case KeyCode.KC_C: {
				if (input.LocalValue("UAWalkRunTemp")) {
					Copy();
					return true;
				}
				break;
			}
			
			case KeyCode.KC_V: {
				if (input.LocalValue("UAWalkRunTemp")) {
					Paste();
					return true;
				}
				break;
			}
			
			case KeyCode.KC_E: {
				if (input.LocalValue("UAWalkRunTemp")) {
					//m_EditorUI.ShowExportWindow();
					return true;
				}
				break;
			}
		}
		
		return ActiveEditorUI.OnKeyPress(key));
	}
	
	
	static bool IsPlacing()
	{
		return ObjectInHand != null;
	}
	
	static void LoadPlacedObjects()
	{
		// todo: allow people to open their own savefiles
		//PlacedObjects = new array<Object>;
	}
	
	static void ExportToFile(string filename = "$profile:editor_export.txt", ExportMode mode = ExportMode.TERRAINBUILDER)
	{
		DeleteFile(filename);
		FileHandle handle = OpenFile(filename, FileMode.WRITE | FileMode.APPEND);
		if (handle == 0) {
			Print("ExportToFile Failed: 0");
			return;
		}
		
		foreach (EditorObject editor_object: EditorObjects) {
						
			vector position = editor_object.GetObject().GetPosition();
			vector orientation = editor_object.GetObject().GetOrientation();
			string line;
			
			vector terrainbuilder_offset = Vector(200000, 0, 0);
			
			switch (mode) {
				
				case (ExportMode.TERRAINBUILDER): {
					// "construction_house2";206638.935547;6076.024414;146.000015;0.000000;0.000000;1.000000;
					position[1] = GetGame().SurfaceY(position[0], position[2]) - position[1];
					line = string.Format("\"%1\";%2;%4;%3;%6;%5;%7;", editor_object.GetModelName(), position[0] + terrainbuilder_offset[0], position[1] + terrainbuilder_offset[1], position[2] + terrainbuilder_offset[2], orientation[0], orientation[1], orientation[2]);
					FPrintln(handle, line);
					break;
				}
					
				case (ExportMode.COMFILE): {
					// SpawnObject("Land_Construction_House2", "6638.935547 7.190318 6076.024414", "146.000015 0.000000 0.000000")
					line = string.Format("SpawnObject(\"%1\", \"%2\", \"%3\");", editor_object.GetType(), position.ToString(false), orientation.ToString(false));
					FPrintln(handle, line);
					break;
				}
				
				case ExportMode.EXPANSION: {
					// Land_Construction_House2|13108.842773 10.015385 6931.083984|-101.999985 0.000000 0.000000
					//orientation = orientation.VectorToAngles();
					line = string.Format("%1|%2 %3 %4|%5 %6 %7", editor_object.GetType(), position[0], position[1], position[2], orientation[0], orientation[1], orientation[2]);
					FPrintln(handle, line);
					break;
				}
				
				default: {
					FPrintln(handle, "Line Export Failure");
					break;
				}
				
				
				
			} 
		}
		CloseFile(handle);
	}
}


enum ExportMode {
	TERRAINBUILDER,
	COMFILE, 
	EXPANSION
}


enum HeightType {
	ASL,
	ATL
}

class ExportSettings
{
	static HeightType ExportHeightType;
	
}




