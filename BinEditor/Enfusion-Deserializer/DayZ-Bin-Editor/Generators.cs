using Microsoft.CodeAnalysis;
using System;
using System.Diagnostics;

namespace DayZ_Bin_Editor.Generators
{
    public class GeneratedPropertyAttribute : Attribute { }

    [Generator]
    public class PropertyGenerator : ISourceGenerator
    {
        public void Execute(GeneratorExecutionContext context)
        {
            Debug.WriteLine("Execute");

            INamedTypeSymbol symbol = context.Compilation.GetTypeByMetadataName("Generators.GeneratedPropertyAttribute");
            if (symbol == null) {
                return;
            }

            Debug.WriteLine("Found one");
            Debugger.Break();

            /*var query = from type_symbol in context.Compilation.SourceModule.GlobalNamespace.Get()
                        from property in type_symbol.GetProperties()
                        let info = property.FindAttributeFlattened(symbol)
                        where info != null
                        group property by type_symbol into g
                        select g;*/
        }

        public void Initialize(GeneratorInitializationContext context)
        {
#if DEBUG
            if (!Debugger.IsAttached) {
                Debugger.Launch();
            }
#endif
        }
    }
}
