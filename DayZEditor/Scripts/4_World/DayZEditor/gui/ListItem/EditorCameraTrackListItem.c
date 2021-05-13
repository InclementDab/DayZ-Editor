class EditorCameraTrackListItemController: Controller
{
	string Name;
	StringEvaluater pX, pY, pZ;
	StringEvaluater oX, oY, oZ;
	float Time;
	bool FlipOrientation;
	
	vector GetPosition()
	{
		return Vector(pX.Parse(), pY.Parse(), pZ.Parse());
	}
	
	vector GetOrientation()
	{
		return Vector(oX.Parse(), oY.Parse(), oZ.Parse());
	}
}

class EditorCameraTrackListItem: ScriptViewTemplate<EditorCameraTrackListItemController>
{
	void EditorCameraTrackListItem(vector position, vector orientation, float time, string name)
	{
		GetTemplateController().Name = name;
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
	
	void OnFlyToExecute(ButtonCommandArgs args)
	{
		EditorCameraTrackListItemController template_controller = GetTemplateController();
		GetEditor().GetCamera().SetPosition(template_controller.GetPosition());
		GetEditor().GetCamera().SetOrientation(template_controller.GetOrientation());
	}
	
	void SetData(vector position, vector orientation, float time)
	{
		EditorCameraTrackListItemController template_controller = GetTemplateController();
		template_controller.pX = position[0].ToString();
		template_controller.pY = position[1].ToString();
		template_controller.pZ = position[2].ToString();
		template_controller.oX = orientation[0].ToString();
		template_controller.oY = orientation[1].ToString();
		template_controller.oZ = orientation[2].ToString();
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