class MessageBoxPrefab: ScriptView
{
	protected RichTextWidget CaptionText;
	
	void MessageBoxPrefab(string content)
	{
		CaptionText.SetText(content);
	}
	
	override string GetLayoutFile() {
		return "DayZEditor/gui/Layouts/prefabs/MessageBoxPrefab.layout";
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
