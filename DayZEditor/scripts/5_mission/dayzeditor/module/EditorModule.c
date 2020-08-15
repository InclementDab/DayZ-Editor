
modded class JMModuleConstructor
{
	override void RegisterModules(out TTypenameArray modules)
	{
		modules.Insert(EditorSettings);
		modules.Insert(EditorClientModule);
		modules.Insert(EditorServerModule);
	}
	
}

