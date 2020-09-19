
class EditorTooltipController: Controller
{
	string ContentTitle = "Testing";
	string ContentText;
	
	Object ContentItemData;
}

class EditorTooltip: MVCLayout
{
	protected ref EditorTooltipController m_EditorTooltipController;
	
	void EditorTooltip(Widget parent = null)
	{
		m_EditorTooltipController = EditorTooltipController.Cast(GetController());
	}
	
	// Use Close()
	void ~EditorTooltip() {}
	
	void SetTitle(string title)
	{
		m_EditorTooltipController.ContentTitle = title;
		m_EditorTooltipController.NotifyPropertyChanged("ContentTitle");
	}
	
	void SetContent(string text)
	{
		m_EditorTooltipController.ContentText = text;
		m_EditorTooltipController.NotifyPropertyChanged("ContentText");
	}
	
	void SetContent(Object item)
	{
		m_EditorTooltipController.ContentItemData = item;
		m_EditorTooltipController.NotifyPropertyChanged("ContentItemData");
	}
	
	override void Show()
	{
		EditorLog.Trace("EditorTooltip::Show");
		super.Show();
		

		if (GetEditorHudController().CurrentTooltip) {
			GetEditorHudController().CurrentTooltip.Close();
		}
		
		GetEditorHudController().CurrentTooltip = this;
	}
	
	override void Close()
	{
		EditorLog.Trace("EditorTooltip::Close");
		super.Close();
		GetEditorHudController().CurrentTooltip = null;
		delete this;
	}
	
	override typename GetControllerType() {
		return EditorTooltipController;
	}

	override string GetLayoutFile() {
		return "DayZEditor/gui/layouts/tooltips/EditorTooltip.layout";
	}
	
}