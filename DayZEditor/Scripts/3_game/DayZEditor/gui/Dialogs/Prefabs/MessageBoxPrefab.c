class MessageBoxPrefab: ScriptView
{
	protected RichTextWidget CaptionText;
	
	void MessageBoxPrefab(Widget parent, string content)
	{
		CaptionText.SetText(content);
	}
	
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/dialogs/prefabs/MessageBoxPrefab.layout";
	}
}

enum MessageBoxButtons
{
	OK,
	OKCancel,
	AbortRetryIgnore,
	YesNoCancel,
	YesNo,
	RetryCancel
};
