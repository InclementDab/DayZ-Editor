class EditorCameraTrackListItemController: ViewController
{		
	StringEvaluater pX, pY, pZ;
	StringEvaluater oX, oY, oZ;
	bool CollapseTab = false;
	Widget CollapsePanel;
	ButtonWidget CollapseButton;
	ImageWidget CollapseButton_Icon;
			
	vector GetPosition()
	{
		return Vector(pX.Parse(), pY.Parse(), pZ.Parse());
	}
	
	vector GetOrientation()
	{
		return Vector(oX.Parse(), oY.Parse(), oZ.Parse());
	}
		
	override void PropertyChanged(string property_name)
	{
		switch (property_name) {
			case "CollapseTab": {
				CollapsePanel.Show(CollapseTab);
				int size = 1 + (GetWidgetUnderCursor() == CollapseButton) * 2;
				if (CollapseTab) {
					Symbols.MINUS.Load(CollapseButton_Icon, size);
				} else {
					Symbols.PLUS.Load(CollapseButton_Icon, size);
				}
				
				break;
			}
		}
	}
}

class EditorCameraTrackListItem: ScriptView
{
	protected Editor m_Editor = GetEditor();
	protected EditorCameraTrack m_CameraTrack;
	protected EditorCameraTrackListItemController m_TemplateController;

	//PanelColor
	Widget CameraTrackGroupWrapper;
	//OutlineColor
	Widget MainContainerWrapper;
	//CameraIconColor
	ButtonWidget DeleteButton, CollapseButton, ReorderUpButton, ReorderDownButton;
	ImageWidget CollapseButton_Icon;
	
	TextWidget CameraTrackNodeTitle;
	
	void EditorCameraTrackListItem(notnull EditorCameraTrack camera_track)
	{
		m_CameraTrack = camera_track;
		m_TemplateController = EditorCameraTrackListItemController.Cast(m_Controller);

		CameraTrackNodeTitle.SetText(string.Format("Node \#%1", m_CameraTrack.GetData().GetID()));

		m_CameraTrack.OnObjectSelected.Insert(OnSelected);
		m_CameraTrack.OnObjectDeselected.Insert(OnDeselected);
		m_CameraTrack.OnUpdated.Insert(OnObjectUpdate);

		OnObjectUpdate();
	}

    void ~EditorCameraTrackListItem()
    {
        if (m_CameraTrack)
        {
            m_CameraTrack.OnObjectSelected.Remove(OnSelected);
            m_CameraTrack.OnObjectDeselected.Remove(OnDeselected);
            m_CameraTrack.OnUpdated.Remove(OnObjectUpdate);
        }
    }

	protected void OnObjectUpdate()
	{
		vector position = m_CameraTrack.GetPosition();
		vector orientation = m_CameraTrack.GetOrientation();
		m_TemplateController.pX = position[0].ToString();
		m_TemplateController.pY = position[1].ToString();
		m_TemplateController.pZ = position[2].ToString();
		m_TemplateController.oX = orientation[0].ToString();
		m_TemplateController.oY = orientation[1].ToString();
		m_TemplateController.oZ = orientation[2].ToString();
		//m_TemplateController.Time = EditorCameraTrackData.Cast(m_CameraTrack.GetData()).Time;
		m_TemplateController.NotifyPropertyChanged();
	}
	
	protected void OnSelected(EditorObject editor_object) 
	{
		//CameraTrackGroupWrapper.SetColor(m_Editor.GetSettings().SelectionColor);
		//MainContainerWrapper.SetColor(m_Editor.GetSettings().HighlightColor);
		//CameraTrackOptionButton.SetColor(m_Editor.GetSettings().MarkerPrimaryColor);
		
		CameraTrackGroupWrapper.SetColor(GetEditor().GetSettings().SelectionColor);
	}
		
	protected void OnDeselected(EditorObject editor_object) 
	{	
		
		//MainContainerWrapper.SetAlpha(0);
		//CameraTrackOptionButton.SetColor(-1);
		
		WidgetAnimator.AnimateColor(CameraTrackGroupWrapper, 0xff191919, 60);
		//WidgetAnimator.AnimateColor(MainContainerWrapper, 0xff191919, 60);
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		int old_index = GetEditor().GetObjectManager().GetCameraTrackIndex(m_CameraTrack);
		switch (w) {
			case ReorderUpButton: {
				if (old_index > 0) {
					GetEditor().GetObjectManager().SetCameraTrackIndex(m_CameraTrack, old_index--);
				}

				return true;
			}

			case ReorderDownButton: {
				if (old_index < GetEditor().GetObjectManager().GetCameraTracks().Count() - 1) {
					GetEditor().GetObjectManager().SetCameraTrackIndex(m_CameraTrack, old_index++);
				}

				return true;
			}
			
			case DeleteButton: {
				GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(GetEditor().DeleteObject, 0, false, m_CameraTrack, true);
				return true;
			}
		}

		return super.OnClick(w, x, y, button);
	}

	override bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
		if (button != 0) {
			return super.OnMouseButtonUp(w, x, y, button);
		}

		if (w.IsInherited(ButtonWidget) || w.IsInherited(EditBoxWidget)) {
			return super.OnMouseButtonUp(w, x, y, button);
		}
		
		if (!m_TemplateController.CollapseTab) {
			//m_TemplateController.CollapseTab = true;
			//m_TemplateController.NotifyPropertyChanged("CollapseTab");
		}
		
		if (!m_CameraTrack.IsSelected()) {
			if (!GetEditor().IsShiftDown()) {
				GetEditor().ClearSelection();
			}
			
			GetEditor().SelectObject(m_CameraTrack);
		} else {
			GetEditor().DeselectObject(m_CameraTrack);
		}
		
		return true;
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		switch (w) {
			case CollapseButton: {
				CollapseButton_Icon.SetImage(SymbolSize.IDX_SOLID);
				break;
			}
		}
		
		if (!m_CameraTrack.IsSelected()) {
			MainContainerWrapper.SetColor(GetEditor().GetSettings().HighlightColor);
		}
		
		return true;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		switch (w) {
			case CollapseButton: {
				CollapseButton_Icon.SetImage(SymbolSize.IDX_LIGHT);
				break;
			}
		}
		
		if (!m_CameraTrack.IsSelected()) {
			WidgetAnimator.AnimateColor(MainContainerWrapper, 0xff191919, 60);
		}
		
		return true;
	}

	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		switch (w) {

			
		}

		return super.OnChange(w, x, y, finished);
	}
	
	void OnSetExecute(ButtonCommandArgs args)
	{
		vector transform[4];
		GetEditor().GetCamera().GetTransform(transform);
		m_CameraTrack.SetTransform(transform);
	}
	
	void OnDeleteExecute(ButtonCommandArgs args)
	{
		GetEditor().DeleteCameraTrack(m_CameraTrack);
	}
	
	void OnFlyToExecute(ButtonCommandArgs args)
	{
		GetEditor().GetCamera().SetPosition(m_TemplateController.GetPosition());
		GetEditor().GetCamera().SetOrientation(m_TemplateController.GetOrientation());
	}
	
	EditorCameraTrackListItemController GetTemplateController()
	{
		return m_TemplateController;
	}

	override typename GetControllerType()
	{
		return EditorCameraTrackListItemController;
	}
	
	override string GetLayoutFile()
	{
		return "DayZEditor/GUI/layouts/cameratrack/CameraTrackGroup.layout";
	}
}