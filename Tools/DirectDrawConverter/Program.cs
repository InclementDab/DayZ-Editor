// was publishing wiht 'dotnet publish -r win-x64 --sc false' but idk how to reliably get it running on other machines

using System.Drawing;
using DdsFileTypePlus;
using DirectDrawConverter;
using PaintDotNet;
using Svg;

const int ROW_COUNT = 10;
const int IMAGE_MAX_SIZE = 512; // this is the space allocated to each image

string folder_output = "P:\\Editor\\GUI\\icons";
DirectoryInfo dest_directory = Directory.CreateDirectory(folder_output);
DirectoryInfo source_directory = Directory.CreateDirectory(Directory.GetCurrentDirectory() + "\\svgs");

foreach (DirectoryInfo directory in source_directory.EnumerateDirectories())
{
    string output_file = dest_directory + $"\\{directory.Name}.dds";
    string enfusion_output_file = output_file.Replace("P:\\", "");
    enfusion_output_file = enfusion_output_file.Replace("dds", "edds");
    Dictionary<string, Bitmap> source_bitmaps = new Dictionary<string, Bitmap>();

    foreach (FileInfo file in directory.GetFiles())
    {
        Console.WriteLine($"Loading file {file}");
        // workbench doesnt like dashes in the name
        string format_name = file.Name;
        format_name = format_name.Replace("-", "_");

        string file_name = Path.GetFileNameWithoutExtension(format_name);
        switch (file.Extension)
        {
            case ".png": {
                source_bitmaps[file_name] = new Bitmap(file.ToString());
                break;
            }

            case ".svg": {
                SvgDocument doc = SvgDocument.Open(file.ToString());

                double w_aspect = 1.0;
                double h_aspect = 1.0;
                if (doc.Bounds.Width > IMAGE_MAX_SIZE || doc.Bounds.Height > IMAGE_MAX_SIZE) {
                    w_aspect = doc.Bounds.Width / IMAGE_MAX_SIZE;
                }
                // we bound to the larger of the two aspects
                if (w_aspect > 1.0) {
                    h_aspect = 1.0 / w_aspect;
                    w_aspect = 1.0;
                }

                source_bitmaps[file_name] = doc.Draw((int)(IMAGE_MAX_SIZE * w_aspect), (int)(IMAGE_MAX_SIZE * h_aspect));
                break;
            }
        }
    }

    int width = Math.Min(ROW_COUNT, source_bitmaps.Count) * IMAGE_MAX_SIZE;
    int height = (int)(Math.Ceiling((decimal)source_bitmaps.Count / (decimal)ROW_COUNT) * (decimal)IMAGE_MAX_SIZE);
    Bitmap output_map = new(width, height);
    ImageSet image_set = new() {
        Name = directory.Name,
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
        int w_centering_offset = (IMAGE_MAX_SIZE - source_bitmap.Value.Size.Width) / 2;
        int h_centering_offset = (IMAGE_MAX_SIZE - source_bitmap.Value.Size.Height) / 2;
        for (int x = 0; x < source_bitmap.Value.Size.Width; x++) {
            for (int y = 0; y < source_bitmap.Value.Size.Height; y++) {
                Color color = source_bitmap.Value.GetPixel(x, y);
                output_map.SetPixel(x + x_offset + w_centering_offset, y + y_offset + h_centering_offset, Color.FromArgb(color.A, 255, 255, 255));
            }
        }

        ImageSet.ImageSetDef image_set_def = new() {
            Name = source_bitmap.Key,
            Pos = new Vec2D(x_offset + w_centering_offset, y_offset + h_centering_offset),
            Size = new Vec2D(source_bitmap.Value.Size.Width, source_bitmap.Value.Size.Height),
            Flags = 0
        };

        image_set.Images.Add(image_set_def);

        x_offset += IMAGE_MAX_SIZE;
        if (x_offset >= ROW_COUNT * IMAGE_MAX_SIZE) {
            x_offset = 0;
            y_offset += IMAGE_MAX_SIZE;
        }
    }

    File.Delete(dest_directory + $"\\{directory.Name}.imageset");
    File.WriteAllText(dest_directory + $"\\{directory.Name}.imageset", image_set.ToStringy());
    //File.WriteAllText(dest_directory + $"\\{OUTPUT_NAME}.edds.meta", meta_file.ToStringy());
    Console.WriteLine($"Outputting to {output_file}");

    File.Delete(output_file);
    Stream dds_stream = new FileStream(output_file, FileMode.CreateNew);
    DdsFile.Save(
        dds_stream,
        DdsFileFormat.BC7,
        DdsErrorMetric.Uniform,
        BC7CompressionSpeed.Medium,
        true,
        true,
        ResamplingAlgorithm.Bilinear,
        Surface.CopyFromBitmap(output_map),
        ProgressChanged);

    dds_stream.Close();
}

void ProgressChanged(object sender, ProgressEventArgs e)
{
    Console.Clear();
    Console.WriteLine($"{e.Percent}%");
}    
