class EditorCameraTrackListItemController: Controller
{
	float pX, pY, pZ;
	float oX, oY, oZ;
	float Time;
}

class EditorCameraTrackListItem: ScriptViewTemplate<EditorCameraTrackListItemController>
{
	void EditorCameraTrackListItem(vector position, vector orientation, float time)
	{
		SetData(position, orientation, time);
	}
	
	void OnSetExecute(ButtonCommandArgs args)
	{
		SetData(GetEditor().GetCamera().GetPosition(), GetEditor().GetCamera().GetOrientation(), GetTemplateController().Time);
	}
	
	void OnDeleteExecute(ButtonCommandArgs args)
	{
		delete this;
	}
	
	void SetData(vector position, vector orientation, float time)
	{
		EditorCameraTrackListItemController template_controller = GetTemplateController();
		template_controller.pX = position[0];
		template_controller.pY = position[1];
		template_controller.pZ = position[2];
		template_controller.oX = orientation[0];
		template_controller.oY = orientation[1];
		template_controller.oZ = orientation[2];
		template_controller.Time = time;
		template_controller.NotifyPropertyChanged();
	}
	
	EditorCameraTrackListItemController GetData()
	{
		return GetTemplateController();
	}
	
	override string GetLayoutFile()
	{
		return "DayZEditor/GUI/layouts/cameratrack/CameraTrackGroup.layout";
	}
}