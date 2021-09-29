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

            //EnfusionSerializer stream = new("P:\\profiles\\Client\\test.bin", FileMode.CreateNew, FileAccess.ReadWrite);
            EnfusionSerializer stream = new("P:\\profiles\\Client\\Users\\tyler\\Editor\\azs_trader.dze", FileMode.Open, FileAccess.ReadWrite);
            EditorSaveData data = new();
            data.Read(stream);
            stream.Close();

            foreach (EditorObjectData dta in data.EditorObjects) {
                Console.WriteLine(dta.Scale);
            }

            //File.Delete("P:\\profiles\\Client\\Users\\tyler\\Editor\\test1.dze");
            //stream = new("P:\\profiles\\Client\\Users\\tyler\\Editor\\test1.dze", FileMode.CreateNew, FileAccess.ReadWrite);
            //data.Write(stream);
            //stream.Close();

            Console.WriteLine("Finished Writing");
        }
    }
}
