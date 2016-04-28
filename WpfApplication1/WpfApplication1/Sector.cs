using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.Windows.Media;
using System.Xml;

namespace WpfApplication1
{
    class Sector : System.Windows.Controls.StackPanel
    {
        public StackPanel mParent { get; set; }
        public XmlNode rootNode;
        public XmlNode SectorNode;
        public Sector(int offset, MainWindow main, StackPanel parent, XmlNode root, XmlDocument xmlDoc)
        {
            mParent = parent;
            rootNode = root;
            SectorNode = xmlDoc.CreateElement("Sector");
            XmlAttribute attribute = xmlDoc.CreateAttribute("Name");

            StackPanel SectorStack = new StackPanel();
            SectorStack.Background = Brushes.Green;
            SectorStack.Orientation = Orientation.Horizontal;
            for (int i = 0; i < offset; ++i)
            {
                TextBlock Offset = new TextBlock();
                Offset.Width = 100;
                Offset.Background = Brushes.White;
                SectorStack.Children.Add(Offset);
            }

            TextBlock TitleText = new TextBlock();
            TitleText.Text = "Sector";
            TitleText.Width = 100;
            SectorStack.Children.Add(TitleText);

            StackPanel NameStack = new StackPanel();
            TextBlock Name = new TextBlock();
            Name.Width = 100;
            Name.Text = "Name:";
            NameStack.Children.Add(Name);

            TextBox EnterName = new TextBox();
            EnterName.Width = 100;
            EnterName.TextChanged += new System.Windows.Controls.TextChangedEventHandler(delegate (Object o, System.Windows.Controls.TextChangedEventArgs a)
            {
                XmlAttribute attribut = xmlDoc.CreateAttribute("Name");
                attribut.Value = EnterName.Text;
                SectorNode.Attributes.Append(attribut);
            });
            NameStack.Children.Add(EnterName);
            SectorStack.Children.Add(NameStack);

            Button AddButton = new Button();
            AddButton.Content = "Add";
            AddButton.Width = 100;
            AddButton.Tag = (++offset).ToString();
            AddButton.Click += new System.Windows.RoutedEventHandler(delegate (Object o, System.Windows.RoutedEventArgs a)
            {
                var button = o as Button;
                main.currentPanel = this;
                main.CurrentNode = SectorNode;
                SelectionWindow win2 = new SelectionWindow(main, int.Parse(button.Tag.ToString()));
                win2.Show();
            });
            SectorStack.Children.Add(AddButton);

            Button RemoveButton = new Button();
            RemoveButton.Content = "Remove";
            RemoveButton.Width = 100;
            RemoveButton.Click += new System.Windows.RoutedEventHandler(delegate (Object o, System.Windows.RoutedEventArgs a)
            {
                mParent.Children.Remove(this);
                rootNode.RemoveChild(SectorNode);
            });
            SectorStack.Children.Add(RemoveButton);

            this.Children.Add(SectorStack);

            attribute.Value = EnterName.Text;
            SectorNode.Attributes.Append(attribute);
            rootNode.AppendChild(SectorNode);
        }
    }
}
