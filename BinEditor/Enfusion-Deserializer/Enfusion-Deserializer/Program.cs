using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace Enfusion_Deserializer
{
    public class Program
    {
        static void Main(string[] args)
        {
            EnfusionSerializer stream = new("P:\\profiles\\Client\\Users\\tyler\\Editor\\test.dze", FileMode.Open, FileAccess.Read);
            EditorSaveData data = new();
            Console.WriteLine(data.Read(stream));

            Console.WriteLine(data.MapName);

            Console.ReadKey();
        }
    }

    public class vector
    {
        public float X, Y, Z;

        public vector(float x = 0, float y = 0, float z = 0)
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

        public static readonly vector Zero = new();

        public override string ToString()
        {
            return $"{X}, {Y}, {Z}";
        }
    }

    public class EditorSaveData
    {
        public int Version;
        public string MapName;
        public vector CameraPosition;

        public List<EditorObjectData> EditorObjects = new();
        public List<EditorDeletedObjectData> EditorDeletedObjects = new();

        public bool Read(EnfusionSerializer stream)
        {
            string bin_check = stream.ReadString();
            Console.WriteLine(bin_check);
            if (bin_check != "EditorBinned") {
                return false;
            }

            Version = stream.ReadInt();
            MapName = stream.ReadString();
            CameraPosition = stream.ReadVector();

            int objects_length = stream.ReadInt();
            Console.WriteLine("--Placements--");
            for (int i = 0; i < objects_length; i++) {
                EditorObjectData editor_object = new();
                editor_object.Read(stream, Version);
                Console.WriteLine(editor_object.ToString());
                EditorObjects.Add(editor_object);
            }

            int deletions_length = stream.ReadInt();
            Console.WriteLine("--Deletions--");
            for (int i = 0; i < deletions_length; i++) {
                EditorDeletedObjectData deletion_data = new();
                deletion_data.Read(stream, Version);
                Console.WriteLine(deletion_data.ToString());
                EditorDeletedObjects.Add(deletion_data);
            }

            return true;
        }
    }

    public class EditorObjectData
    {
        public string Type;
        public string DisplayName;
        public vector Position;
        public vector Orientation;
        public float Scale = 1;
        public int Flags;

        public List<string> Attachments = new();

        public bool Read(EnfusionSerializer stream, int version)
        {
            Type = stream.ReadString();
            DisplayName = stream.ReadString();
            Position = stream.ReadVector();
            Orientation = stream.ReadVector();
            Scale = stream.ReadFloat();
            Flags = stream.ReadInt();

            int attachment_count = stream.ReadInt();
            for (int i = 0; i < attachment_count; i++) {
                string tt = stream.ReadString();
                Console.WriteLine(tt);
                Attachments.Add(tt);
            }

            int parameters_count = stream.ReadInt();
            for (int i = 0; i < parameters_count; i++) {
                string param_key = stream.ReadString();
                string param_type = stream.ReadString();
                Console.WriteLine(param_key);
                Console.WriteLine(param_type);

            }

            return true;
        }

        public override string ToString()
        {
            return $"{Type}: <{DisplayName}> at {Position}, {Orientation}";
        }
    }

    public class EditorDeletedObjectData
    {
        public string Type;
        public vector Position;
        public int Flags;

        public bool Read(EnfusionSerializer stream, int version)
        {
            Type = stream.ReadString();
            Position = stream.ReadVector();
            Flags = stream.ReadInt();

            return true;
        }

        public override string ToString()
        {
            return $"{Type}: {Position}";
        }
    }

    public class EnfusionSerializer : FileStream
    {
        public EnfusionSerializer(string path, FileMode mode, FileAccess access) : base(path, mode, access)
        { }

        public int ReadInt()
        {
            byte[] bytes = new byte[4];
            Read(bytes, 0, 4);
            return BitConverter.ToInt32(bytes);
        }

        public float ReadFloat()
        {
            byte[] bytes = new byte[4];
            Read(bytes, 0, 4);
            return BitConverter.ToSingle(bytes);
        }

        public string ReadString()
        {
            byte[] bytes = new byte[4];
            Read(bytes, 0, 4);
            int length = BitConverter.ToInt32(bytes);

            byte[] string_data = new byte[length];
            Read(string_data, 0, length);
            return Encoding.UTF8.GetString(string_data);
        }

        public vector ReadVector()
        {
            // Cuts the top off
            byte[] bytes = new byte[4];
            Read(bytes, 0, 4);
            int length = BitConverter.ToInt32(bytes);
            vector result = new();
            for (int i = 0; i < length; i++) {
                Read(bytes, 0, 4);
                result[i] = BitConverter.ToSingle(bytes);
            }

            return result;
        }
    }

}
