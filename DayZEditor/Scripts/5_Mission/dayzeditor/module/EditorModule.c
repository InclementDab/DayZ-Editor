
modded class JMModuleConstructor
{
	override void RegisterModules(out TTypenameArray modules)
	{
		modules.Insert(EditorClientModule);
		modules.Insert(EditorServerModule);
		modules.Insert(EditorObjectManagerModule);
	}
}
