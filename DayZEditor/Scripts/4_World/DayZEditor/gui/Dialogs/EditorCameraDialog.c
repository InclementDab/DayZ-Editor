class EditorCameraDialog: EditorDialogBase
{
	static bool OpenCameraGroup = true;
	static bool OpenColorCorrectionGroup;
	static bool OpenAdvancedGroup;
	
	private EditorCamera m_EditorCamera;
	
	protected ref GroupPrefab m_CameraGroup;
	protected ref GroupPrefab m_ColorCorrectionGroup;
	protected ref GroupPrefab m_AdvancedGroup;
	
	void EditorCameraDialog(string title) 
	{
		EditorLog.Trace("EditorCameraDialog");
			
		m_EditorCamera = GetEditor().GetCamera();
		/*
		map<int, ref PPEClassBase> class_map;
		EnScript.GetClassVar(PPEManagerStatic.GetPPEManager(), "m_PPEClassMap", 0, class_map);
		foreach (int id, PPEClassBase clascs: class_map) {
		    Print(id);
		    typename ty = clascs.Type();
			
		    map<int,ref PPEMatClassParameterCommandData> parameter_data;
		    EnScript.GetClassVar(clascs, "m_MaterialParamMapStructure", 0, parameter_data);
		
			GroupPrefab group_prefab = new GroupPrefab(ty.ToString(), null, string.Empty);
			
			map<int, string> param_map = new map<int, string>();
		    for (int i = 0; i < ty.GetVariableCount(); i++) {
		        string variable_name = ty.GetVariableName(i);
				if (variable_name.Contains("PARAM_")) {
					int var;
					ty.GetVariableValue(clascs, i, var);
					param_map[var] = variable_name;
					
					switch (parameter_data[var].Type()) {
						case PPEMatClassParameterColor: {
							group_prefab.Insert(new ColorPickerPrefab(variable_name, this, string.Empty));
							break;	
						}
						
						case PPEMatClassParameterFloat: {
							group_prefab.Insert(new SliderPrefab(variable_name, this, string.Empty));
							break;
						}
					}
				}
		    }
			
		    
		    foreach (int id2, PPEMatClassParameterCommandData data: parameter_data) {
		        Print(id2);
				Print(data);
		    }
			
			AddContent(group_prefab);
		}*/
		
		m_CameraGroup = new GroupPrefab("Camera", m_EditorCamera, string.Empty);
		m_CameraGroup.Insert(new SliderPrefab("FOV", m_EditorCamera, "FOV", 0, 2));
		m_CameraGroup.Insert(new SliderPrefab("Gaussian Blur", m_EditorCamera, "Blur", 0, 1));
		m_CameraGroup.Insert(new SliderPrefab("Near Plane", m_EditorCamera, "NearPlane",  0, 1));
		m_CameraGroup.Insert(new SliderPrefab("DOF Distance", m_EditorCamera, "DOFDistance", 0, 500));
		m_CameraGroup.Insert(new SliderPrefab("DOF Blur", m_EditorCamera, "DOFBlur", 0, 1));
		m_CameraGroup.Insert(new SliderPrefab("Vignette", m_EditorCamera, "Vignette", 0, 1));
		m_CameraGroup.Insert(new SliderPrefab("Sharpness", m_EditorCamera, "Sharpness", 0, 1));
		m_CameraGroup.Insert(new SliderPrefab("Exposure", m_EditorCamera, "Exposure", 0, 3));
		m_CameraGroup.Insert(new SliderPrefab("Smoothing", m_EditorCamera, "Smoothing", 0, 1));
		m_CameraGroup.Open(OpenCameraGroup);
				
		m_ColorCorrectionGroup = new GroupPrefab("Filters", m_EditorCamera, string.Empty);
		m_ColorCorrectionGroup.Insert(new ColorPickerPrefab("Color Correction", m_EditorCamera, "ColorCorrection", false));
		m_ColorCorrectionGroup.Open(OpenColorCorrectionGroup);
		
		m_AdvancedGroup = new GroupPrefab("Advanced", m_EditorCamera, string.Empty);
		m_AdvancedGroup.Insert(new CheckBoxPrefab("Hide Cursor On Drag", m_EditorCamera, "HideCursorOnDrag"));
		m_AdvancedGroup.Open(OpenAdvancedGroup);
		
		AddContent(m_CameraGroup);
		AddContent(m_ColorCorrectionGroup);
		AddContent(m_AdvancedGroup);
		
		AddButton(DialogResult.OK);
		AddButton(new DialogButton("Default", "ResetDefaultExecute"));
		//AddButton(DialogResult.Cancel);
	}
	
	void ~EditorCameraDialog()
	{
		OpenCameraGroup = m_CameraGroup.IsOpen();
		OpenColorCorrectionGroup = m_ColorCorrectionGroup.IsOpen();
		OpenAdvancedGroup = m_AdvancedGroup.IsOpen();
		
		delete m_CameraGroup;
		delete m_ColorCorrectionGroup;
		delete m_AdvancedGroup;
	}

	void ResetDefaultExecute()
	{
	}
	
	override string GetIcon() 
	{
		return "set:dayz_editor_gui image:camera_alt";
	}
}