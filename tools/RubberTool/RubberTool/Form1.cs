using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.Runtime.InteropServices;
using System.Xml.Serialization;

namespace RubberTool
{
    public partial class Form1 : Form
    {
        [DllImport("kernel32")]
        static extern bool AllocConsole();

        //
        // Индекс для генерации имени контрольной точки
        //

        private int kpIndexLeft = 1;
        private int kpIndexRight = 1;

        public Form1()
        {
            InitializeComponent();

#if DEBUG
            AllocConsole();
#endif

            Console.WriteLine("RubberTool 1.0");
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            //
            // Сделать ширину границы ячейки треугольной сетки после выделения минимальной
            //

            entityBox1.WireBaseSize = 1;
            entityBox2.WireBaseSize = 1;

#if !DEBUG
            debugToolStripMenuItem.Visible = false;
#endif
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FormAbout about = new FormAbout();
            about.ShowDialog();
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void keypointColorToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DialogResult res = colorDialog1.ShowDialog();

            if (res == DialogResult.OK)
            {
                entityBox1.ViasConnectColor = colorDialog1.Color;
                entityBox2.ViasConnectColor = colorDialog1.Color;

                entityBox1.Invalidate();
                entityBox2.Invalidate();
            }
        }

        private void loadLeftToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DialogResult result = openFileDialog1.ShowDialog();

            if ( result == DialogResult.OK)
            {
                entityBox1.Image0 = Image.FromFile(openFileDialog1.FileName);
            }
        }

        private void loadRightToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DialogResult result = openFileDialog1.ShowDialog();

            if (result == DialogResult.OK)
            {
                entityBox2.Image0 = Image.FromFile(openFileDialog1.FileName);
            }
        }

        private void toolStripButton1_Click(object sender, EventArgs e)
        {
            entityBox1.Mode = EntityMode.ViasConnect;
            toolStripButton1.BackColor = SystemColors.ControlDark;
            toolStripButton2.BackColor = SystemColors.Control;
        }

        private void toolStripButton2_Click(object sender, EventArgs e)
        {
            entityBox1.Mode = EntityMode.Selection;
            toolStripButton1.BackColor = SystemColors.Control;
            toolStripButton2.BackColor = SystemColors.ControlDark;
        }

        private void toolStripButton3_Click(object sender, EventArgs e)
        {
            entityBox2.Mode = EntityMode.ViasConnect;
            toolStripButton3.BackColor = SystemColors.ControlDark;
            toolStripButton4.BackColor = SystemColors.Control;
        }

        private void toolStripButton4_Click(object sender, EventArgs e)
        {
            entityBox2.Mode = EntityMode.Selection;
            toolStripButton3.BackColor = SystemColors.Control;
            toolStripButton4.BackColor = SystemColors.ControlDark;
        }

        private void toolStripButton5_Click(object sender, EventArgs e)
        {
            Random rnd = new Random();
            Color randomColor = Color.FromArgb(rnd.Next(256), rnd.Next(256), rnd.Next(256));

            entityBox1.RegionOverrideColor = randomColor;

            entityBox1.DrawRegionBetweenSelectedViases();
        }

        private void toolStripButton6_Click(object sender, EventArgs e)
        {
            Random rnd = new Random();
            Color randomColor = Color.FromArgb(rnd.Next(256), rnd.Next(256), rnd.Next(256));

            entityBox1.RegionOverrideColor = randomColor;

            entityBox2.DrawRegionBetweenSelectedViases();
        }

        private void clearLeftToolStripMenuItem_Click(object sender, EventArgs e)
        {
            entityBox1.Image0 = null;
        }

        private void clearRightToolStripMenuItem_Click(object sender, EventArgs e)
        {
            entityBox2.Image0 = null;
        }

        private void saveLeftToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DialogResult result = saveFileDialog1.ShowDialog();

            if (result == DialogResult.OK)
            {
                entityBox1.Image0.Save(saveFileDialog1.FileName);
            }
        }

        private void saveRightToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DialogResult result = saveFileDialog1.ShowDialog();

            if (result == DialogResult.OK)
            {
                entityBox2.Image0.Save(saveFileDialog1.FileName);
            }
        }

#region Названия контрольных точек

        private void entityBox1_OnEntitySelect(object sender, Entity entity, EventArgs e)
        {
            Entity last = entityBox1.GetLastSelected();

            toolStripTextBox1.Text = last.Label;
        }

        private void entityBox2_OnEntitySelect(object sender, Entity entity, EventArgs e)
        {
            Entity last = entityBox2.GetLastSelected();

            toolStripTextBox2.Text = last.Label;
        }

        private void toolStripTextBox1_Leave(object sender, EventArgs e)
        {
            Entity last = entityBox1.GetLastSelected();

            last.Label = toolStripTextBox1.Text;
        }

        private void toolStripTextBox2_Leave(object sender, EventArgs e)
        {
            Entity last = entityBox2.GetLastSelected();

            last.Label = toolStripTextBox2.Text;
        }

#endregion

        private void entityBox1_OnEntityAdd(object sender, Entity entity, EventArgs e)
        {
            if (EntityBox.IsEntityVias(entity))
            {
                entity.Label = "kp" + kpIndexLeft.ToString();
                kpIndexLeft++;

                ListInsertKeypoint(entity, true);
            }
        }

        private void entityBox2_OnEntityAdd(object sender, Entity entity, EventArgs e)
        {
            if (EntityBox.IsEntityVias(entity))
            {
                entity.Label = "kp" + kpIndexRight.ToString();
                kpIndexRight++;

                ListInsertKeypoint(entity, false);
            }
        }

        private void entityBox1_OnEntityRemove(object sender, Entity entity, EventArgs e)
        {
            if (EntityBox.IsEntityVias(entity))
            {
                ListRemoveKeypoint(entity, true);
            }
        }

        private void entityBox2_OnEntityRemove(object sender, Entity entity, EventArgs e)
        {
            if (EntityBox.IsEntityVias(entity))
            {
                ListRemoveKeypoint(entity, false);
            }
        }

        /// <summary>
        /// Из левой части с изображением и тремя точками нарисовать треугольник в правой части
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void drawTexturedTriangleFromLeftImageToolStripMenuItem_Click(object sender, EventArgs e)
        {
            List<Entity> _selectedLeft = entityBox1.GetSelected();
            List<Entity> _selectedRight = entityBox2.GetSelected();

            //
            // Проверить что изображение в левой части загружено
            //

            if ( entityBox1.Image0 == null )
            {
                MessageBox.Show("Load image on Left");
                return;
            }

            //
            // Проверить что ключевые точки слева выделены правильно
            //

            if (_selectedLeft.Count != 3 )
            {
                MessageBox.Show("Select exactly 3 keypoints to form triangle");
                return;
            }

            foreach ( Entity entity in _selectedLeft)
            {
                if ( !EntityBox.IsEntityVias(entity))
                {
                    MessageBox.Show("Select keypoints only");
                    return;
                }
            }

            //
            // Проверить что ключевые точки справа выделены правильно
            //

            if (_selectedRight.Count != 3)
            {
                MessageBox.Show("Select exactly 3 keypoints to form triangle");
                return;
            }

            foreach (Entity entity in _selectedRight)
            {
                if (!EntityBox.IsEntityVias(entity))
                {
                    MessageBox.Show("Select keypoints only");
                    return;
                }
            }

            //
            // Сформировать изображение справа
            //

            Bitmap bitmap = new Bitmap ( entityBox1.Image0.Width * 3, entityBox1.Image0.Height * 3);
            Graphics gr = Graphics.FromImage(bitmap);
            gr.Clear(Color.White);
            entityBox2.Image0 = bitmap;

            //
            // Сформировать параметры
            //

            Triangle sourceTri = new Triangle();

            Point a0 = entityBox1.LambdaToScreen(_selectedLeft[0].LambdaX, _selectedLeft[0].LambdaY);
            Point a1 = entityBox1.LambdaToScreen(_selectedLeft[1].LambdaX, _selectedLeft[1].LambdaY);
            Point a2 = entityBox1.LambdaToScreen(_selectedLeft[2].LambdaX, _selectedLeft[2].LambdaY);

            sourceTri.a = new Point2D(a0.X, a0.Y);
            sourceTri.b = new Point2D(a1.X, a1.Y);
            sourceTri.c = new Point2D(a2.X, a2.Y);

            Triangle destTri = new Triangle();

            Point b0 = entityBox1.LambdaToScreen(_selectedRight[0].LambdaX, _selectedRight[0].LambdaY);
            Point b1 = entityBox1.LambdaToScreen(_selectedRight[1].LambdaX, _selectedRight[1].LambdaY);
            Point b2 = entityBox1.LambdaToScreen(_selectedRight[2].LambdaX, _selectedRight[2].LambdaY);

            destTri.a = new Point2D(b0.X, b0.Y);
            destTri.b = new Point2D(b1.X, b1.Y);
            destTri.c = new Point2D(b2.X, b2.Y);

            NonAffineTransform.WarpTriangle(entityBox1.Image0,
                entityBox2.Image0,
                sourceTri, destTri);
        }

        private void lineCrossTestToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Vec v1 = new Vec(new Point2D(75F, 53F), new Point2D(96F,80F));
            Vec v2 = new Vec(new Point2D(250F, 50F), new Point2D(214F, 74F));

            Point2D cross = Geom.LineCross(v1, v2);

            Console.WriteLine( "X: " + cross.X.ToString() + ", Y:" + cross.Y.ToString() );
        }

        private void barycenterTestToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Triangle tri = new Triangle();

            tri.a = new Point2D(751, 294);
            tri.b = new Point2D(526, 546);
            tri.c = new Point2D(866, 626);

            Point2D c = Geom.BaryCenter(tri);

            Console.WriteLine("X: " + c.X.ToString() + ", Y: " + c.Y.ToString());
        }

        private void trilateralTestToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Triangle sourceTri = new Triangle();

            sourceTri.a = new Point2D(466, 191);
            sourceTri.b = new Point2D(285, 419);
            sourceTri.c = new Point2D(636, 462);

            Triangle destTri = new Triangle();

            destTri.a = new Point2D(316, 243);
            destTri.b = new Point2D(194, 350);
            destTri.c = new Point2D(383, 372);

            Point2D n = new Point2D(268, 285);

            Point2D n2 = NonAffineTransform.TrilateralXform(sourceTri, destTri, n);

            Console.WriteLine("X: " + n2.X.ToString() + ", Y: " + n2.Y.ToString());

        }

        /// <summary>
        /// Сгененрировать сетку слева
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void generateTrianglesToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ClearMesh(entityBox1);
            GenerateMesh(entityBox1);
        }

        /// <summary>
        /// Сгененрировать сетку справа
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void generateRightToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ClearMesh(entityBox2);
            GenerateMesh(entityBox2);
        }

        /// <summary>
        /// Сгенерировать и показать треугольную сетку
        /// </summary>
        /// <param name="box"></param>
        private void GenerateMesh ( EntityBox box )
        {
            Delaunay delaunay = new Delaunay();

            List<Point2D> points = new List<Point2D>();

            //
            // Получить ключевые точки
            //

            foreach (Entity entity in box._entities)
            {
                if (EntityBox.IsEntityVias(entity))
                {
                    Point p = box.LambdaToScreen(entity.LambdaX, entity.LambdaY);
                    Point2D point = new Point2D(p.X, p.Y);
                    points.Add(point);
                }
            }

            if ( points.Count < 3)
            {
                MessageBox.Show("3 or more keypoints required!", "Error");
                return;
            }

            //
            // Триангулируем
            //

            List<Triangle> mesh = delaunay.GenMesh(points);

            //
            // Отобразить треугольную сетку
            //

            Random rnd = new Random();

            foreach (Triangle tri in mesh)
            {
                Color randomColor = Color.FromArgb(rnd.Next(256), rnd.Next(256), rnd.Next(256));

                AddTriangle(box, tri, randomColor);
            }
        }

        private Entity AddTriangle ( EntityBox box, Triangle tri, Color color )
        {
            List<Point> points = new List<Point>();

            points.Add(new Point((int)tri.a.X, (int)tri.a.Y));
            points.Add(new Point((int)tri.b.X, (int)tri.b.Y));
            points.Add(new Point((int)tri.c.X, (int)tri.c.Y));

            return box.AddRegion(points, color);
        }

        /// <summary>
        /// Очистить треугольную сетку слева
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void clearLeftToolStripMenuItem2_Click(object sender, EventArgs e)
        {
            ClearMesh(entityBox1);
        }

        /// <summary>
        /// Очистить треугольную сетку справа
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void clearRightToolStripMenuItem2_Click(object sender, EventArgs e)
        {
            ClearMesh(entityBox2);
        }

        /// <summary>
        /// Очистить треугольную сетку
        /// </summary>
        /// <param name="box"></param>
        private void ClearMesh ( EntityBox box )
        {
            List<Entity> ents = new List<Entity>();

            foreach (Entity entity in box._entities)
            {
                if (EntityBox.IsEntityRegion(entity))
                {
                    ents.Add(entity);
                }
            }

            foreach (Entity entity in ents)
            {
                box._entities.Remove(entity);
            }

            box.Invalidate();
        }

        /// <summary>
        /// Резиновая трансфомация левого изображения в правые ключевые точки
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void leftRightToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if ( entityBox1.Image0 == null )
            {
                MessageBox.Show("Image not loaded!", "Error");
                return;
            }

            if ( !VerifyKeypoints(entityBox1, entityBox2))
            {
                MessageBox.Show("Keypoints doesn't match!", "Error");
                return;
            }

            RubberWarping(entityBox1, entityBox2);
        }

        /// <summary>
        /// Резиновая трансфомация правого изображения в левые ключевые точки
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void rightLeftToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (entityBox2.Image0 == null)
            {
                MessageBox.Show("Image not loaded!", "Error");
                return;
            }

            if (!VerifyKeypoints(entityBox2, entityBox1))
            {
                MessageBox.Show("Keypoints doesn't match!", "Error");
                return;
            }

            RubberWarping(entityBox2, entityBox1);
        }

        /// <summary>
        /// Верифицировать количество и названия ключевых точек
        /// </summary>
        /// <param name="sourceBox"></param>
        /// <param name="destBox"></param>
        /// <returns></returns>
        private bool VerifyKeypoints ( EntityBox sourceBox, EntityBox destBox )
        {
            //
            // Получить ключевые точки 
            //

            List<Point2D> pointsLeft = GetKeypoints(sourceBox);
            List<Point2D> pointsRight = GetKeypoints(destBox);

            foreach ( Point2D point in pointsLeft)
            {
                if ( MapPoint ( point, pointsRight) == null )
                {
                    return false;
                }
            }

            return true;
        }

        private List<Point2D> GetKeypoints ( EntityBox box )
        {
            List<Point2D> points = new List<Point2D>();

            foreach (Entity entity in box._entities)
            {
                if (EntityBox.IsEntityVias(entity))
                {
                    Point p = box.LambdaToScreen(entity.LambdaX, entity.LambdaY);
                    Point2D point = new Point2D(p.X, p.Y);
                    point.name = entity.Label;
                    points.Add(point);
                }
            }

            return points;
        }

        /// <summary>
        /// Резиновая трансформация изображений
        /// </summary>
        /// <param name="boxFrom"></param>
        /// <param name="boxTo"></param>
        private void RubberWarping ( EntityBox boxFrom, EntityBox boxTo )
        {
            Delaunay delaunay = new Delaunay();

            //
            // Получить ключевые точки 
            //

            List<Point2D> pointsLeft = GetKeypoints (boxFrom);
            List<Point2D> pointsRight = GetKeypoints(boxTo);

            //
            // Триангулируем
            //

            List<Triangle> mesh = delaunay.GenMesh(pointsLeft);

            //
            // Сформировать изображение справа
            //

            Bitmap bitmap = new Bitmap(boxFrom.Image0.Width * 3, boxFrom.Image0.Height * 3);
            Graphics gr = Graphics.FromImage(bitmap);
            gr.Clear(Color.Gray);
            boxTo.Image0 = bitmap;

            //
            // Трилатеральный перенос треугольников левого изображения в правое
            //

            foreach (Triangle sourceTri in mesh)
            {
                Triangle destTri = new Triangle();

                destTri.a = MapPoint(sourceTri.a, pointsRight);
                destTri.b = MapPoint(sourceTri.b, pointsRight);
                destTri.c = MapPoint(sourceTri.c, pointsRight);

                NonAffineTransform.WarpTriangle(boxFrom.Image0,
                    boxTo.Image0,
                    sourceTri, destTri);
            }
        }

        /// <summary>
        /// Найти контрольную точку в списке ассоциативно связанную по имени
        /// </summary>
        /// <param name="source"></param>
        /// <param name="dest"></param>
        /// <returns></returns>
        private Point2D MapPoint ( Point2D source, List<Point2D> dest)
        {
            foreach ( Point2D point in dest )
            {
                if ( point.name == source.name)
                {
                    return point;
                }
            }

            return null;
        }

        /// <summary>
        /// Удалить контрольные точки слева
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void clearLeftToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            RemoveKeypoints(entityBox1, true);
        }

        /// <summary>
        /// Удалить контрольные точки справа
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void clearRightToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            RemoveKeypoints(entityBox2, false);
        }

        /// <summary>
        /// Удалить контрольные точки
        /// </summary>
        /// <param name="box"></param>
        private void RemoveKeypoints ( EntityBox box, bool left )
        {
            List<Entity> kps = new List<Entity>();

            foreach (Entity entity in box._entities)
            {
                if (EntityBox.IsEntityVias(entity))
                {
                    kps.Add(entity);
                }
            }

            foreach (Entity entity in kps)
            {
                box._entities.Remove(entity);
                ListRemoveKeypoint(entity, left);
            }

            box.Invalidate();
        }

        /// <summary>
        /// Сохранить контрольные точки слева
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void saveLeftToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            DialogResult res = saveFileDialog2.ShowDialog();
            
            if (res == DialogResult.OK)
            {
                SaveKeypoints(entityBox1, saveFileDialog2.FileName);
            }
        }

        /// <summary>
        /// Сохранить контрольные точки справа
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void saveRightToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            DialogResult res = saveFileDialog2.ShowDialog();

            if (res == DialogResult.OK)
            {
                SaveKeypoints(entityBox2, saveFileDialog2.FileName);
            }
        }

        /// <summary>
        /// Загрузить контрольные точки слева
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void loadLeftToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            DialogResult res = openFileDialog2.ShowDialog();

            if ( res == DialogResult.OK)
            {
                LoadKeypoints(entityBox1, openFileDialog2.FileName, true);
            }
        }

        /// <summary>
        /// Загрузить контрольные точки справа
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void loadRightToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            DialogResult res = openFileDialog2.ShowDialog();

            if (res == DialogResult.OK)
            {
                LoadKeypoints(entityBox2, openFileDialog2.FileName, false);
            }
        }

        /// <summary>
        /// Сохранить контрольные точки в XML
        /// </summary>
        /// <param name="box"></param>
        /// <param name="filename"></param>
        private void SaveKeypoints (EntityBox box, string filename )
        {
            List<Entity> kps = new List<Entity>();

            foreach (Entity entity in box._entities)
            {
                if (EntityBox.IsEntityVias(entity))
                {
                    kps.Add(entity);
                }
            }

            XmlSerializer ser = new XmlSerializer(typeof(List<Entity>));

            using (FileStream fs = new FileStream(filename, FileMode.Create))
            {
                ser.Serialize(fs, kps);
            }
        }

        /// <summary>
        /// Загрузить контрольные точки из XML
        /// </summary>
        /// <param name="box"></param>
        /// <param name="filename"></param>
        private void LoadKeypoints ( EntityBox box, string filename, bool left)
        {
            XmlSerializer ser = new XmlSerializer(typeof(List<Entity>));

            using (FileStream fs = new FileStream(filename, FileMode.Open))
            {
                List<Entity> list = (List<Entity>)ser.Deserialize(fs);

                foreach (Entity entity in list)
                {
                    box._entities.Add(entity);
                    ListInsertKeypoint(entity, left);
                }

                box._entities = box._entities.OrderBy(o => o.Priority).ToList();

                box.Invalidate();
            }
        }

        /// <summary>
        /// Изменение автоподстановки индекса контрольных точек
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        /// 
        private void editKeypointIndexToolStripMenuItem_Click(object sender, EventArgs e)
        {
            FormKeypointIndex form = new FormKeypointIndex(kpIndexLeft, kpIndexRight);

            form.FormClosed += Form_FormClosed;
            form.ShowDialog();
        }

        private void Form_FormClosed(object sender, FormClosedEventArgs e)
        {
            FormKeypointIndex form = (FormKeypointIndex)sender;

            if (form.OkPressed)
            {
                kpIndexLeft = form.indexLeft;
                kpIndexRight = form.indexRight;
            }
        }

        #region List view of keypoints

        private void ListInsertKeypoint(Entity kp, bool left)
        {
            ListViewItem item = new ListViewItem(kp.Label);

            //
            // Проверить, если такая точка уже есть в списке, то просто обновить координаты
            //

            ListViewItem itemExists = CheckAlreadyPresent(kp.Label);

            if (itemExists != null)
            {
                if (left)
                {
                    itemExists.SubItems[1].Text =
                        kp.LambdaX.ToString() + "; " + kp.LambdaY.ToString();
                }
                else
                {
                    itemExists.SubItems[2].Text =
                        kp.LambdaX.ToString() + "; " + kp.LambdaY.ToString();
                }

                if ( itemExists.SubItems[1].Text == "" ||
                    itemExists.SubItems[2].Text == "" )
                {
                    itemExists.BackColor = Color.Tomato;
                }
                else
                {
                    itemExists.BackColor = SystemColors.Control;
                }
            }
            else
            {
                //
                // Иначе добавить новую
                //

                if (left)
                {
                    item.SubItems.Add(kp.LambdaX.ToString() + "; " + kp.LambdaY.ToString());
                    item.SubItems.Add("");
                }
                else
                {
                    item.SubItems.Add("");
                    item.SubItems.Add(kp.LambdaX.ToString() + "; " + kp.LambdaY.ToString());
                }

                item.BackColor = Color.Tomato;

                listView1.Items.Add(item);
            }
        }

        private ListViewItem CheckAlreadyPresent ( string name )
        {
            foreach (ListViewItem item in listView1.Items)
            {
                if (item.SubItems[0].Text == name )
                {
                    return item;
                }
            }

            return null;
        }

        private void ListRemoveKeypoint (Entity kp, bool left )
        {
            ListViewItem itemExists = CheckAlreadyPresent(kp.Label);

            if (itemExists != null)
            {
                if (left)
                {
                    itemExists.SubItems[1].Text = "";
                }
                else
                {
                    itemExists.SubItems[2].Text = "";
                }

                if (itemExists.SubItems[1].Text == "" ||
                    itemExists.SubItems[2].Text == "")
                {
                    itemExists.BackColor = Color.Tomato;
                }
                else
                {
                    itemExists.BackColor = SystemColors.Control;
                }

                if (itemExists.SubItems[1].Text == "" &&
                    itemExists.SubItems[2].Text == "" )
                {
                    listView1.Items.Remove(itemExists);
                }
            }
        }

        /// <summary>
        /// Обработка нажатия на DELETE
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void listView1_KeyUp(object sender, KeyEventArgs e)
        {
            if (listView1.SelectedIndices.Count == 0 )
            {
                return;
            }

            if (e.KeyCode == Keys.Delete)
            {
                ListViewItem item = listView1.SelectedItems[0];

                RemoveKeypointByName(entityBox1, item.SubItems[0].Text);
                RemoveKeypointByName(entityBox2, item.SubItems[0].Text);

                listView1.Items.Remove(item);
            }
        }

        /// <summary>
        /// Удалить контрольную точку по имени
        /// </summary>
        /// <param name="box"></param>
        /// <param name="name"></param>
        private void RemoveKeypointByName ( EntityBox box, string name )
        {
            Entity kp = null;

            foreach ( Entity entity in box._entities)
            {
                if (EntityBox.IsEntityVias (entity))
                {
                    if ( entity.Label == name )
                    {
                        kp = entity;
                    }
                }
            }

            if (kp != null )
            {
                box._entities.Remove(kp);
                box.Invalidate();
            }
        }

        /// <summary>
        /// Подсветить точки по клику на элемент списка
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void listView1_ItemSelectionChanged(object sender, ListViewItemSelectionChangedEventArgs e)
        {
            if (listView1.SelectedIndices.Count == 0)
            {
                return;
            }

            ListViewItem item = listView1.SelectedItems[0];

            string name = item.Text;

            entityBox1.RemoveSelection();
            entityBox2.RemoveSelection();

            foreach ( Entity entity in entityBox1._entities)
            {
                if ( EntityBox.IsEntityVias(entity) && entity.Label == name )
                {
                    entity.Selected = true;
                    entityBox1.EnsureVisible(entity);
                }
            }

            foreach (Entity entity in entityBox2._entities)
            {
                if (EntityBox.IsEntityVias(entity) && entity.Label == name)
                {
                    entity.Selected = true;
                    entityBox2.EnsureVisible(entity);
                }
            }

            entityBox1.Invalidate();
            entityBox2.Invalidate();
        }

        private void entityBox1_OnEntityScroll(object sender, Entity entity, EventArgs e)
        {
            if (EntityBox.IsEntityVias(entity))
            {
                UpdateKeypointCoords(entity, true);
            }
        }

        private void entityBox2_OnEntityScroll(object sender, Entity entity, EventArgs e)
        {
            if (EntityBox.IsEntityVias(entity))
            {
                UpdateKeypointCoords(entity, false);
            }
        }

        private void UpdateKeypointCoords ( Entity kp, bool left)
        {
            ListViewItem itemExists = CheckAlreadyPresent(kp.Label);

            if (itemExists != null)
            {
                if (left)
                {
                    itemExists.SubItems[1].Text = kp.LambdaX.ToString() + "; " + kp.LambdaY.ToString();
                }
                else
                {
                    itemExists.SubItems[2].Text = kp.LambdaX.ToString() + "; " + kp.LambdaY.ToString();
                }

                if (itemExists.SubItems[1].Text == "" ||
                    itemExists.SubItems[2].Text == "")
                {
                    itemExists.BackColor = Color.Tomato;
                }
                else
                {
                    itemExists.BackColor = SystemColors.Control;
                }
            }
        }

        #endregion

    }
}
