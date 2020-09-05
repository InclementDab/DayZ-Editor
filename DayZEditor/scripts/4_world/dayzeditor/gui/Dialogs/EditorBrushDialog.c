

class EditorBrushDialogController: Controller
{
	string BrushName;
	
	
	protected EditorBrushData m_BrushData;
	
	EditorBrushObject BrushObjectArray[1024];
	
	void SetBrushData(EditorBrushData brush_data)
	{
		m_BrushData = brush_data;
		
		BrushName = m_BrushData.Name;
		BrushObjectArray = m_BrushData.PlaceableObjectTypes;
	}
	
}


class EditorBrushDialog: EditorDialog
{
	
	protected ref EditorBrushDialogController m_EditorBrushDialogController;
	
	protected ref EditorPrefabGroup m_BrushSettings;
	protected ref EditorPrefabGroup m_BrushObjects;
	
	protected EditorBrushData m_BrushData;
	
	void EditorBrushDialog(EditorBrushData brush_data)
	{
		EditorLog.Trace("EditorBrushDialog"); 
		
		m_BrushData = brush_data;
		m_EditorBrushDialogController = new EditorBrushDialogController();
		m_EditorBrushDialogController.SetBrushData(m_BrushData);
		
		m_BrushSettings = new EditorPrefabGroup("Brush Settings");
		m_BrushSettings.AddPrefab(new EditorPrefabEditText("Name", "BrushName"));
		
		m_BrushObjects = new EditorPrefabGroup("Brush Objects");
		
		
		
		for (int i = 0; i < m_BrushData.PlaceableObjectTypeCount; i++) {
			m_BrushObjects.AddPrefab(new EditorPrefabEditText("Brush", "BrushObjectArray", i));
		}
		
		m_BrushSettings.SetController(m_EditorBrushDialogController);
		m_BrushObjects.SetController(m_EditorBrushDialogController);
		
		AddContent(m_BrushSettings);
		AddContent(m_BrushObjects);
			
		AddButton("Save", "SaveCallback");
		AddButton("Export", "ExportCallback");
		AddButton("Close", "CloseDialog");
		
	}
	
	void SaveCallback()
	{
		EditorLog.Trace("EditorBrushDialog::SaveCallback"); 
	}
	
	
	void ExportCallback()
	{
		EditorLog.Trace("EditorBrushDialog::ExportCallback");
		m_BrushObjects.AddPrefab(new EditorPrefabEditText("Brush", "BrushObjectArray", m_BrushData.PlaceableObjectTypeCount));
	}
	
}

