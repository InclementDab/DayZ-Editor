

// Loads .layout file Widgets into Properties of context (when they are the same name)
/*

Example:

.layout file:
MenuBarRoot		FrameWidget
	MenuBarFile   	ButtonWidget
		MenuBarFileLabel	TextWidget


.c file:
class TestClass
{
	ButtonWidget MenuBarFile; //<-- these properties will be assigned
	private TextWidget MenuBarFileLabel;
}

*/
static void LoadViewProperties(Class context, PropertyTypeHashMap property_map, Widget root_widget)
{
	foreach (string propertyName, typename property_type : property_map)
	{

		if (!property_type.IsInherited(Widget))
			continue;

		Widget target = root_widget.FindAnyWidget(propertyName);

		// fixes bug that breaks everything
		if (target && root_widget.GetName() != propertyName)
		{
			EnScript.SetClassVar(context, propertyName, 0, target);
			continue;
		}

		// Allows you to define the layout root aswell within it
		if (!target && root_widget.GetName() == propertyName)
		{
			EnScript.SetClassVar(context, propertyName, 0, root_widget);
			continue;
		}
	}
}

// 0: Context of Start Scope, out is context of final scope
// 1: Name of variable string Ex: m_Binding.Value.Root
// return: Final variable name
static PropertyInfo GetSubScope(out Class context, string name)
{
	if (name == string.Empty)
		return null;

	TStringArray variable_scope = {};
	name.Split(".", variable_scope);

	for (int i = 0; i < variable_scope.Count() - 1; i++)
	{
		EnScript.GetClassVar(context, variable_scope[i], 0, context);
	}

	if (variable_scope.Count() == 1)
	{
		return PropertyInfo.GetFromClass(context, name);
	}

	return PropertyInfo.GetFromClass(context, variable_scope[variable_scope.Count() - 1]);
}

// Gets typename from Templated type
// This is useful when getting this error...
// 'Can't get typename from forward declaration 'Class'. Use Type("name") instead....'
class TemplateType<Class T>
{
	T value;

	static typename GetType()
	{
		return (new TemplateType<T>()).Type().GetVariableType(0);
	}
}