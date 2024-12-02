modded class JMModuleConstructor
{
	override void RegisterModules(out TTypenameArray modules)
	{
		super.RegisterModules(modules);
		modules.Insert(EditorCameraTrackManagerModule);
	}
}