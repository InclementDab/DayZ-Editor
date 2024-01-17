// was publishing wiht 'dotnet publish -r win-x64 --sc false' but idk how to reliably get it running on other machines

using System.Drawing;
using DdsFileTypePlus;
using PaintDotNet;
using Svg;

namespace DirectDrawConverter
{
    internal class Program
    {
        static void Main(string[] args)
        {
            const string OUTPUT_NAME = "icons";
            const int ROW_COUNT = 10;
            const int IMAGE_MAX_SIZE = 512; // this is the space allocated to each image
            const string ICON_DIR = "P:\\DayZEditor\\GUI\\icons";
            
            DirectoryInfo dest_directory = Directory.CreateDirectory(ICON_DIR);
            DirectoryInfo source_directory = Directory.CreateDirectory(Directory.GetCurrentDirectory() + "\\Source");
            
            string output_file = dest_directory + $"\\{OUTPUT_NAME}.dds";
            string enfusion_output_file = output_file.ToString().Replace("P:\\", "");
            enfusion_output_file = enfusion_output_file.Replace("dds", "edds");

            Dictionary<string, Bitmap> source_bitmaps = new Dictionary<string, Bitmap>();
            foreach (FileInfo file in source_directory.GetFiles()) {
                Console.WriteLine($"Loading file {file}");

                // workbench doesnt like dashes in the name
                string format_name = file.Name;
                format_name = format_name.Replace("-", "_");

                string file_name = Path.GetFileNameWithoutExtension(format_name);
                switch (file.Extension) {
                    case ".png": {
                        source_bitmaps[file_name] = new Bitmap(file.ToString());
                        break;
                    }

                    case ".svg": {
                        SvgDocument doc = SvgDocument.Open(file.ToString());
                        double w_aspect = doc.Bounds.Width / 512.0;
                        
                        // we bound to the larger of the two aspects
                        source_bitmaps[file_name] = doc.Draw(IMAGE_MAX_SIZE * (int)w_aspect, IMAGE_MAX_SIZE);
                        break;
                    }
                }                
            }

            if (source_bitmaps.Count() == 0) {
                Console.WriteLine("No files found!");
                return;
            }

            int width = Math.Min(ROW_COUNT, source_bitmaps.Count) * IMAGE_MAX_SIZE;
            int height = (int)(Math.Ceiling((decimal)source_bitmaps.Count / (decimal)ROW_COUNT) * (decimal)IMAGE_MAX_SIZE);

            Bitmap output_map = new Bitmap(width, height);
            ImageSet image_set = new() {
                Name = OUTPUT_NAME,
                RefSize = new Vec2D(width, height),
                Textures = {
                    new() {
                        mpix = 1,
                        path = enfusion_output_file
                    }
                }
            };

            MetaFile meta_file = new() {
                Name = enfusion_output_file,
                Author = Environment.UserName,
                ChangeDate = (int)(DateTime.UtcNow - new DateTime(1970, 1, 1)).TotalSeconds
            };

            int x_offset = 0, y_offset = 0;
            foreach (KeyValuePair<string, Bitmap> source_bitmap in source_bitmaps) {
                bool is_empty = true;                
                int w_centering_offset = (IMAGE_MAX_SIZE - source_bitmap.Value.Size.Width) / 2;
                int h_centering_offset = (IMAGE_MAX_SIZE - source_bitmap.Value.Size.Height) / 2;
                for (int x = 0; x < source_bitmap.Value.Size.Width; x++) {
                    for (int y = 0; y < source_bitmap.Value.Size.Height; y++) {
                        Color color = source_bitmap.Value.GetPixel(x, y);
                        output_map.SetPixel(x + x_offset + w_centering_offset, y + y_offset + h_centering_offset, Color.FromArgb(color.A, 255, 255, 255));
                        if (color.A != 0 && is_empty) {
                            is_empty = false;
                        }
                    }
                }

                if (is_empty) {
                    Console.WriteLine($"{source_bitmap.Key} was empty!");
                }

                ImageSet.ImageSetDef image_set_def = new() {
                    Name = source_bitmap.Key,
                    Pos = new Vec2D(x_offset, y_offset),
                    Size = new Vec2D(IMAGE_MAX_SIZE, IMAGE_MAX_SIZE),
                    Flags = 0
                };

                image_set.Images.Add(image_set_def);

                x_offset += IMAGE_MAX_SIZE;
                if (x_offset >= ROW_COUNT * IMAGE_MAX_SIZE) {
                    x_offset = 0;
                    y_offset += IMAGE_MAX_SIZE;
                }
            }

            File.Delete(dest_directory + $"\\{OUTPUT_NAME}.imageset");
            File.WriteAllText(dest_directory + $"\\{OUTPUT_NAME}.imageset", image_set.ToStringy());
            //File.WriteAllText(dest_directory + $"\\{OUTPUT_NAME}.edds.meta", meta_file.ToStringy());
            Console.WriteLine($"Outputting to {output_file}");
            
            File.Delete(output_file);
            Stream dds_stream = new FileStream(output_file, FileMode.CreateNew);
            DdsFile.Save(
                dds_stream,
                DdsFileFormat.BC7, 
                DdsErrorMetric.Uniform, 
                BC7CompressionSpeed.Slow,
                true,
                true,
                ResamplingAlgorithm.Bilinear,
                Surface.CopyFromBitmap(output_map),
                ProgressChanged);

            dds_stream.Close();
/*
            Console.WriteLine(dds_stream.Length);

            //Console.WriteLine(LZ4Codec.MaximumOutputSize((int)dds_stream.Length));
            //LZ4HighChainEncoder chain_encoder = new LZ4HighChainEncoder(LZ4Level.L07_HC, 65535);
            File.Delete(output_file);
            using (LZ4EncoderStream encoder_stream = LZ4Stream.Encode(File.Create(output_file))) {
                dds_stream.CopyTo(encoder_stream);
                Console.WriteLine(encoder_stream.Length);
            }

           /
            int encoded_length = LZ4Codec.Encode(dds_stream, 0, dds_stream.Length, output_stream, 0, LZ4Codec.MaximumOutputSize((int)dds_stream.Length), LZ4Level.L12_MAX);
            Console.WriteLine(encoded_length);
            File.Delete(output_file);
            File.WriteAllBytes(output_file, target);*/
        }

        private static void ProgressChanged(object sender, ProgressEventArgs e)
        {
            Console.Clear();
            Console.WriteLine($"{e.Percent}%");
        }    
/*
        static void EDDSToDDS(string source_file, string output_file)
        {
            List<int> copy_blocks = new List<int>();
            List<int> LZ4_blocks = new List<int>();
            List<byte> Decoded_blocks = new List<byte>();

            var find_blocks = (BinaryReader reader) => {
                while (true) {
                    byte[] blocks = reader.ReadBytes(4);
                    char[] dd = Encoding.UTF8.GetChars(blocks);

                    string block = new string(dd);
                    int size = reader.ReadInt32();
                    Console.WriteLine(block);
                    switch (block) {
                        case "COPY": {
                            Console.WriteLine(size);
                            copy_blocks.Add(size);
                            break;
                        }
                        case "LZ4 ": {
                            Console.WriteLine(size);
                            LZ4_blocks.Add(size); 
                            break;
                        }

                        default: reader.BaseStream.Seek(-8, SeekOrigin.Current); return;
                    }
                }
            };

            using (var reader = new BinaryReader(File.Open(source_file, FileMode.Open))) {
                byte[] dds_header = reader.ReadBytes(128);
                byte[] dds_header_dx10 = {};

                if (dds_header[84]=='D' && dds_header[85] == 'X' && dds_header[86] == '1' && dds_header[87] == '0') {
                    dds_header_dx10 = reader.ReadBytes(20);
                }

                find_blocks(reader);

                foreach (int count in copy_blocks) {
                    byte[] buff = reader.ReadBytes(count);
                    Decoded_blocks.InsertRange(0, buff);
                }

                foreach (int Length in LZ4_blocks) {
                    uint size = reader.ReadUInt32();
                    byte[] target = new byte[size];

                    int num = 0;
                    LZ4ChainDecoder lz4ChainDecoder = new LZ4ChainDecoder(65536, 0);
                    int count1;
                    int idx = 0;
                    for (; num < Length - 4; num += (count1 + 4)) {
                        count1 = reader.ReadInt32() & int.MaxValue;
                        byte[] numArray = reader.ReadBytes(count1);
                        byte[] buffer = new byte[65536];
                        int count2 = 0;
                        LZ4EncoderExtensions.DecodeAndDrain((ILZ4Decoder)lz4ChainDecoder, numArray, 0, count1, buffer, 0, 65536, out count2);

                        Array.Copy(buffer, 0, target, idx, count2);

                        idx += count2;
                    }

                    Decoded_blocks.InsertRange(0, target);
                }

                if (dds_header_dx10!= null) {
                    Decoded_blocks.InsertRange(0, dds_header_dx10);
                }

                Decoded_blocks.InsertRange(0, dds_header);
                byte[] final = Decoded_blocks.ToArray();

                using (var wr = File.Create(output_file)) {
                    wr.Write(final, 0, final.Length);
                }
            }
        }*/
    }
}