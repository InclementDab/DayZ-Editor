modded class MVC
{
	override void RegisterConversionTemplates(out TypeConversionHashMap type_conversions)
	{
		super.RegisterConversionTemplates(type_conversions);
		type_conversions.Insert(EditorWidget, TypeConversionEditorWidget);
		type_conversions.Insert(EditorBrushData, TypeConversionBrush);
		type_conversions.Insert(EditorBrushObject, TypeConversionBrushObject);
		type_conversions.Insert(DropdownListPrefabItemBase, DropdownListPrefabItemConverter);
		type_conversions.Insert(EditorFile, TypeConversionEditorFile);
	}
}