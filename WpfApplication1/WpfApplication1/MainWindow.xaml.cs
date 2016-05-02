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
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Text.RegularExpressions;
using System.Xml;
using Microsoft.Win32;
using System.Collections;

namespace WpfApplication1
{
    public class AddEventArg : RoutedEventArgs
    {
        public StackPanel currentPanel { get; set; }
    }

    //todo: open file and populate, yushaing polygon maker
    //notes: cant give something char data and also give it children. 
    //aka. <int>asdf</int> cant have anything between char data and end element. 
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public StackPanel currentPanel;
        public XmlDocument xmlDoc;
        public XmlNode rootNode;
        public XmlNode CurrentNode;
        public MainWindow()
        {
            InitializeComponent();

            currentPanel = this.Content as StackPanel;
            currentPanel = currentPanel.FindName("World") as StackPanel;

            xmlDoc = new XmlDocument();
            rootNode = xmlDoc.CreateElement("world");
            XmlAttribute attribute = xmlDoc.CreateAttribute("Name");
            TextBox Name = currentPanel.FindName("WorldName") as TextBox;
            attribute.Value = Name.Text;
            rootNode.Attributes.Append(attribute);
            xmlDoc.AppendChild(rootNode);

            Scroller.Height = (Window.ActualHeight);
            Scroller.Width = Window.ActualWidth;
        }

        private void Add_Click(object sender, RoutedEventArgs e)
        {
            var button = sender as Button;
            currentPanel = this.Content as StackPanel;
            currentPanel = currentPanel.FindName("World") as StackPanel;;
            CurrentNode = rootNode;
            SelectionWindow win2 = new SelectionWindow(this, int.Parse(button.Tag.ToString()));
            win2.Show();
        }

        public Section AddSection(int offset, string name, Hashtable table, Hashtable hasText)
        {
            Section section = new Section(offset, this, currentPanel, CurrentNode, xmlDoc, table, name, hasText);

            currentPanel.Children.Add(section);

            return section;
        }

        public Section AddInteranlSection(int offset, StackPanel parentPanel, XmlNode parentNode, string name, Hashtable table, Hashtable hasText)
        {
            Section section = new Section(offset, this, parentPanel, parentNode, xmlDoc, table, name, hasText);

            parentPanel.Children.Add(section);

            return section;
        }

        private void Save_Click(object sender, RoutedEventArgs e)
        {
            SaveFileDialog saveFileDialog = new SaveFileDialog();
            if (saveFileDialog.ShowDialog() == true)
                xmlDoc.Save(saveFileDialog.FileName);
        }

        private void WorldName_TextChanged(object sender, TextChangedEventArgs e)
        {
            XmlAttribute attribute = xmlDoc.CreateAttribute("Name");
            TextBox Name = (this.Content as StackPanel).FindName("WorldName") as TextBox;
            attribute.Value = Name.Text;
            rootNode.Attributes.Append(attribute);
        }

        private void ChangeSize(object sender, SizeChangedEventArgs e)
        {
            Scroller.Height = Window.ActualHeight - 60f;
            Scroller.Width = Window.ActualWidth - 15f;
        }

        private void Open_Cliick(object sender, RoutedEventArgs e)
        {
            rootNode.RemoveAll();
            int offset = 0;
            currentPanel = this.Content as StackPanel;
            currentPanel = currentPanel.FindName("World") as StackPanel;
            currentPanel.Children.RemoveRange(1, currentPanel.Children.Count);
            currentPanel.Name = "World";
            //rootNode = xmlDoc.CreateElement("world");
            CurrentNode = rootNode;

            OpenFileDialog open = new OpenFileDialog();

            if (open.ShowDialog() == true)
            {
                XmlTextReader reader = new XmlTextReader(open.FileName);
                reader.WhitespaceHandling = WhitespaceHandling.None;
                reader.Read();
                if (reader.HasAttributes)
                {
                    while (reader.MoveToNextAttribute())
                    {
                        XmlAttribute attribut = xmlDoc.CreateAttribute(reader.Name);
                        attribut.Value = reader.Value;
                        rootNode.Attributes.Append(attribut);
                        TextBox box = currentPanel.FindName("WorldName") as TextBox;
                        box.Text = reader.Value;
                    }
                }

                Hashtable items = new Hashtable();
                Hashtable hasText = new Hashtable();
                string name = reader.Name;
                while (reader.Read())
                {
                    switch (reader.NodeType)
                    {
                        //Section(int offset, MainWindow main, StackPanel parent, XmlNode root, XmlDocument xmlDoc, Hashtable table, string name, bool hasText)
                        case XmlNodeType.Element:
                            name = reader.Name;
                            List<Tuple<string, string>> list = new List<Tuple<string, string>>();
                            items[name] = list;
                            bool isEmpty = reader.IsEmptyElement;
                            if (reader.HasAttributes)
                            {
                                List<Tuple<string, string>> templist = items[name] as List<Tuple<string, string>>;
                                while (reader.MoveToNextAttribute())
                                {
                                    Tuple<string, string> tuple = new Tuple<string, string>(reader.Name, reader.Value);
                                    templist.Add(tuple);
                                }
                            }
                            currentPanel = AddSection(++offset, name, items, hasText);
                            CurrentNode = (currentPanel as Section).GetXmlNode();
                            if (isEmpty)
                            {
                                offset--;
                                CurrentNode = CurrentNode.ParentNode;
                                currentPanel = currentPanel.Parent as StackPanel;
                            }                      
                            break;
                        case XmlNodeType.Text:
                            hasText[name] = reader.Value;
                            StackPanel tempPanel = currentPanel.Parent as StackPanel;
                            tempPanel.Children.Remove(currentPanel);
                            currentPanel = tempPanel;
                            XmlNode tempNode = CurrentNode.ParentNode;
                            tempNode.RemoveChild(CurrentNode);
                            CurrentNode = tempNode;
                            currentPanel = AddSection(offset, name, items, hasText);
                            XmlText tex = xmlDoc.CreateTextNode(reader.Value);
                            CurrentNode = (currentPanel as Section).GetXmlNode();
                            CurrentNode.InnerText = tex.Value;
                            break;
                        case XmlNodeType.Comment:
                            name = "comment";
                            List<Tuple<string, string>> lis = new List<Tuple<string, string>>();
                            items[name] = lis;
                            if (reader.HasAttributes)
                            {
                                List<Tuple<string, string>> templist = items[name] as List<Tuple<string, string>>;
                                while (reader.MoveToNextAttribute())
                                {
                                    Tuple<string, string> tuple = new Tuple<string, string>(reader.Name, reader.Value);
                                    templist.Add(tuple);
                                }
                            }
                            AddSection(offset, name, items, hasText);
                            //CurrentNode = (currentPanel as Section).GetXmlNode();
                            break;
                        case XmlNodeType.EndElement:
                            --offset;
                            CurrentNode = CurrentNode.ParentNode;
                            currentPanel = currentPanel.Parent as StackPanel;
                            break; 
                    }
                }
                reader.Close();
            }
        }

        private void New_Click(object sender, RoutedEventArgs e)
        {
            rootNode.RemoveAll();
            currentPanel = this.Content as StackPanel;
            currentPanel = currentPanel.FindName("World") as StackPanel;
            currentPanel.Children.RemoveRange(1, currentPanel.Children.Count);
            currentPanel.Name = "World";
            CurrentNode = rootNode;
        }
    }
}
