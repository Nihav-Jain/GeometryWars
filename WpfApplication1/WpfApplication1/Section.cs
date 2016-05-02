using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.Windows.Media;
using System.Xml;
using System.Collections;

namespace WpfApplication1
{
    public class Section : System.Windows.Controls.StackPanel
    {
        public StackPanel mParent { get; set; }
        public int Offset { get; set; }
        public XmlNode rootNode;
        public XmlNode SectorNode;
        private static int StackLength = 100;
        public Section(int offset, MainWindow main, StackPanel parent, XmlNode root, XmlDocument xmlDoc, Hashtable table, string name, Hashtable hasText)
        {
            mParent = parent;
            rootNode = root;
            Offset = offset;
            if(name != "comment")
                SectorNode = xmlDoc.CreateElement(name);
            else
                SectorNode = xmlDoc.CreateComment("");

            StackPanel SectionStack = new StackPanel();
            switch (name)
            {
                case "Sector":
                    SectionStack.Background = Brushes.LightGreen;
                    break;
                case "Entity":
                    SectionStack.Background = Brushes.LightGoldenrodYellow;
                    break;
                case "Action":
                    SectionStack.Background = Brushes.LightCoral;
                    break;
                case "comment":
                    SectionStack.Background = Brushes.Wheat;
                    break;
                case "polygon_renderer":
                    SectionStack.Background = Brushes.Blue;
                    break;
                default:
                    SectionStack.Background = Brushes.Gray;
                    break;
            }
            
            SectionStack.Orientation = Orientation.Horizontal;
            
            // add white space
            for (int i = 0; i < offset; ++i)
            {
                TextBlock Offset = new TextBlock();
                Offset.Width = StackLength;
                Offset.Background = Brushes.White;
                SectionStack.Children.Add(Offset);
            }

            //add name block
            TextBlock TitleText = new TextBlock();
            TitleText.Text = name;
            TitleText.Width = StackLength;
            SectionStack.Children.Add(TitleText);

            if(name == "comment")
            {
                StackPanel CommentStack = new StackPanel();
                TextBlock Comment = new TextBlock();
                Comment.Width = StackLength * 3;
                Comment.Text = "Comment:";
                CommentStack.Children.Add(Comment);
                TextBox EnterComment = new TextBox();
                EnterComment.Width = StackLength * 3;
                EnterComment.TextChanged += new System.Windows.Controls.TextChangedEventHandler(delegate (Object o, System.Windows.Controls.TextChangedEventArgs a)
                {
                    SectorNode.Value = EnterComment.Text;
                });
                CommentStack.Children.Add(EnterComment);
                SectionStack.Children.Add(CommentStack);
            }

            //add attribute boxes
            List<Tuple<string, string>> list = table[name] as List<Tuple<string, string>>;
            foreach (Tuple<string, string> item in list)
            {
                StackPanel NameStack = new StackPanel();
                TextBlock Name = new TextBlock();
                Name.Width = StackLength;
                Name.Text = item.Item1;
                NameStack.Children.Add(Name);

                TextBox EnterName = new TextBox();
                EnterName.Width = StackLength;
                EnterName.Text = item.Item2;
                EnterName.TextChanged += new System.Windows.Controls.TextChangedEventHandler(delegate (Object o, System.Windows.Controls.TextChangedEventArgs a)
                {
                    XmlAttribute attribut = xmlDoc.CreateAttribute(item.Item1);
                    attribut.Value = EnterName.Text;
                    SectorNode.Attributes.Append(attribut);
                });
                XmlAttribute attribute = xmlDoc.CreateAttribute(item.Item1);
                attribute.Value = EnterName.Text;
                SectorNode.Attributes.Append(attribute);
                NameStack.Children.Add(EnterName);
                SectionStack.Children.Add(NameStack);
            }

            Button AddButton = new Button();
            if (hasText[name] != null)
            {
                StackPanel NameStack = new StackPanel();
                TextBlock Name = new TextBlock();
                Name.Width = StackLength*3;
                Name.Text = "Character Data";
                NameStack.Children.Add(Name);

                TextBox EnterText = new TextBox();
                EnterText.Width = StackLength*3;
                EnterText.Text = hasText[name] as string;
                //write xml char data
                EnterText.TextChanged += new System.Windows.Controls.TextChangedEventHandler(delegate (Object o, System.Windows.Controls.TextChangedEventArgs a)
                {
                    XmlText tex = xmlDoc.CreateTextNode(EnterText.Text);
                    tex.Value = EnterText.Text;
                    SectorNode.InnerText = tex.Value;
                    SectionStack.Children.Remove(AddButton);
                    this.Children.RemoveRange(1, this.Children.Count);
                });
                NameStack.Children.Add(EnterText);
                SectionStack.Children.Add(NameStack);
            }

            //XmlText text = xmlDoc.CreateTextNode("");
            //SectorNode.InnerText = text.Value;

            //add button
            if (name != "comment" && name != "polygon_renderer")
            {
                
                AddButton.Content = "Add";
                AddButton.Width = StackLength;
                AddButton.Tag = (++offset).ToString();
                AddButton.Click += new System.Windows.RoutedEventHandler(delegate (Object o, System.Windows.RoutedEventArgs a)
                {
                    var button = o as Button;
                    main.currentPanel = this;
                    main.CurrentNode = SectorNode;
                    SelectionWindow win2 = new SelectionWindow(main, int.Parse(button.Tag.ToString()));
                    win2.Show();
                });
                SectionStack.Children.Add(AddButton);
            }

            //remove button
            Button RemoveButton = new Button();
            RemoveButton.Content = "Remove";
            RemoveButton.Width = StackLength;
            RemoveButton.Click += new System.Windows.RoutedEventHandler(delegate (Object o, System.Windows.RoutedEventArgs a)
            {
                mParent.Children.Remove(this);
                rootNode.RemoveChild(SectorNode);
            });
            SectionStack.Children.Add(RemoveButton);

            this.Children.Add(SectionStack);
            rootNode.AppendChild(SectorNode);
        }

        public XmlNode GetXmlNode()
        {
            return SectorNode;
        }
     }
}
