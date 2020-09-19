
class EditorTooltipController: Controller
{
	string ContentTitle = "Testing";
	string ContentText;
	
	//Object ContentItemData;
	
	void ~EditorTooltipController()
	{
		DumpStack();
		EditorLog.Trace("~EditorTooltipController");
		//GetGame().ObjectDelete(ContentItemData);
	}
}

class EditorTooltip: MVCLayout
{
	void EditorToolTip(Widget parent = null)
	{
		GetEditorHudController().CurrentTooltip = this;
	}
	
	void SetTitle(string title)
	{
		GetEditorTooltipController().ContentTitle = title;
		GetEditorTooltipController().NotifyPropertyChanged("ContentTitle");
	}
	
	void SetContent(string text)
	{
		GetEditorTooltipController().ContentText = text;
		GetEditorTooltipController().NotifyPropertyChanged("ContentText");
	}
	
	void SetContent(Object item)
	{
		//m_EditorTooltipController.ContentItemData = item;
		//m_EditorTooltipController.NotifyPropertyChanged("ContentItemData");
	}
		
	override typename GetControllerType() {
		return EditorTooltipController;
	}

	override string GetLayoutFile() {
		return "DayZEditor/gui/layouts/tooltips/EditorTooltip.layout";
	}
	
	private EditorTooltipController GetEditorTooltipController() {
		return EditorTooltipController.Cast(GetController());
	}
}