

class EditorBrushDialogController: Controller
{
	string name;
	
	
	protected EditorBrushData m_BrushData;
	
	void SetBrushData(EditorBrushData brush_data)
	{
		m_BrushData = brush_data;
		
		name = m_BrushData.Name;
	}
	

}


class EditorBrushDialog: EditorDialog
{
	
	protected ref EditorBrushDialogController m_EditorBrushDialogController;
	
	protected ref EditorPrefabGroup m_BrushObjects;
	
	void EditorBrushDialog(EditorBrushData brush_data)
	{
		EditorLog.Trace("EditorBrushDialog"); 
		
		m_EditorBrushDialogController = new EditorBrushDialogController();
		m_EditorBrushDialogController.SetBrushData(brush_data);
		
		EditorPrefabGroup brush_settings = new EditorPrefabGroup("Brush Settings");
		brush_settings.AddPrefab(new EditorPrefabEditText("Name", "name"));
		
		m_BrushObjects = new EditorPrefabGroup("Brush Objects");
		
		brush_settings.SetController(m_EditorBrushDialogController);
		m_BrushObjects.SetController(m_EditorBrushDialogController);
		
		
		AddContent(brush_settings);
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
		m_BrushObjects.AddPrefab(new EditorPrefabEditText("Test", "test"));
	}
	
}

