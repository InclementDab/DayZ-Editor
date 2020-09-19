
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

class EditorCameraDialogController: EditorDialogController
{
	
	protected EditorCamera m_EditorCamera;
	
	float fov;
	float blur;
	float near_plane;
	
	float dof_distance;	
	float dof_blur;
	
	float vignette;
	float sharpness;
	
	void SetCamera(EditorCamera editor_camera)
	{
		EditorLog.Trace("EditorCameraDialogController");
		m_EditorCamera = editor_camera;
		
		fov = m_EditorCamera.GetCurrentFOV();
		near_plane = m_EditorCamera.GetNearPlane();
		
		blur = EditorCameraDialogControllerData.Blur;
		dof_distance = EditorCameraDialogControllerData.DOFDistance;
		dof_blur = EditorCameraDialogControllerData.DOFBlur;
		vignette = EditorCameraDialogControllerData.Vignette;
		sharpness = EditorCameraDialogControllerData.Sharpness;
	}
		
	
	override void PropertyChanged(string property_name)
	{
		switch (property_name) {
			
			case "fov": {
				m_EditorCamera.SetFOV(fov);
				break;
			}			
			
			case "near_plane": {
				m_EditorCamera.SetNearPlane(near_plane);
				break;
			}			
			
			case "dof_blur":
			case "dof_distance": {
				EditorCameraDialogControllerData.DOFDistance = dof_distance;
				EditorCameraDialogControllerData.DOFBlur = dof_blur;
				m_EditorCamera.SetFocus(dof_distance, dof_blur);
				break;
			}
			
			case "vignette": {
				EditorCameraDialogControllerData.Vignette = vignette;
				PPEffects.SetVignette(EditorCameraDialogControllerData.Vignette, 0, 0, 0);
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
		}		
	}
	
	
	void ResetDefaultExecute()
	{
		EditorLog.Trace("EditorCameraDialog::ResetDefaultExecute");
		
		fov = DEFAULT_FOV;
		near_plane = DEFAULT_NEARPLANE;
		
		dof_distance = 0;
		dof_blur = 0;
		
		NotifyPropertyChanged("fov");
		NotifyPropertyChanged("near_plane");
		NotifyPropertyChanged("dof_distance");
		NotifyPropertyChanged("dof_blur");
		
		PPEffects.ResetAll();
	}
}



class EditorCameraDialog: EditorDialog
{
	protected EditorCamera m_EditorCamera;
	protected ref EditorCameraDialogController m_EditorCameraDialogController;
	
	void EditorCameraDialog(EditorHudController controller = null)
	{
		EditorLog.Trace("EditorCameraDialog");
		
		SetTitle("Camera Controller");
		AddButton("Default", "ResetDefaultExecute");
		AddButton("Close", "DialogCloseRelayCommand");
	}
	
	void SetEditorCamera(EditorCamera editor_camera)
	{
		m_EditorCamera = editor_camera;
	
		if (DEFAULT_FOV == -1) {
			DEFAULT_FOV = m_EditorCamera.GetCurrentFOV();
		}
		
		if (DEFAULT_NEARPLANE == -1) {
			DEFAULT_NEARPLANE = m_EditorCamera.GetNearPlane();
		}
		
		m_EditorCameraDialogController = EditorCameraDialogController.Cast(m_Controller);
		m_EditorCameraDialogController.SetCamera(m_EditorCamera);
		
		EditorPrefabGroup camera_group = new EditorPrefabGroup("Camera");
		camera_group.AddPrefab(new EditorPrefabSlider("FOV", "fov", 0, 2));
		camera_group.AddPrefab(new EditorPrefabSlider("Gaussian Blur", "blur", 0, 1));
		camera_group.AddPrefab(new EditorPrefabSlider("Near Plane", "near_plane", 0, 1));
		camera_group.AddPrefab(new EditorPrefabSlider("DOF Distance", "dof_distance", 0, 1));
		camera_group.AddPrefab(new EditorPrefabSlider("DOF Blur", "dof_blur", 0, 1));
		camera_group.AddPrefab(new EditorPrefabSlider("Vignette", "vignette", 0, 1));
		camera_group.AddPrefab(new EditorPrefabSlider("Sharpness", "sharpness", 0, 1));
		
		camera_group.SetController(m_EditorCameraDialogController);
		
		AddContent(camera_group);
	}
	
	override typename GetControllerType() {
		return EditorCameraDialogController;
	}
	
}

