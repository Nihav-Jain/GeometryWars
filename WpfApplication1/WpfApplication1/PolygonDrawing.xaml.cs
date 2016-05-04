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
using System.Reflection;
using System.Windows.Media.Animation;
using System.Collections;

namespace WpfApplication1
{
    /// <summary>
    /// Interaction logic for PolygonDrawing.xaml
    /// </summary>
    public partial class PolygonDrawing : Window
    {
        Line Line;
        bool LineEnd = true;
        List<Line> Lines;
        Dictionary<Tuple<double, double>, int> mVertexDic;
        Dictionary<Tuple<double, double>, int> mVertexDicReferences;
        int mVertexCount = 0;
        MainWindow mWindow;
        private int mOffset { get; set; }

        public PolygonDrawing(MainWindow window, int offset)
        {
            InitializeComponent();
            Lines = new List<Line>();
            mVertexDic = new Dictionary<Tuple<double, double>, int>();
            mVertexDicReferences = new Dictionary<Tuple<double, double>, int>();
            mWindow = window;
            mOffset = offset;

            Line OriginY = new Line();
            OriginY.Y1 = 0;
            OriginY.Y2 = MainGrid.Height;
            OriginY.X1 = MainGrid.Width / 2;
            OriginY.X2 = MainGrid.Width / 2;
            OriginY.Visibility = System.Windows.Visibility.Visible;
            OriginY.StrokeThickness = 2;
            OriginY.Stroke = System.Windows.Media.Brushes.Black;
            MainGrid.Children.Add(OriginY);

            Line OriginX = new Line();
            OriginX.Y1 = MainGrid.Height / 2;
            OriginX.Y2 = MainGrid.Height / 2;
            OriginX.X1 = 0;
            OriginX.X2 = MainGrid.Width;
            OriginX.Visibility = System.Windows.Visibility.Visible;
            OriginX.StrokeThickness = 3;
            OriginX.Stroke = System.Windows.Media.Brushes.Black;
            MainGrid.Children.Add(OriginX);

            for(double i = MainGrid.Width/10; i < MainGrid.Width; i += MainGrid.Width/10)
            {
                Line gridLine = new Line();
                gridLine.Y1 = 0;
                gridLine.Y2 = MainGrid.Height;
                gridLine.X1 = i;
                gridLine.X2 = i;
                gridLine.Visibility = System.Windows.Visibility.Visible;
                gridLine.StrokeThickness = .5;
                gridLine.Stroke = System.Windows.Media.Brushes.Black;
                MainGrid.Children.Add(gridLine);
            }

            for (double i = MainGrid.Height / 10; i < MainGrid.Height; i += MainGrid.Height / 10)
            {
                Line gridLine = new Line();
                gridLine.Y1 = i;
                gridLine.Y2 = i;
                gridLine.X1 = 0;
                gridLine.X2 = MainGrid.Height;
                gridLine.Visibility = System.Windows.Visibility.Visible;
                gridLine.StrokeThickness = .5;
                gridLine.Stroke = System.Windows.Media.Brushes.Black;
                MainGrid.Children.Add(gridLine);
            }

            Type colorsType = typeof(System.Windows.Media.Colors);
            PropertyInfo[] colorsTypePropertyInfos = colorsType.GetProperties(BindingFlags.Public | BindingFlags.Static);

            foreach (PropertyInfo colorsTypePropertyInfo in colorsTypePropertyInfos)
            {
                ComboBoxItem cItem = new ComboBoxItem();
                cItem.Content = colorsTypePropertyInfo.Name;
                SolidColorBrush brush = new SolidColorBrush((Color)ColorConverter.ConvertFromString(colorsTypePropertyInfo.Name));
                cItem.Background = brush;
                Colorsbox.Items.Add(cItem);
            }
            Colorsbox.SelectedIndex = 7;

            for(double i = 1; i <= 9; ++i)
            {
                ComboBoxItem cItem = new ComboBoxItem();
                cItem.Content = i.ToString();
                Thickness.Items.Add(cItem);
            }
            Thickness.SelectedIndex = 2;
        }

        void Canvas_MouseLeftButtonDown(object sender, MouseButtonEventArgs args)
        {
            if (LineEnd)
            {
                Line = new Line();
                ColorConverter conv = new ColorConverter();
                Line.Stroke = new SolidColorBrush((Color)ColorConverter.ConvertFromString((Colorsbox.SelectedItem as ComboBoxItem).Content as string));
                Line.StrokeThickness = Convert.ToDouble((Thickness.SelectedItem as ComboBoxItem).Content as string);
                Line.StrokeStartLineCap = PenLineCap.Round;
                Line.StrokeEndLineCap = PenLineCap.Round;
                Line.StrokeDashCap = PenLineCap.Round;

                if (Lines.Count > 0)
                {
                    foreach(var value in mVertexDic)
                    {
                        double distance = Math.Pow((value.Key.Item1 - args.GetPosition(MainGrid).X), 2) + Math.Pow((value.Key.Item2 - args.GetPosition(MainGrid).Y), 2);
                        distance = Math.Sqrt(distance);
                        if (distance <= 10)
                        {
                            Line.X1 = value.Key.Item1;
                            Line.Y1 = value.Key.Item2;
                                
                            LineEnd = false;
                            MainGrid.Children.Add(Line);
                            mVertexDicReferences[new Tuple<double, double>(value.Key.Item1, value.Key.Item2)] += 1;
                            return;
                        }
                    }
                }

                Line.X1 = args.GetPosition(MainGrid).X;
                Line.Y1 = args.GetPosition(MainGrid).Y;
                LineEnd = false;
                mVertexDic.Add(new Tuple<double, double>(Line.X1, Line.Y1), mVertexCount++);
                mVertexDicReferences.Add(new Tuple<double, double>(Line.X1, Line.Y1), 1);

                MainGrid.Children.Add(Line);
                Colorsbox.IsEnabled = false;
                Thickness.IsEnabled = false;
            }
            else
            {
                foreach (var value in mVertexDic)
                {
                    double distance = Math.Pow((value.Key.Item1 - args.GetPosition(MainGrid).X), 2) + Math.Pow((value.Key.Item2 - args.GetPosition(MainGrid).Y), 2);
                    distance = Math.Sqrt(distance);
                    if (distance <= 10)
                    {
                        Line.X2 = value.Key.Item1;
                        Line.Y2 = value.Key.Item2;
                        Lines.Add(Line);
                        LineEnd = true;
                        Line = null;
                        mVertexDicReferences[new Tuple<double, double>(value.Key.Item1, value.Key.Item2)] += 1;
                        return;
                    }
                }

                Line.X2 = args.GetPosition(MainGrid).X;
                Line.Y2 = args.GetPosition(MainGrid).Y;
                Lines.Add(Line);
                mVertexDic.Add(new Tuple<double, double>(Line.X2, Line.Y2), mVertexCount++);
                mVertexDicReferences.Add(new Tuple<double, double>(Line.X2, Line.Y2), 1);
                Line = null;
                LineEnd = true;
            }            
        }

        private void MainGrid_MouseMove(object sender, MouseEventArgs e)
        {
            if (!LineEnd)
            {
                if (Line != null)
                {
                    Line.X2 = e.GetPosition(MainGrid).X;
                    Line.Y2 = e.GetPosition(MainGrid).Y;
                }
            }
        }

        private void ClearButton_Click(object sender, RoutedEventArgs e)
        {
            LineEnd = true;
            if(Line != null)
            {
                MainGrid.Children.Remove(Line);
            }
            foreach (var l in Lines)
            {
                MainGrid.Children.Remove(l);
            }
            Lines.Clear();
            mVertexDic.Clear();
            mVertexCount = 0;
            Colorsbox.IsEnabled = true;
            Thickness.IsEnabled = true;
        }

        private void UndoButton_Click(object sender, RoutedEventArgs e)
        {
            if(Line != null || Lines.Count > 0)
            {
                if (Line != null)
                {
                    MainGrid.Children.Remove(Line);
                    LineEnd = true;
                    foreach (var value in mVertexDic)
                    {
                        if (value.Key.Item1 == Line.X1 && value.Key.Item2 == Line.Y1)
                        {
                            mVertexDicReferences[value.Key]--;
                            if (mVertexDicReferences[value.Key] <= 0)
                            {
                                mVertexDicReferences.Remove(value.Key);
                                mVertexDic.Remove(value.Key);
                                mVertexCount--;
                                break;
                            }
                        }
                    }
                    Line = null;
                }
                else
                {
                    MainGrid.Children.Remove(Lines.Last());
                    Line = Lines.Last();
                    LineEnd = true;
                    foreach (var value in mVertexDic)
                    {
                        if (value.Key.Item1 == Line.X1 && value.Key.Item2 == Line.Y1)
                        {
                            mVertexDicReferences[value.Key]--;
                            if (mVertexDicReferences[value.Key] <= 0)
                            {
                                mVertexDicReferences.Remove(value.Key);
                                mVertexDic.Remove(value.Key);
                                mVertexCount--;
                                break;
                            }
                        }
                    }
                    foreach (var value in mVertexDic)
                    {
                        if (value.Key.Item1 == Line.X2 && value.Key.Item2 == Line.Y2)
                        {
                            mVertexDicReferences[value.Key]--;
                            if (mVertexDicReferences[value.Key] <= 0)
                            {
                                mVertexDicReferences.Remove(value.Key);
                                mVertexDic.Remove(value.Key);
                                mVertexCount--;
                                break;
                            }
                        }
                    }
                    Lines.Remove(Line);
                    Line = null;
                }
            }            
        }

        private void AddButton_Click(object sender, RoutedEventArgs e)
        {
            //polygon
            Hashtable items = new Hashtable();
            List<Tuple<string, string>> list = new List<Tuple<string, string>>();
            Tuple<string, string> tuple = new Tuple<string, string>("name", NameBox.Text);
            list.Add(tuple);
            items["polygon_renderer"] = list;
            Section section = mWindow.AddSection(mOffset++, "polygon_renderer", items, new Hashtable());

            //thickness
            AddPrimitiveSection("float", "width", (Thickness.SelectedItem as ComboBoxItem).Content as string, list, items);
            mWindow.AddInteranlSection(mOffset, section, section.SectorNode, "float", items, new Hashtable());

            //color
            Color color = (Color)ColorConverter.ConvertFromString((Colorsbox.SelectedItem as ComboBoxItem).Content as string);
            AddPrimitiveSection("vector", "color", "vec4(" + color.ScR.ToString() + ", " + color.ScG.ToString() + ", " + color.ScB.ToString() + ", 1)", list, items);
            mWindow.AddInteranlSection(mOffset, section, section.SectorNode, "vector", items, new Hashtable());

            //lines
            foreach( var line in Lines)
            {
                foreach(var value in mVertexDic)
                {
                    if(value.Key.Item1 == line.X1 && value.Key.Item2 == line.Y1)
                    {
                        AddPrimitiveSection("integer", "indices", (value.Value).ToString(), list, items);
                        mWindow.AddInteranlSection(mOffset, section, section.SectorNode, "integer", items, new Hashtable());
                    }
                }
                foreach (var value in mVertexDic)
                {
                    if (value.Key.Item1 == line.X2 && value.Key.Item2 == line.Y2)
                    {
                        AddPrimitiveSection("integer", "indices", (value.Value).ToString(), list, items);
                        mWindow.AddInteranlSection(mOffset, section, section.SectorNode, "integer", items, new Hashtable());
                    }
                }
            }

            foreach (var vertex in mVertexDic)
            {
                list.Clear();
                items.Clear();
                tuple = new Tuple<string, string>("name", "points");
                list.Add(tuple);
                double x = (vertex.Key.Item1 - (MainGrid.Width / 2)) / (MainGrid.Width / 2);
                x = Math.Round(x, 2);
                double y = ((vertex.Key.Item2 - (MainGrid.Height / 2)) / (MainGrid.Height / 2)) * -1;
                y = Math.Round(y, 2);
                tuple = new Tuple<string, string>("value", "vec4(" + x.ToString() + ", " + y.ToString() + ", 0, 0)");
                list.Add(tuple);
                items["vector"] = list;
                mWindow.AddInteranlSection(mOffset, section, section.SectorNode, "vector", items, new Hashtable());
            }

            this.Close();
        }

        private void AddPrimitiveSection(string type, string name, string value, List<Tuple<string, string>> list, Hashtable items)
        {
            list.Clear();
            items.Clear();
            Tuple<string, string> tuple = new Tuple<string, string>("name", name);
            list.Add(tuple);
            tuple = new Tuple<string, string>("value", value);
            list.Add(tuple);
            items[type] = list;
        }
    }
}
