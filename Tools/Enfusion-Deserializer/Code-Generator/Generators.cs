using Microsoft.CodeAnalysis;
using System;
using System.Collections.Generic;
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

            INamedTypeSymbol symbol = context.Compilation.GetTypeByMetadataName("DayZ_Bin_Editor.Generators.GeneratedPropertyAttribute");
            if (symbol == null) {
                return;
            }

            IEnumerable<INamespaceOrTypeSymbol> query = context.Compilation.SourceModule.GlobalNamespace.GetMembers();
            foreach (INamespaceOrTypeSymbol qsymbol in query) {
                foreach (INamedTypeSymbol type_member in qsymbol.GetTypeMembers()) {
                    foreach (ISymbol isymbol in type_member.GetMembers()) {
                        Debug.WriteLine(isymbol);
                        var test = type_member.FindImplementationForInterfaceMember(isymbol);
                        Debug.WriteLine(test);
                    }
                }
            }

            
            //Debugger.Break();

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
                //Debugger.Launch();
            }
#endif
        }

        public class TestCase
        {
            [GeneratedProperty]
            public int Test { get; set; }

            [GeneratedProperty]
            public bool Test2;
        }

        public class Program
        { 
            static void Main(string[] args)
            {
                Console.ReadKey();
            }
        }
    }
}
