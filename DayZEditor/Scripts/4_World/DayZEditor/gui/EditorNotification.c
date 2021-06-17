
class EditorNotification: ScriptView
{
	static const float NOTIF_ANIM_TIME = 0.1;
	
	protected Widget NotificationPanel;
	protected TextWidget NotificationText;
	
	void EditorNotification(string text, int color = -4301218)
	{
		EditorLog.Trace("EditorNotification");
		NotificationText.SetText(text);
		NotificationPanel.SetColor(color);
	}
	
	void ~EditorNotification()
	{
		EditorLog.Trace("~EditorNotification");
	}
	
	void Play(float duration, string sound = "Notification_SoundSet")
	{	
		thread _Animate(duration);		
	}
	
	private void _Animate(float duration)
	{		
		LerpMotion(0, 0.06);
		SEffectManager.PlaySoundOnObject("Notification_SoundSet", GetEditor().GetCamera());
		Sleep(duration * 1000);
		LerpMotion(0.06, 0);
		NotificationPanel.Show(false);
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