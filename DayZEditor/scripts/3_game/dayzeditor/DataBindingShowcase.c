// Example for defining a custom type
modded class MVC
{
	override void RegisterConversionTemplates(out TypeConverterHashMap type_conversions)
	{
		super.RegisterConversionTemplates(type_conversions);
		type_conversions.Insert(EquationEvaluater, TypeConversionString);
		

	}
}

typedef string EquationEvaluater;
class EquationEvaluater: string
{
	void Evaluate()
	{
		StringEvaluater evaluater();
		value = evaluater.Parse(value).ToString();
	}
}

