using System;
using System.IO;

namespace Enfusion_Deserializer
{
    class Program
    {
        static void Main(string[] args)
        {
            EditorSaveData data = new();

            FileStream stream = new FileStream("P:\\profiles\\Client\\test.bin", FileMode.Open, FileAccess.Read);
            Console.WriteLine(stream.Length);

            
            Console.WriteLine(EditorSaveData.ReadInt(stream));
            Console.WriteLine(EditorSaveData.ReadInt(stream));
            Console.WriteLine(EditorSaveData.ReadFloat(stream));

            Console.ReadKey();
        }
    }

    public class EditorSaveData
    {
        public int test;

        public static int ReadInt(FileStream stream)
        {
            byte[] bytes = new byte[4];
            stream.Read(bytes, 0, 4);
            return BitConverter.ToInt32(bytes);
        }

        public static float ReadFloat(FileStream stream)
        {
            byte[] bytes = new byte[4];
            stream.Read(bytes, 0, 4);
            return BitConverter.ToSingle(bytes);
        }
    }
}
