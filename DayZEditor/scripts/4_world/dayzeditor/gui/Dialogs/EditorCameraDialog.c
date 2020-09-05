
class EditorCameraDialogController: Controller
{
	
	protected EditorCamera m_EditorCamera;
	
	float fov;
	float near_plane;
	
	float dof_distance;	
	float dof_blur;
	
	void SetCamera(EditorCamera editor_camera)
	{
		EditorLog.Trace("EditorCameraDialogController");
		m_EditorCamera = editor_camera;
		
		fov = m_EditorCamera.GetCurrentFOV();
		near_plane = m_EditorCamera.GetNearPlane();
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
				m_EditorCamera.SetFocus(dof_distance, dof_blur);
				break;
			}
		}		
	}
	
	
}



class EditorCameraDialog: EditorDialog
{
	protected ref EditorCameraDialogController m_EditorCameraDialogController;
	
	void EditorCameraDialog(EditorCamera editor_camera)
	{
		EditorLog.Trace("EditorCameraDialog");
		
		m_EditorCameraDialogController = new EditorCameraDialogController();
		m_EditorCameraDialogController.SetCamera(editor_camera);
		
		EditorPrefabGroup camera_group = new EditorPrefabGroup("Camera");
		
		camera_group.AddPrefab(new EditorPrefabSlider("FOV", "fov", 0, 0, 10));
		camera_group.AddPrefab(new EditorPrefabSlider("Near Plane", "near_plane", 0, 0, 10));
		camera_group.AddPrefab(new EditorPrefabSlider("DOF Distance", "dof_distance", 0, 0, 10));
		camera_group.AddPrefab(new EditorPrefabSlider("DOF Blur", "dof_blur", 0, 0, 1));
		
		camera_group.SetController(m_EditorCameraDialogController);
		
		AddContent(camera_group);
		
		SetTitle("Camera Controller");
		
		AddButton("Close", "CloseDialog");
	}
	
}

