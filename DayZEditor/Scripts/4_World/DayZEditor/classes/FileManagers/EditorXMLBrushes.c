// abstract to EditorXMLCallback
class XMLEditorBrushes: XMLCallback
{
	private autoptr ObservableCollection<ref EditorBrushData> m_Data;
	
	void XMLEditorBrushes(ObservableCollection<ref EditorBrushData> data) 
	{
		EditorLog.Trace("XMLEditorBrushes");
		m_Data = data;
	}
		
	override void OnStart(ref XMLDocument document)
	{
		EditorLog.Trace("XMLEditorBrushes::OnStart");
	}
	
	override void OnSuccess(ref XMLDocument document)
	{		
		EditorLog.Info("XMLEditorBrushes::OnSuccess");
		ref set<string> object_type_list = new set<string>();
		
		// <BrushTypes>
		for (int i = 0; i < document.Get(1).GetContent().Count(); i++) {
			
			XMLTag brush = document.Get(1).GetContent().Get(i);
			
			EditorBrushData brush_settings = new EditorBrushData();
			brush_settings.Name = brush.GetAttribute("name").ValueAsString();
			

			if (brush.GetAttribute("class")) {
				EditorLog.Info("XMLEditorBrushes:: Code defined brush found!");
				brush_settings.BrushClassName = brush.GetAttribute("class").ValueAsString().ToType();
				
			} else {
			
				// <BrushObject>
				for (int j = 0; j < brush.GetContent().Count(); j++) {
					XMLTag brush_object = brush.GetContent().Get(j);
					
					// attributes
					string object_type;
					float z_offset;
					float object_frequency;
					
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
					} else {
						z_offset = 0;
					}
					
					// frequency attribute
					if (brush_object.GetAttribute("frequency")) {
						object_frequency = brush_object.GetAttribute("frequency").ValueAsFloat();
					} else {
						object_frequency = 1.0;
					}
					
					brush_settings.InsertPlaceableObject(new EditorBrushObject(object_type, object_frequency, z_offset));
				}
			}
				
			GetEditor().GetEditorHud().GetTemplateController().GetToolbarController().BrushTypeBoxData.Insert(brush_settings);
		}
	}
	
	override void OnFailure(ref XMLDocument document)
	{
		EditorLog.Error("XMLEditorBrushes::OnFailure");
	}
}