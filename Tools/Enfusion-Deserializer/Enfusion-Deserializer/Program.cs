using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Runtime.Remoting;
using System.Text;

using DayZ_Bin_Editor.Enfusion;
using DayZ_Bin_Editor.Editor;

namespace Enfusion_Deserializer
{
    public class Program
    {
        static void Main(string[] args)
        {
            foreach (string file_name in args)
            {
                if (file_name == string.Empty)
                {
                    Console.WriteLine("No file specified, drag a file over the exe to open it");
                    Console.ReadKey();
                    return;
                }

                EnfusionSerializer stream = new(file_name, FileMode.Open, FileAccess.ReadWrite);
                EditorSaveData data = new();
                data.Read(stream);
                stream.Close();

                int count = 0;
                foreach (EditorObjectData dta in data.EditorObjects)
                {
                    if (dta.Type.Contains("Static_"))
                    {
                        dta.Type = dta.Type.Replace("Static_", "StaticObj_");
                        count++;
                    }
                }

                File.Delete(file_name);
                stream = new(file_name, FileMode.CreateNew, FileAccess.ReadWrite);
                data.Write(stream);
                stream.Close();

                Console.WriteLine($"Fixed naming of {count} static objects");
            }

            Console.WriteLine($"fixed {args.Length} files");
            Console.ReadLine();
        }
    }
}
