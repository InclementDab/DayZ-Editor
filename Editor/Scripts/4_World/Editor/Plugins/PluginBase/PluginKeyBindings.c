modded class PluginKeyBinding
{
	override void OnInit()
	{
		super.OnInit();

#ifdef DIAG_DEVELOPER
		DisableKeybind("ToggleFreeCamera");
		DisableKeybind("TeleportAtCursor");
#endif
	}
	
	void DisableKeybind(string callback)
	{
		foreach (KeyBinding key_binding: m_KeyBindings) {
			if (key_binding.GetCallbackFunction() == callback) {
				EnScriptVar<string>.Set(key_binding, "m_CallbackFunction", string.Empty);
			}
		}
	}
}