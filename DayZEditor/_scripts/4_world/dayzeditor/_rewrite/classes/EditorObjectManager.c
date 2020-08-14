



class EditorObjectManager
{
	

	
	
	// Getters

	
	void EditorObjectManager() 
	{ 
		Print("EditorObjectManager");
		m_PlacedObjects 	= new EditorObjectSet();
		m_SelectedObjects	= new EditorObjectSet();
		m_SessionCache 		= new EditorObjectDataSet();
		m_ActionStack 		= new set<ref EditorAction>();
		
		EditorEvents.OnObjectSelected.Insert(OnObjectSelected);
		EditorEvents.OnObjectDeselected.Insert(OnObjectDeselected);
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
	
	static int GetPlaceableObjects(out EditorPlaceableListItemSet placeable_objects) 
	{ 
		TStringArray paths = new TStringArray;
		paths.Insert(CFG_VEHICLESPATH);

		for (int i = 0; i < paths.Count(); i++)	{
			string Config_Path = paths.Get(i);			
			
		    for (int j = 0; j < g_Game.ConfigGetChildrenCount(Config_Path); j++) {
				string Config_Name, Base_Name;
		        g_Game.ConfigGetChildName(Config_Path, j, Config_Name); 
				
				EditorPlaceableObjectData placeable_object_data = new EditorPlaceableObjectData(Config_Name, Config_Path);
				placeable_objects.Insert(new EditorPlaceableListItem(placeable_object_data));
		    }
		}
		

		
		return placeable_objects.Count();
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
	
	
	
	
	
	
		
		


	
	static void ToggleSelection(EditorObject target)
	{
		if (target.IsSelected())
			EditorEvents.DeselectObject(null, target);
		else EditorEvents.SelectObject(null, target);
	}
	

	
	
	void CutSelection()
	{
		EditorPrint("EditorObjectManager::CutSelection");
		CopySelection();
		DeleteObjects(m_SelectedObjects);
	}
	
	
	void CopySelection()
	{
		EditorPrint("EditorObjectManager::CopySelection");
		
		ref array<ref EditorObjectData>> world_objects = new ref array<ref EditorObjectData>>();
		
		vector avg_position;
		foreach (int id, EditorObject copy_object: m_SelectedObjects)
			avg_position += copy_object.GetPosition();
		
		for (int i = 0; i < 3; i++)
			avg_position[i] = avg_position[i] / m_SelectedObjects.Count();
		
		foreach (int idx, EditorObject selected_object: m_SelectedObjects) {
			EditorObjectData data = selected_object.GetData();
			data.Position = selected_object.GetPosition() - avg_position;
			world_objects.Insert(data);
		}
	
		GetGame().CopyToClipboard(JsonFileLoader<array<ref EditorObjectData>>.JsonMakeData(world_objects));
	}
	
	void PasteSelection()
	{
		EditorPrint("EditorObjectManager::PasteSelection");
		string clipboard_text;
		GetGame().CopyFromClipboard(clipboard_text);
		
		ref array<ref EditorObjectData>> data = new array<ref EditorObjectData>>();
		JsonFileLoader<array<ref EditorObjectData>>.JsonLoadData(clipboard_text, data);
		if (data.Count() == 0) return;

		EditorEvents.ClearSelection(this);
				
		foreach (ref EditorObjectData pasted_object: data) {
			
			vector position;
			if (GetEditor().GetUIManager().GetEditorUI().IsMapOpen()) {
				MapWidget map_widget = GetEditor().GetUIManager().GetEditorUI().GetMapWidget();
				int x, y;
				GetCursorPos(x, y);
				position = pasted_object.Position + map_widget.ScreenToMap(Vector(x, y, 0));
			} else {
				position = pasted_object.Position + Editor.CurrentMousePosition;
			}
			
			vector transform[4] = {
				"1 0 0",
				"0 1 0",
				"0 0 1",
				position
			};
			
			EditorObject editor_object = CreateObject(EditorObjectData.Create(pasted_object.Type, position, pasted_object.Orientation, pasted_object.Flags));
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
	
	// EditorObject.WorldObject.GetID(), EditorObject.GetID()


	

	
}

