static float DEFAULT_FOV = -1;
static float DEFAULT_NEARPLANE = -1;


class EditorCameraDialog: EditorDialogBase
{

	private EditorCamera m_EditorCamera;
	
	void EditorCameraDialog(string title) 
	{
		EditorLog.Trace("EditorCameraDialog");
			
		m_EditorCamera = GetEditor().GetCamera();
		
		if (DEFAULT_FOV == -1) {
			DEFAULT_FOV = m_EditorCamera.FOV;
		}
		
		if (DEFAULT_NEARPLANE == -1) {
			DEFAULT_NEARPLANE = m_EditorCamera.NearPlane;
		}
				
		GroupPrefab camera_group = new GroupPrefab("Camera", m_EditorCamera, string.Empty);
		camera_group.Insert(new SliderPrefab("Speed", m_EditorCamera, "Speed", m_EditorCamera.Speed, 0, 500));
		camera_group.Insert(new SliderPrefab("FOV", m_EditorCamera, "FOV",  m_EditorCamera.FOV, 0, 2));
		camera_group.Insert(new SliderPrefab("Gaussian Blur", m_EditorCamera, "Blur", m_EditorCamera.Blur, 0, 1));
		camera_group.Insert(new SliderPrefab("Near Plane", m_EditorCamera, "NearPlane",  m_EditorCamera.NearPlane, 0, 1));
		camera_group.Insert(new SliderPrefab("DOF Distance", m_EditorCamera, "DOFDistance", m_EditorCamera.DOFDistance, 0, 500));
		camera_group.Insert(new SliderPrefab("DOF Blur", m_EditorCamera, "DOFBlur", m_EditorCamera.DOFBlur, 0, 1));
		camera_group.Insert(new SliderPrefab("Vignette", m_EditorCamera, "Vignette", m_EditorCamera.Vignette, 0, 1));
		camera_group.Insert(new SliderPrefab("Sharpness", m_EditorCamera, "Sharpness", m_EditorCamera.Sharpness, 0, 1));
		camera_group.Insert(new SliderPrefab("Exposure", m_EditorCamera, "Exposure", m_EditorCamera.Exposure, 0, 1));
		
		AddContent(camera_group);
		
		AddButton(DialogResult.OK);
		AddButton(new DialogButton("Default", "ResetDefaultExecute"));
		//AddButton(DialogResult.Cancel);
	}

	
	void ResetDefaultExecute()
	{
		EditorLog.Trace("EditorCameraDialog::ResetDefaultExecute");
		
		m_EditorCamera.FOV = DEFAULT_FOV;
		m_EditorCamera.NearPlane = DEFAULT_NEARPLANE;
		
		m_EditorCamera.PropertyChanged("FOV");
		m_EditorCamera.PropertyChanged("NearPlane");
		
		PPEffects.ResetAll();
	}	
}