using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.Remoting;
using System.Text;

namespace Enfusion_Deserializer
{
    public class Program
    {
        static void Main(string[] args)
        {
            //EnfusionSerializer stream = new("P:\\profiles\\Client\\test.bin", FileMode.CreateNew, FileAccess.ReadWrite);
            EnfusionSerializer stream = new("P:\\profiles\\Client\\Users\\tyler\\Editor\\test.dze", FileMode.Open, FileAccess.ReadWrite);
            EditorSaveData data = new();
            data.Read(stream);
            stream.Close();


            File.Delete("P:\\profiles\\Client\\Users\\tyler\\Editor\\test.dze");
            stream = new("P:\\profiles\\Client\\Users\\tyler\\Editor\\test.dze", FileMode.CreateNew, FileAccess.ReadWrite);
            data.Write(stream);
            stream.Close();

            Console.WriteLine("Finished Writing");

            Console.ReadKey();
        }
    }

    public enum EditorObjectFlags
    {
        NONE = 0,
        BBOX = 2,
        MAPMARKER = 4,
        OBJECTMARKER = 8,
        LISTITEM = 16,
        ALL = 2147483647
    }

    public class EditorSaveData
    {
        public static readonly string BIN_CHECK = "EditorBinned";

        public int Version = 2;
        public string MapName;
        public vector CameraPosition;

        public List<EditorObjectData> EditorObjects = new();
        public List<EditorDeletedObjectData> EditorDeletedObjects = new();

        public bool Read(EnfusionSerializer stream)
        {
            string bin_check = stream.ReadString();
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
            stream.WriteInt(Version);
            stream.WriteString(MapName);
            stream.WriteVector(CameraPosition);

            stream.WriteInt(EditorObjects.Count);
            foreach (EditorObjectData data in EditorObjects) {
                data.Write(stream, Version);
            }

            stream.WriteInt(EditorDeletedObjects.Count);
            foreach (EditorDeletedObjectData data in EditorDeletedObjects) {
                data.Write(stream, Version);
            }
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

        public void Write(EnfusionSerializer stream, int version)
        {
            stream.WriteString(Type);
            stream.WriteString(DisplayName);
            stream.WriteVector(Position);
            stream.WriteVector(Orientation);
            stream.WriteFloat(Scale);
            stream.WriteInt(Flags);

            if (version < 2) {
                return;
            }

            stream.WriteInt(Attachments.Count);
            foreach (string attachment in Attachments) {
                stream.WriteString(attachment);
            }

            stream.WriteInt(Parameters.Count);
            foreach (KeyValuePair<string, EditorObjectParam> param in Parameters) {
                stream.WriteString(param.Key);
                stream.WriteString(param.Value.GetSerializableType());

                param.Value.Write(stream);
            }
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

        public void Write(EnfusionSerializer stream, int version)
        {
            stream.WriteString(Type);
            stream.WriteVector(Position);
            stream.WriteInt(Flags);
        }

        public override string ToString()
        {
            return $"{Type}: {Position}";
        }
    }
}
