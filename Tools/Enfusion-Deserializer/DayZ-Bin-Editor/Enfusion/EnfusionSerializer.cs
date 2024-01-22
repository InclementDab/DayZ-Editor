﻿using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DayZ_Bin_Editor.Enfusion
{
    public class EnfusionSerializer : FileStream
    {
        public EnfusionSerializer(string path, FileMode mode, FileAccess access) : base(path, mode, access)
        { }

        protected void Write(byte[] bytes)
        {
            Write(bytes, 0, bytes.Length);
        }

        public bool ReadBool()
        {
            byte[] bytes = new byte[4];
            Read(bytes, 0, 4);
            return BitConverter.ToBoolean(bytes, 0);
        }

        public void WriteBool(bool value)
        {
            // Booleans in enfusion are literally just integers widepeepohappy
            WriteInt(value ? 1 : 0);
        }

        public int ReadInt()
        {
            byte[] bytes = new byte[4];
            Read(bytes, 0, 4);
            return BitConverter.ToInt32(bytes, 0);
        }

        public void WriteInt(int value)
        {
            Write(BitConverter.GetBytes(value));
        }

        public float ReadFloat()
        {
            byte[] bytes = new byte[4];
            Read(bytes, 0, 4);
            return BitConverter.ToSingle(bytes, 0);
        }

        public void WriteFloat(float value)
        {
            Write(BitConverter.GetBytes(value));
        }

        public string ReadString()
        {
            byte[] bytes = new byte[4];
            Read(bytes, 0, 4);
            int length = BitConverter.ToInt32(bytes, 0);

            byte[] string_data = new byte[length];
            Read(string_data, 0, length);
            return Encoding.UTF8.GetString(string_data);
        }

        public void WriteString(string value)
        {
            Write(BitConverter.GetBytes(value.Length));
            Write(Encoding.UTF8.GetBytes(value));
        }

        public vector ReadVector()
        {
            // Cuts the top off
            byte[] bytes = new byte[4];
            Read(bytes, 0, 4);
            int length = BitConverter.ToInt32(bytes, 0);
            vector result = new vector();
            for (int i = 0; i < length; i++) {
                Read(bytes, 0, 4);
                result[i] = BitConverter.ToSingle(bytes, 0);
            }

            return result;
        }

        public void WriteVector(vector value)
        {
            // length is always 3
            Write(BitConverter.GetBytes(3));
            for (int i = 0; i < 3; i++) {
                Write(BitConverter.GetBytes(value[i]));
            }
        }
    }
}
