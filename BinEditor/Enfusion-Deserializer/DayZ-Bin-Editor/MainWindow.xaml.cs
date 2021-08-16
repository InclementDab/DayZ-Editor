using DayZ_Bin_Editor.Editor;
using DayZ_Bin_Editor.Enfusion;
using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace DayZ_Bin_Editor
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        protected MainWindowViewModel m_MainWindowViewModel = new();

        public MainWindow()
        {
            InitializeComponent();
            DataContext = m_MainWindowViewModel;
        }

        private void OpenCommand_Executed(object sender, ExecutedRoutedEventArgs e)
        {
            OpenFileDialog file_dialog = new() {
                Filter = "DayZ Editor Files (*.dze)|*.dze"
            };

            if (file_dialog.ShowDialog() ?? false) {
                return;
            }

            EnfusionSerializer stream = new(file_dialog.FileName, FileMode.Open, FileAccess.Read);

            m_MainWindowViewModel.SaveData.Read(stream);
            stream.Close();
        }

        private void SaveCommand_Executed(object sender, ExecutedRoutedEventArgs e)
        {
            SaveFileDialog file_dialog = new() {
                Filter = "DayZ Editor Files (*.dze)|*.dze"
            };

            if (file_dialog.ShowDialog() ?? false) {
                return;
            }

            EnfusionSerializer stream = new(file_dialog.FileName, FileMode.CreateNew, FileAccess.Write);

            m_MainWindowViewModel.SaveData.Write(stream);
            stream.Close();
        }

        private void ExitCommand_Executed(object sender, ExecutedRoutedEventArgs e)
        {
            Application.Current.Shutdown();
        }
    }

    public class MainWindowViewModel
    {
        public EditorSaveData SaveData { get; set; } = new EditorSaveData();
    }
}
