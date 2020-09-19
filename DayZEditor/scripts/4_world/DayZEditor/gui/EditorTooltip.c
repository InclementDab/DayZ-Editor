
class EditorTooltipController: Controller
{
	string ContentTitle;
	string ContentText;
	
	Object ContentItemData;
}

class EditorTooltip: MVCLayout
{
	protected ref EditorTooltipController m_EditorTooltipController;
	
	void EditorTooltip(string title)
	{
		m_EditorTooltipController = EditorTooltipController.Cast(GetController());
		m_EditorTooltipController.ContentTitle = title;
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
	
	override typename GetControllerType() {
		return EditorTooltipController;
	}

	override string GetLayoutFile() {
		return "‪DayZEditor/gui/layouts/EditorTooltip.layout";
	}
	
}