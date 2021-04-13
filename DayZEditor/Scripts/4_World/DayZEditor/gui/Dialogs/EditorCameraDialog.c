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
						
		m_CameraGroup = new GroupPrefab("Camera", m_EditorCamera, string.Empty);
		m_CameraGroup.Insert(new SliderPrefab("Speed", m_EditorCamera, "Speed", m_EditorCamera.Speed, 0, 500));
		m_CameraGroup.Insert(new SliderPrefab("FOV", m_EditorCamera, "FOV",  m_EditorCamera.FOV, 0, 2));
		m_CameraGroup.Insert(new SliderPrefab("Gaussian Blur", m_EditorCamera, "Blur", m_EditorCamera.Blur, 0, 1));
		m_CameraGroup.Insert(new SliderPrefab("Near Plane", m_EditorCamera, "NearPlane",  m_EditorCamera.NearPlane, 0, 1));
		m_CameraGroup.Insert(new SliderPrefab("DOF Distance", m_EditorCamera, "DOFDistance", m_EditorCamera.DOFDistance, 0, 500));
		m_CameraGroup.Insert(new SliderPrefab("DOF Blur", m_EditorCamera, "DOFBlur", m_EditorCamera.DOFBlur, 0, 1));
		m_CameraGroup.Insert(new SliderPrefab("Vignette", m_EditorCamera, "Vignette", m_EditorCamera.Vignette, 0, 1));
		m_CameraGroup.Insert(new SliderPrefab("Sharpness", m_EditorCamera, "Sharpness", m_EditorCamera.Sharpness, 0, 1));
		m_CameraGroup.Insert(new SliderPrefab("Exposure", m_EditorCamera, "Exposure", m_EditorCamera.Exposure, 0, 1));
		m_CameraGroup.Insert(new SliderPrefab("Smoothing", m_EditorCamera, "Smoothing", m_EditorCamera.Smoothing, 0, 1));
		m_CameraGroup.Open(OpenCameraGroup);
		
		m_ColorCorrectionGroup = new GroupPrefab("Filters", m_EditorCamera, string.Empty);
		m_ColorCorrectionGroup.Insert(new SliderPrefab("Red", m_EditorCamera, "ColorizeRed", m_EditorCamera.ColorizeRed));
		m_ColorCorrectionGroup.Insert(new SliderPrefab("Green", m_EditorCamera, "ColorizeGreen", m_EditorCamera.ColorizeGreen));
		m_ColorCorrectionGroup.Insert(new SliderPrefab("Blue", m_EditorCamera, "ColorizeBlue", m_EditorCamera.ColorizeBlue));
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
		EditorLog.Trace("EditorCameraDialog::ResetDefaultExecute");
		
		m_EditorCamera.Speed = 60;
		m_EditorCamera.Blur = 0;
		m_EditorCamera.FOV = 1;
		m_EditorCamera.NearPlane = 0;
		
		m_EditorCamera.PropertyChanged("Speed");
		m_EditorCamera.PropertyChanged("Blur");
		m_EditorCamera.PropertyChanged("FOV");
		m_EditorCamera.PropertyChanged("NearPlane");
		
		PPEffects.ResetAll();
	}	
}