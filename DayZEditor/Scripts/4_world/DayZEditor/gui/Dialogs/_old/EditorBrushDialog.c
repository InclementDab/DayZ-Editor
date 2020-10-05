

class EditorBrushDialogController: Controller
{
	string BrushName;
	bool AddBrushData;
	
	protected EditorBrushData m_BrushData;

	ref ObservableCollection<ref EditorBrushObject> BrushObjects;
	
	ref EditorBrushObjectArray BrushObjectArray;
	
	void SetBrushData(EditorBrushData brush_data)
	{
		m_BrushData = brush_data;
		
		BrushName = m_BrushData.Name;
		BrushObjectArray = m_BrushData.PlaceableObjectTypes;
		
		BrushObjects = new ObservableCollection<ref EditorBrushObject>(this);
	}
	
	override void PropertyChanged(string property_name)
	{
		
		switch (property_name) {
			
			case "AddBrushData": {
				if (AddBrushData) {
					BrushObjects.Insert(new EditorBrushObject("", 1, 0));
				}
				
				break;
			}
			
		}
	}
	
}

/*
class EditorBrushDialog: EditorDialog
{
	
	protected ref EditorBrushDialogController m_EditorBrushDialogController;
	
	//protected ref EditorPrefabGroup m_BrushSettings;
	//protected ref EditorPrefabGroup m_BrushObjects;
	
	protected EditorBrushData m_BrushData;
	
	void EditorBrushDialog(Widget parent = null) 
	{
		EditorLog.Trace("EditorBrushDialog"); 
				
		AddButton("Save", SaveCallback);
		AddButton("Export", ExportCallback);
		AddButton("Close", CloseDialog);	
	}
	
	void SetEditorBrushData(EditorBrushData brush_data)
	{
		m_BrushData = brush_data;
		m_EditorBrushDialogController = new EditorBrushDialogController();
		m_EditorBrushDialogController.SetBrushData(m_BrushData);
		
		/*m_BrushSettings = new EditorPrefabGroup("Brush Settings");
		m_BrushSettings.AddPrefab(new EditorPrefabEditText("Name", "BrushName"));
		
		m_BrushObjects = new EditorPrefabGroup("Brush Objects");		
		m_BrushSettings.SetController(m_EditorBrushDialogController);
		m_BrushObjects.SetController(m_EditorBrushDialogController);
		
		
		
		AddContent(m_BrushSettings);
		AddContent(m_BrushObjects); 
		
		//EditorPrefabButton btn = new EditorPrefabButton("Add Brush...", "AddBrushData");
		//btn.SetController(m_EditorBrushDialogController);
		//AddContent(btn);
	}
	
	void SaveCallback()
	{
		EditorLog.Trace("EditorBrushDialog::SaveCallback"); 
	}
	
	
	void ExportCallback()
	{
		EditorLog.Trace("EditorBrushDialog::ExportCallback");
	}
	
}*/

