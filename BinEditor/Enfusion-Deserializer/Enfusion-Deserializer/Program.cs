using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Runtime.Remoting;
using System.Text;

namespace Enfusion_Deserializer
{
    public class Program
    {
        static void Main(string[] args)
        {
            File.Delete("P:\\profiles\\Client\\test.bin");

            EnfusionSerializer stream = new("P:\\profiles\\Client\\test.bin", FileMode.CreateNew, FileAccess.ReadWrite);
            //EnfusionSerializer stream = new("P:\\profiles\\Client\\Users\\tyler\\Editor\\test.dze", FileMode.Open, FileAccess.ReadWrite);
            EditorSaveData data = new();
            stream.WriteInt(25);
            stream.WriteBool(true);
            stream.WriteFloat(2.235f);
            stream.WriteString("Testuing");
            stream.WriteVector(new vector(1, 2, 3));

            stream.Close();

            Console.WriteLine("Finished Writing");
            //data.Write(stream);
            //Console.WriteLine(data.Read(stream));
            //Console.WriteLine(data.MapName);

            Console.ReadKey();
        }
    }

    public class EditorSaveData
    {
        public static readonly string BIN_CHECK = "EditorBinned";

        public int Version;
        public string MapName;
        public vector CameraPosition;

        public List<EditorObjectData> EditorObjects = new();
        public List<EditorDeletedObjectData> EditorDeletedObjects = new();

        public bool Read(EnfusionSerializer stream)
        {
            string bin_check = stream.ReadString();
            Console.WriteLine(bin_check);
            if (bin_check != BIN_CHECK) {
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

        public void Write(EnfusionSerializer stream)
        {
            stream.WriteString(BIN_CHECK);
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

        public Dictionary<string, EditorObjectParam> Parameters = new();

        public bool Read(EnfusionSerializer stream, int version)
        {
            Type = stream.ReadString();
            DisplayName = stream.ReadString();
            Position = stream.ReadVector();
            Orientation = stream.ReadVector();
            Scale = stream.ReadFloat();
            Flags = stream.ReadInt();

            if (version < 2) {
                return true;
            }

            int attachment_count = stream.ReadInt();
            for (int i = 0; i < attachment_count; i++) {
                Attachments.Add(stream.ReadString());
            }

            int parameters_count = stream.ReadInt();
            for (int i = 0; i < parameters_count; i++) {
                string param_key = stream.ReadString();
                string param_type = stream.ReadString();
                string[] param_type_data = param_type.Split('<');

                param_type_data[1] = param_type_data[1].Replace('>', char.MinValue);

                dynamic editor_object_param = Activator.CreateInstance(null, $"Enfusion_Deserializer.{param_type_data[0]}").Unwrap();
                editor_object_param.Types.AddRange(param_type_data[1].Split(','));
                editor_object_param.Read(stream);

                Parameters[param_key] = editor_object_param;
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

    public abstract class EditorObjectParam
    {
        public List<string> Types = new();

        //public abstract string GetSerializableType();

        public abstract bool Write(EnfusionSerializer stream);

        public abstract bool Read(EnfusionSerializer stream);
    }

    public class EditorObjectParam1: EditorObjectParam
    {
        public object param1;

        public override bool Write(EnfusionSerializer stream)
        {
            return true;
        }

        public override bool Read(EnfusionSerializer stream)
        {
            foreach (string type in Types) {
                switch (type) {
                    case "string\0":
                        param1 = stream.ReadString();
                        Console.WriteLine(param1);
                        return true;

                    case "int\0":
                        param1 = stream.ReadInt();
                        return true;

                    case "float\0":
                        param1 = stream.ReadFloat();
                        return true;

                    case "bool\0":
                        param1 = stream.ReadBool();
                        return true;

                    default: throw new NotSupportedException();
                }
            }

            return false;
        }
    }
}
