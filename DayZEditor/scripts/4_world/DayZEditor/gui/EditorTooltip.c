
class EditorTooltipController: Controller
{
	string ContentTitle;
	string ContentText;
	
	Object ContentItemData;
	
	void ~EditorTooltipController()
	{
		EditorLog.Trace("~EditorTooltipController");
		if (ContentItemData)
			GetGame().ObjectDelete(ContentItemData);
	}
}

class EditorTooltip: MVCLayout
{
	protected TextWidget HeaderLabel;
	
	void EditorToolTip(Widget parent = null)
	{
		GetEditorHudController().CurrentTooltip = this;
	}
	
	void SetTitle(string title)
	{
		GetEditorTooltipController().ContentTitle = title;
		GetEditorTooltipController().NotifyPropertyChanged("ContentTitle");
		
		float w, h, lw, lh;
		HeaderLabel.GetScreenSize(w, h);
		m_LayoutRoot.GetScreenSize(lw, lh);
		m_LayoutRoot.SetSize(w + 15, lh);
	}
	
	void SetContent(string text)
	{
		GetEditorTooltipController().ContentText = text;
		GetEditorTooltipController().NotifyPropertyChanged("ContentText");
	}
	
	void SetContent(Object item)
	{
		GetEditorTooltipController().ContentItemData = item;
		GetEditorTooltipController().NotifyPropertyChanged("ContentItemData");
	}
		
	override typename GetControllerType() {
		return EditorTooltipController;
	}

	override string GetLayoutFile() {
		return "DayZEditor/gui/layouts/tooltips/EditorTooltip.layout";
	}
	
	protected EditorTooltipController GetEditorTooltipController() {
		return EditorTooltipController.Cast(GetController());
	}
}