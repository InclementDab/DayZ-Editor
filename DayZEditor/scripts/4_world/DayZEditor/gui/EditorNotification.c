
class EditorNotification: ScriptView
{
	static const float NOTIF_ANIM_TIME = 0.1;
	
	protected Widget NotificationPanel;
	protected TextWidget NotificationText;
	
	void EditorNotification(Widget parent, string text, int color = -4301218)
	{
		EditorLog.Trace("EditorNotification::CreateNotification");
		NotificationText.SetText(text);
		NotificationPanel.SetColor(color);
	}
	
	void Play(float duration, string sound = "Notification_SoundSet")
	{	
		thread _Animate(duration);		
	}
	
	private void _Animate(float duration)
	{		
		LerpMotion(0, 0.5);
		SEffectManager.PlaySoundOnObject("Notification_SoundSet", GetEditor().GetCamera());
		Sleep(duration * 1000);
		LerpMotion(0.5, 0);
	}
	
	private void LerpMotion(float start, float finish)
	{
		int i = 0;
		while (i < NOTIF_ANIM_TIME * 1000) {
			NotificationPanel.SetPos(0, Math.Lerp(start, finish, (1 / NOTIF_ANIM_TIME) * i / 1000));
			Sleep(10);
			i += 10;
		}
	}
	
	override string GetLayoutFile() {
		return "DayZEditor/gui/layouts/EditorNotification.layout";
	}
}