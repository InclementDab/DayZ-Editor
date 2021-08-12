using System;
using System.Collections;
using System.IO;

namespace Enfusion_Deserializer
{
    public class Program
    {
        static void Main(string[] args)
        {
            EditorSaveData data = new();

            FileStream stream = new FileStream("P:\\profiles\\Client\\test.bin", FileMode.Open, FileAccess.Read);
            // Console.WriteLine(stream.Length);


            /*Console.WriteLine(EditorSaveData.ReadInt(stream));
            Console.WriteLine(EditorSaveData.ReadInt(stream));
            Console.WriteLine(EditorSaveData.ReadFloat(stream));
            */

            Console.WriteLine(EditorSaveData.ReadVector(stream));

            Console.ReadKey();
        }
    }

    public class Vector
    {
        public float X, Y, Z;

        public Vector(float x = 0, float y = 0, float z = 0)
        {
            X = x; Y = y; Z = z;
        }

        public float this[int index] {
            get {
                return index switch {
                    0 => X,
                    1 => Y,
                    2 => Z,
                    _ => throw new IndexOutOfRangeException(),
                };
            }

            set {
                switch (index) {
                    case 0: X = value; break;
                    case 1: Y = value; break;
                    case 2: Z = value; break;
                    default: throw new IndexOutOfRangeException();
                }
            }
        }

        public static readonly Vector Zero = new();

        public override string ToString()
        {
            return $"{X}, {Y}, {Z}";
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

        public static Vector ReadVector(FileStream stream)
        {
            // Cuts the top off
            byte[] bytes = new byte[4];
            stream.Read(bytes, 0, 4);
            int length = BitConverter.ToInt32(bytes);

            Vector result = new();
            for (int i = 0; i < length; i++) {
                stream.Read(bytes, 0, 4);
                result[i] = BitConverter.ToSingle(bytes);
            }

            return result;
        }
    }
}
