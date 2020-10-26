static float DEFAULT_FOV = -1;
static float DEFAULT_NEARPLANE = -1;

class EditorCameraDialogControllerData
{
	static float DOFDistance;
	static float DOFBlur;
	static float Blur;
	static float Vignette;
	static float Sharpness;
}

class EditorCameraDialogController: DialogBaseController
{	
	float speed;
	float fov;
	float blur;
	float near_plane;
	
	float dof_distance;	
	float dof_blur;
	
	float vignette;
	float sharpness;
	float exposure;
	
	void EditorCameraDialogController()
	{
		EditorLog.Trace("EditorCameraDialogController");
		
		fov = GetEditor().GetCamera().GetCurrentFOV();
		near_plane = GetEditor().GetCamera().GetNearPlane();
		
		blur = EditorCameraDialogControllerData.Blur;
		dof_distance = EditorCameraDialogControllerData.DOFDistance;
		dof_blur = EditorCameraDialogControllerData.DOFBlur;
		vignette = EditorCameraDialogControllerData.Vignette;
		sharpness = EditorCameraDialogControllerData.Sharpness;
	}
		
	
	override void PropertyChanged(string property_name)
	{
		switch (property_name) {
			
			case "speed": {
				EditorCamera.CAMERA_SPEED = speed;
				break;
			}
			
			case "fov": {
				GetEditor().GetCamera().SetFOV(fov);
				break;
			}			
			
			case "near_plane": {
				GetEditor().GetCamera().SetNearPlane(near_plane);
				break;
			}			
			
			case "dof_blur":
			case "dof_distance": {
				EditorCameraDialogControllerData.DOFDistance = dof_distance;
				EditorCameraDialogControllerData.DOFBlur = dof_blur;
				GetEditor().GetCamera().SetFocus(dof_distance, dof_blur);
				break;
			}
			
			case "vignette": {
				EditorCameraDialogControllerData.Vignette = vignette;
#ifdef DAYZ_1_10
				PPEffects.SetVignette(EditorCameraDialogControllerData.Vignette, 0, 0, 0, 255);
#endif
				break;
			}
			
			case "blur": {
				EditorCameraDialogControllerData.Blur = blur;
				PPEffects.SetBlur(EditorCameraDialogControllerData.Blur);
				break;
			}
			
			case "sharpness": {	
				EditorCameraDialogControllerData.Sharpness = sharpness;
				GetGame().GetWorld().GetMaterial("Graphics/Materials/postprocess/filmgrainNV").SetParam("Sharpness", EditorCameraDialogControllerData.Sharpness);
				break;
			}
			
			case "exposure": {
				GetGame().GetWorld().SetEyeAccom(exposure);
				break;
			}
		}		
	}
}



class EditorCameraDialog: EditorDialogBase
{
	protected EditorCameraDialogController m_EditorCameraDialogController;
	
	void EditorCameraDialog(string title) 
	{
		EditorLog.Trace("EditorCameraDialog");
		
		m_EditorCameraDialogController = EditorCameraDialogController.Cast(m_Controller);
		
			
		if (DEFAULT_FOV == -1) {
			DEFAULT_FOV = GetEditor().GetCamera().GetCurrentFOV();
		}
		
		if (DEFAULT_NEARPLANE == -1) {
			DEFAULT_NEARPLANE = GetEditor().GetCamera().GetNearPlane();
		}
				
		GroupPrefab camera_group = new GroupPrefab("Camera", m_Controller, string.Empty);
		camera_group.Insert(new SliderPrefab("Speed", m_Controller, "speed", 0, 500));
		camera_group.Insert(new SliderPrefab("FOV", m_Controller, "fov",  0, 2));
		camera_group.Insert(new SliderPrefab("Gaussian Blur", m_Controller, "blur", 0, 1));
		camera_group.Insert(new SliderPrefab("Near Plane", m_Controller, "near_plane", 0, 1));
		camera_group.Insert(new SliderPrefab("DOF Distance", m_Controller, "dof_distance", 0, 500));
		camera_group.Insert(new SliderPrefab("DOF Blur", m_Controller, "dof_blur", 0, 1));
		camera_group.Insert(new SliderPrefab("Vignette", m_Controller, "vignette", 0, 1));
		camera_group.Insert(new SliderPrefab("Sharpness", m_Controller, "sharpness", 0, 1));
		camera_group.Insert(new SliderPrefab("Exposure", m_Controller, "exposure", 0, 1));
		
		AddContent(camera_group);
		
		AddButton(DialogResult.OK);
		AddButton(new DialogButton("Default", "ResetDefaultExecute"));
		//AddButton(DialogResult.Cancel);
	}

	
	void ResetDefaultExecute()
	{
		EditorLog.Trace("EditorCameraDialog::ResetDefaultExecute");
		
		m_EditorCameraDialogController.fov = DEFAULT_FOV;
		m_EditorCameraDialogController.near_plane = DEFAULT_NEARPLANE;
		
		m_EditorCameraDialogController.dof_distance = 0;
		m_EditorCameraDialogController.dof_blur = 0;
		
		m_EditorCameraDialogController.NotifyPropertyChanged("fov");
		m_EditorCameraDialogController.NotifyPropertyChanged("near_plane");
		m_EditorCameraDialogController.NotifyPropertyChanged("dof_distance");
		m_EditorCameraDialogController.NotifyPropertyChanged("dof_blur");
		
		PPEffects.ResetAll();
	}
	
	override typename GetControllerType() {
		return EditorCameraDialogController;
	}
	
}