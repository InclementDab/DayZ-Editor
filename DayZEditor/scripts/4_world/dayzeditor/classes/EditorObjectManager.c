
class EditorObjectSet: map<int, EditorObject>
{
	
	void ~EditorObjectSet()
	{
		foreach (EditorObject obj: this)
			delete obj;
	}
	
	bool InsertEditorObject(EditorObject target)
	{
		return Insert(target.GetID(), target);
	}
	
	void RemoveEditorObject(EditorObject target)
	{
		Remove(target.GetID());
	}
	
}


//typedef ref map<int, ref EditorObject> EditorObjectSet;

class EditorObjectManager: Managed
{
	
	private ref EditorObjectSet 			m_PlacedObjects;
	private ref EditorObjectSet				m_SelectedObjects;
	private ref map<int, ref EditorObject> 	m_SessionCache;
	private ref set<ref EditorAction> 		m_ActionStack;
	
	
	// Getters
	ref set<ref EditorAction> GetActionStack() { return m_ActionStack; }
	ref EditorObjectSet GetSelectedObjects() { return m_SelectedObjects; }
	ref EditorObjectSet GetPlacedObjects() { return m_PlacedObjects; }
	ref map<int, ref EditorObject> GetSessionCache() { return m_SessionCache; }
	
	void EditorObjectManager() 
	{ 
		Print("EditorObjectManager");
		m_PlacedObjects 	= new EditorObjectSet();
		m_SelectedObjects	= new EditorObjectSet();
		//m_SessionCache 		= new set<ref EditorObjectLink>();
		m_SessionCache 		= new map<int, ref EditorObject>();
		m_ActionStack 		= new set<ref EditorAction>();
		
		
		EditorEvents.OnObjectSelected.Insert(OnObjectSelected);
		EditorEvents.OnObjectDeselected.Insert(OnObjectDeselected);
		EditorEvents.OnSelectionCleared.Insert(OnSelectionCleared);
		
		EditorEvents.OnObjectCreated.Insert(OnObjectCreated);
		EditorEvents.OnObjectDeleted.Insert(OnObjectDeleted);
		
	}
	
	void ~EditorObjectManager() 
	{
		Print("~EditorObjectManager");
		
		delete m_PlacedObjects;
		delete m_SelectedObjects;
		delete m_SessionCache;
		delete m_ActionStack;
	}
	
	static int GetPlaceableObjects(out EditorPlaceableObjectSet placeable_objects) 
	{ 
		TStringArray paths = new TStringArray;
		paths.Insert(CFG_VEHICLESPATH);

		for (int i = 0; i < paths.Count(); i++)	{
			string Config_Path = paths.Get(i);			
			
		    for (int j = 0; j < g_Game.ConfigGetChildrenCount(Config_Path); j++) {
				string Config_Name, Base_Name;
		        g_Game.ConfigGetChildName(Config_Path, j, Config_Name); 
				
				EditorPlaceableObject placeable_object = new EditorPlaceableObject(Config_Name, Config_Path);
				placeable_objects.Insert(placeable_object);
		    }
		}
		

		
		return placeable_objects.Count();
	}
	
	
	void CreateObjects(ref EditorObjectDataSet data_list)
	{
		EditorPrint("EditorObjectManager::CreateObjects");
		
		EditorAction action = new EditorAction("Delete", "Create");
		foreach (EditorObjectData editor_object_data: data_list) {
			
			editor_object_data.ID = m_SessionData.Count();
			m_SessionData.Insert(editor_object_data);
			EditorObject editor_object = new EditorObject(editor_object_data);
			m_SessionCache.Insert(editor_object.GetID(), editor_object);
			
			ref Param1<int> params = new Param1<int>(editor_object.GetID());
			action.InsertUndoParameter(editor_object, params);
			action.InsertRedoParameter(editor_object, params);
			
		}
		
		InsertAction(action);		
	}
	
	
	EditorObject CreateObject(ref EditorObjectData editor_object_data)
	{		
		EditorPrint("EditorObjectManager::CreateObject");
		
		editor_object_data.ID = m_SessionData.Count();
		m_SessionData.Insert(editor_object_data);
		EditorObject editor_object = new EditorObject(editor_object_data);
		m_SessionCache.Insert(editor_object.GetID(), editor_object);
		

		EditorAction action = new EditorAction("Delete", "Create");
		ref Param1<int> params = new Param1<int>(editor_object.GetID());
		action.InsertUndoParameter(editor_object, params);
		action.InsertRedoParameter(editor_object, params);
		InsertAction(action);

		return editor_object;
	}
	
	private void SetupObject(EditorObject target)
	{
		//EditorObjectLink link = new EditorObjectLink(target);
		//m_SessionCache.Insert(link);

		// Create Undo / redo action for creation
		//EditorAction action = new EditorAction("Delete", "Create");
		//action.InsertUndoParameter(link, null);
		//action.InsertRedoParameter(link, null);
		//InsertAction(action);
	}
	
	
	void OnObjectSelected(Class context, EditorObject target)
	{
		EditorPrint("EditorObjectManager::SelectObject");		
		m_SelectedObjects.InsertEditorObject(target);
	}
	
	void OnObjectDeselected(Class context, EditorObject target)
	{
		EditorPrint("EditorObjectManager::SelectObject");
		m_SelectedObjects.RemoveEditorObject(target);
	}	
	
	void OnObjectCreated(Class context, EditorObject target)
	{
		EditorPrint("EditorObjectManager::OnObjectCreated");
		m_SelectedObjects.InsertEditorObject(target);
		m_PlacedObjects.InsertEditorObject(target);
	}	
	
	void OnObjectDeleted(Class context, EditorObject target)
	{
		EditorPrint("EditorObjectManager::OnObjectDeleted");
		m_SelectedObjects.RemoveEditorObject(target);
		m_PlacedObjects.RemoveEditorObject(target);
	}
	
	
	
	int DeleteSelection()
	{
		Print("EditorObjectManager::DeleteObject");
		int result = m_SelectedObjects.Count();
		
		EditorAction action = new EditorAction("Create", "Delete");
		foreach (EditorObject selected_object: m_SelectedObjects) {
			action.InsertUndoParameter(selected_object, new Param1<int>(selected_object.GetID()));
			action.InsertRedoParameter(selected_object, new Param1<int>(selected_object.GetID()));
			delete selected_object;
		}
		
		InsertAction(action);
		//GetGame().ObjectDelete(GetEditor().GetTranslationWidget());
		return result;
	}
		
		

	
	void OnSelectionCleared(Class context)
	{
		Print("EditorObjectManager::ClearSelections");		
		foreach (EditorObject editor_object: m_SelectedObjects)
			EditorEvents.DeselectObject(this, editor_object);
	
	}
	
	void ToggleSelection(EditorObject target)
	{
		if (target.IsSelected())
			EditorEvents.DeselectObject(this, target);
		
		EditorEvents.SelectObject(this, target);
	}
	

	
	
	void CutSelection()
	{
		string copy;
		EditorObjectDataSet world_objects = new EditorObjectDataSet();
		foreach (int id, EditorObject selected_object: m_SelectedObjects) {
			world_objects.Insert(selected_object.GetData());		
		}
		copy = JsonFileLoader<EditorObjectDataSet>.JsonMakeData(world_objects);
		GetGame().CopyToClipboard(copy);
		DeleteSelection();
	}
	
	
	void CopySelection()
	{
		string copy;
		EditorObjectDataSet world_objects = new EditorObjectDataSet();
		foreach (int id, EditorObject selected_object: m_SelectedObjects) {
			world_objects.Insert(selected_object.GetData());			
		}
		copy = JsonFileLoader<EditorObjectDataSet>.JsonMakeData(world_objects);
		GetGame().CopyToClipboard(copy);
	}
	
	void PasteSelection()
	{
		string clipboard_text;
		GetGame().CopyFromClipboard(clipboard_text);
		
		EditorObjectDataSet data = new EditorObjectDataSet();
		JsonFileLoader<EditorObjectDataSet>.JsonLoadData(clipboard_text, data);
		if (data.Count() == 0) return;
		
		EditorEvents.ClearSelection(this);
		vector avg_position;

		foreach (EditorObjectData copy_object: data) {
			avg_position += copy_object.Position;
		}
		
		avg_position[0] = avg_position[0] / data.Count();
		avg_position[1] = avg_position[1] / data.Count();
		avg_position[2] = avg_position[2] / data.Count();
		
		foreach (EditorObjectData pasted_object: data) {
			
			
			vector position = avg_position - pasted_object.Position + Editor.CurrentMousePosition;
			vector transform[4] = {
				"1 0 0",
				"0 1 0",
				"0 0 1",
				position
			};
			
			EditorObject editor_object = CreateObject(pasted_object);
			float surfacey = GetGame().SurfaceY(position[0], position[2]);
			vector size = editor_object.GetSize();
			position[1] = surfacey + size[1] / 2;
			editor_object.SetPosition(position);
			editor_object.Update();
			
			if (EditorSettings.MAGNET_PLACEMENT) {
				
				set<Object> o;
				vector ground, ground_dir; int component;
				DayZPhysics.RaycastRV(position, position + transform[1] * -1000, ground, ground_dir, component, o, NULL, editor_object.GetWorldObject(), false, true); // set to ground only
				vector surface_normal = GetGame().SurfaceGetNormal(position[0], position[2]);
				editor_object.PlaceOnSurfaceRotated(transform, position, surface_normal[0] * -1, surface_normal[2] * -1, 0, EditorSettings.MAGNET_PLACEMENT);
				transform[3] = transform[3] - transform[1] * vector.Distance(ground, position);
				
				editor_object.SetTransform(transform);
				editor_object.Update();
				
			}
			
			EditorEvents.SelectObject(this, editor_object);
		}	
	}
	
	
	void InsertAction(EditorAction target)
	{
		int count = m_ActionStack.Count();
		for (int i = 0; i < count; i++) {
			if (m_ActionStack[i].IsUndone()) {
				m_ActionStack.Remove(i);
				i--; count--;
			}
		}
			
		// Adds to bottom of stack
		m_ActionStack.InsertAt(target, 0);
		
		// debug
		GetEditor().GetUIManager().GetEditorUI().m_DebugActionStack.ClearItems();
		
		for (int debug_i = 0; debug_i < m_ActionStack.Count(); debug_i++) {
			GetEditor().GetUIManager().GetEditorUI().m_DebugActionStack.AddItem(m_ActionStack[debug_i].GetName(), m_ActionStack[debug_i], 0);
		}
	}
	
	EditorObject GetEditorObjectFromListItem(EditorPlacedListItem list_item)
	{
		foreach (EditorObject editor_object: m_PlacedObjects)
			if (editor_object == list_item.GetEditorObject())
				return editor_object;
		
		
		Print("EditorObjectManager::GetEditorObjectFromListItem Item Not Found!");
		return null;
	}
	
	
	bool CheckIfRootIsSelected(Widget root)
	{
		foreach (EditorObject editor_object: m_SelectedObjects)
			if (editor_object.IsSelected() && editor_object.IsRootSelected(root))
				return true;
		
		return false;
	}
	
	EditorObject GetFromUILinkedRoot(Widget root)
	{
		Print("EditorObjectManager::GetFromObjectRoot");
		foreach (EditorObject editor_object: m_PlacedObjects)
			if (editor_object.IsRootSelected(root))
				return editor_object;
		
		
		Print("EditorObjectManager::GetFromUILinkedRoot: Item Not Found!");
		return null;
	}
	


	
	// O(n) shit :)
	EditorObject GetEditorObject(Object target)
	{
		foreach (ref EditorObject editor_object: m_PlacedObjects) 
			if (editor_object.GetWorldObject() == target) 
				return editor_object;
		
		return null;
	}
	
	EditorObject GetEditorObject(int id)
	{
		return m_PlacedObjects.Get(id);
	}
	
	
	EditorObject GetSessionObjectById(int id) { return m_SessionCache.Get(id); }
	void SetSessionObjectById(int id, EditorObject data) { m_SessionCache.Set(id, data); }
	
	
	private ref array<ref EditorObjectData> m_SessionData = new array<ref EditorObjectData>();
	EditorObjectData GetSessionDataById(int id)
	{
		foreach (EditorObjectData data: m_SessionData) {
			if (data.ID == id)
				return data;
		}
		
		Print("Session Data Not Found!");
		return null;
	}
	
	void SetSessionDataById(int id, EditorObjectData data)
	{
		m_SessionData.Set(id, data);
	}
	
	
	
	

}