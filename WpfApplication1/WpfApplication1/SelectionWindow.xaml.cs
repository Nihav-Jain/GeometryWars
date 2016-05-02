using System;
using System.Collections.Generic;
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
using System.Windows.Shapes;
using System.Xml;
using System.Collections;

namespace WpfApplication1
{
    /// <summary>
    /// Interaction logic for SelectionWindow.xaml
    /// </summary>
    public partial class SelectionWindow : Window
    {
        ComboBox cbox;
        MainWindow mWindow;
        private Hashtable items;
        private Hashtable hasText;
        private string name;
        private int mOffset { get; set; }
        public SelectionWindow(MainWindow window, int offset)
        {
            InitializeComponent();
            mWindow = window;
            mOffset = offset;
            XmlTextReader reader = new XmlTextReader("../../Content/Types.xml");
            items = new Hashtable();
            hasText = new Hashtable();

            StackPanel stack = new StackPanel();

            cbox = new ComboBox();
            cbox.Background = Brushes.LightBlue;
            
            reader.Read();

            while (reader.Read())
            {
                switch (reader.NodeType)
                {
                    case XmlNodeType.Element:
                        var cboxitem = new ComboBoxItem();
                        cboxitem.Content = reader.Name;
                        cbox.Items.Add(cboxitem);
                        name = reader.Name;
                        //hasText[name] = false;
                        List<Tuple<string, string>> list = new List<Tuple<string, string>>();
                        items[name] = list;

                        if (reader.HasAttributes)
                        {
                            List<Tuple<string, string>> templist = items[name] as List<Tuple<string, string>>;
                            while (reader.MoveToNextAttribute())
                            {
                                Tuple<string, string> tuple = new Tuple<string, string>(reader.Name, reader.Value);
                                templist.Add(tuple);
                            }
                        }
                        break;
                    case XmlNodeType.Text:
                        hasText[name] = reader.Value;
                        break;
                    case XmlNodeType.EndElement:
                        break;
                }
            }

            stack.Children.Add(cbox);

            Button AddButton = new Button();
            AddButton.Content = "Add";
            AddButton.Click += AddButton_Click;

            stack.Children.Add(AddButton);

            this.AddChild(stack);
        }

        private void AddButton_Click(object sender, RoutedEventArgs e)
        {
            if(cbox.SelectedItem != null)
            {
                if((cbox.SelectedItem as ComboBoxItem).Content as string == "polygon_renderer")
                {
                    PolygonDrawing win2 = new PolygonDrawing(mWindow, mOffset);
                    win2.Show();
                    this.Close();
                }
                else
                {
                    mWindow.AddSection(mOffset, cbox.Text, items, hasText);
                }
            }         
        }
    }
}
