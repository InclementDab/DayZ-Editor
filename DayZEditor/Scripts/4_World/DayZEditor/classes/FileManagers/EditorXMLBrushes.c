// abstract to EditorXMLCallback
class XMLEditorBrushes: XMLCallback
{
	private ObservableCollection<ref EditorBrushData> m_Data;

	void XMLEditorBrushes(ObservableCollection<ref EditorBrushData> data) 
	{
		EditorLog.Trace("XMLEditorBrushes");
		m_Data = data;
	}
		
	override void OnStart(XMLDocument document)
	{
		EditorLog.Trace("XMLEditorBrushes::OnStart");
		EditorLog.Info("Loading Brushes...");
	}
	
	override void OnSuccess(XMLDocument document)
	{		
		EditorLog.Trace("XMLEditorBrushes::OnSuccess");
		
		set<string> object_type_list = new set<string>();
		
		// <BrushTypes>
		for (int i = 0; i < document.Get(1).GetContent().Count(); i++) {
			
			XMLTag brush = document.Get(1).GetContent().Get(i);
			
			EditorBrushData brush_settings = new EditorBrushData();
			brush_settings.Name = brush.GetAttribute("name").ValueAsString();
			
			if (brush.GetAttribute("class")) {
				brush_settings.BrushClassName = brush.GetAttribute("class").ValueAsString().ToType();
				if (brush_settings.BrushClassName) {
					EditorLog.Info("Code Defined brush loaded: %1", brush_settings.BrushClassName.ToString());
				}
				
			} else {
			
				// <BrushObject>
				for (int j = 0; j < brush.GetContent().Count(); j++) {
					XMLTag brush_object = brush.GetContent().Get(j);
					
					// attributes
					string object_type;
					float z_offset = 0;
					float object_frequency = 1.0;
					float minscale = 1, maxscale = 1;
					
					// type attribute
					XMLAttribute object_type_attribute = brush_object.GetAttribute("type");
					if (object_type_attribute == null) {
						EditorLog.Trace("XMLEditorBrushes:: Object type not specified, skipping...");
						continue;
					}
					
					object_type = object_type_attribute.ValueAsString();
					if (object_type_list.Insert(object_type) == -1) {
						EditorLog.Trace("XMLEditorBrushes:: Duplicate brush name found, skipping...");
						continue;
					}
					
					// Z Offset attribute
					if (brush_object.GetAttribute("zoffset")) {
						z_offset = brush_object.GetAttribute("zoffset").ValueAsFloat();
					}
					
					// frequency attribute
					if (brush_object.GetAttribute("frequency")) {
						object_frequency = brush_object.GetAttribute("frequency").ValueAsFloat();
					}
					
					// scale attributes
					if (brush_object.GetAttribute("minscale")) {
						minscale = brush_object.GetAttribute("minscale").ValueAsFloat();
					}
					
					if (brush_object.GetAttribute("maxscale")) {
						maxscale = brush_object.GetAttribute("maxscale").ValueAsFloat();
					}
					
					brush_settings.InsertPlaceableObject(new EditorBrushObject(object_type, object_frequency, z_offset, minscale, maxscale));
				}
			}
			
			m_Data.Insert(brush_settings);
		}
		
		EditorLog.Info("Loaded %1 Brushes!", m_Data.Count().ToString());
	}
	
	override void OnFailure(XMLDocument document)
	{
		EditorLog.Error("XMLEditorBrushes::OnFailure");
	}
}