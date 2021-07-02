modded class JMModuleConstructor
{
	override void RegisterModules(out TTypenameArray modules)
	{
		super.RegisterModules(modules);
		modules.Insert(EditorClientModule);
		//modules.Insert(EditorServerModule);
		modules.Insert(EditorObjectManagerModule);
		modules.Insert(EditorCameraTrackManagerModule);
	}
}