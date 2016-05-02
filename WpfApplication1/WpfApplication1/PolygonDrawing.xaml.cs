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
        bool finished = false;
        MainWindow mWindow;
        private int mOffset { get; set; }

        public PolygonDrawing(MainWindow window, int offset)
        {
            InitializeComponent();
            Lines = new List<Line>();
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
            if(!finished)
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

                    Line.X1 = args.GetPosition(MainGrid).X;
                    Line.Y1 = args.GetPosition(MainGrid).Y;
                    LineEnd = false;

                    MainGrid.Children.Add(Line);
                    Colorsbox.IsEnabled = false;
                    Thickness.IsEnabled = false;
                }
                else
                {
                    lock (Line)
                    {
                        if (Lines.Count > 0)
                        {
                            double distance = Math.Pow((Lines[0].X1 - args.GetPosition(MainGrid).X), 2) + Math.Pow((Lines[0].Y1 - args.GetPosition(MainGrid).Y), 2);
                            distance = Math.Sqrt(distance);
                            if (distance <= 10)
                            {
                                Finish();
                                return;
                            }
                        }

                        Line.X2 = args.GetPosition(MainGrid).X;
                        Line.Y2 = args.GetPosition(MainGrid).Y;
                        Lines.Add(Line);

                        Line = new Line();
                        Line.Stroke = new SolidColorBrush((Color)ColorConverter.ConvertFromString((Colorsbox.SelectedItem as ComboBoxItem).Content as string));
                        Line.StrokeThickness = Convert.ToDouble((Thickness.SelectedItem as ComboBoxItem).Content as string);
                        Line.StrokeStartLineCap = PenLineCap.Round;
                        Line.StrokeEndLineCap = PenLineCap.Round;
                        Line.StrokeDashCap = PenLineCap.Round;

                        Line.X1 = args.GetPosition(MainGrid).X;
                        Line.Y1 = args.GetPosition(MainGrid).Y;
                        MainGrid.Children.Add(Line);
                    }
                }
            }
        }

        private void Finish()
        {
            Line.X2 = Lines[0].X1;
            Line.Y2 = Lines[0].Y1;
            Lines.Add(Line);
            Line = null;
            finished = true;
            return;
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

        private void FinsihButton_Click(object sender, RoutedEventArgs e)
        {
            if(Lines.Count > 0)
            {
                if(!finished)
                    Finish();
            }
        }

        private void ClearButton_Click(object sender, RoutedEventArgs e)
        {
            finished = false;
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
            Colorsbox.IsEnabled = true;
            Thickness.IsEnabled = true;
        }

        private void UndoButton_Click(object sender, RoutedEventArgs e)
        {
            finished = false;
            if (Line != null)
            {
                MainGrid.Children.Remove(Line);
            }
            if(Lines.Count > 0)
            {
                MainGrid.Children.Remove(Lines.Last());
                Lines.Remove(Lines.Last());
                if(Lines.Count > 0)
                {
                    Line = Lines.Last();
                }
                else
                {
                    LineEnd = true;
                    Colorsbox.IsEnabled = true;
                    Thickness.IsEnabled = true;
                }
            }
        }

        private void AddButton_Click(object sender, RoutedEventArgs e)
        {
            if(finished)
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
                int i;
                for (i = 0; i < Lines.Count - 1; )
                {
                    AddPrimitiveSection("integer", "indices", (i).ToString(), list, items);
                    mWindow.AddInteranlSection(mOffset, section, section.SectorNode, "integer", items, new Hashtable());

                    AddPrimitiveSection("integer", "indices", (++i).ToString(), list, items);
                    mWindow.AddInteranlSection(mOffset, section, section.SectorNode, "integer", items, new Hashtable());
                }
                AddPrimitiveSection("integer", "indices", (i).ToString(), list, items);
                mWindow.AddInteranlSection(mOffset, section, section.SectorNode, "integer", items, new Hashtable());
                AddPrimitiveSection("integer", "indices", "0", list, items);
                mWindow.AddInteranlSection(mOffset, section, section.SectorNode, "integer", items, new Hashtable());

                foreach (var line in Lines)
                {
                    list.Clear();
                    items.Clear();
                    tuple = new Tuple<string, string>("name", "points");
                    list.Add(tuple);
                    double x = (line.X1 - (MainGrid.Width / 2)) / (MainGrid.Width / 2);
                    double y = ((line.Y1 - (MainGrid.Height / 2)) / (MainGrid.Height / 2)) * -1;
                    tuple = new Tuple<string, string>("value", "vec4(" + x.ToString() + ", " + y.ToString() + ", 0, 0)");
                    list.Add(tuple);
                    items["vector"] = list;
                    mWindow.AddInteranlSection(mOffset, section, section.SectorNode, "vector", items, new Hashtable());
                }

                this.Close();
            }
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
