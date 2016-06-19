using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using System.Runtime.InteropServices;
using System.IO;

namespace PatternsDbGen
{
    public partial class Form1 : Form
    {
        [DllImport("kernel32")]
        static extern bool AllocConsole();

        private float ScanWindowWidth = 1.0F;   // lambdas
        private List<Cell> cells = new List<Cell>();

        private class Vias
        {
            public PointF posOffset;
            public string Label;
            public EntityType Type;
        }

        private class Cell 
        {
            public string Label;
            public List<Vias> vias;
            public EntityType Type;
            public PointF labeledPos;
        }

        public Form1()
        {
            InitializeComponent();

#if DEBUG
            AllocConsole ();
#endif
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            entityBox1.AssociateSelectionPropertyGrid(propertyGrid1);
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void loadEntitiesXMLToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DialogResult result = openFileDialog1.ShowDialog();

            if (result == DialogResult.OK)
            {
                entityBox1.Unserialize(openFileDialog1.FileName, true);
            }
        }

        private void exportCellsAsPatternsdbTXTToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DialogResult result = saveFileDialog1.ShowDialog();

            if (result == DialogResult.OK)
            {
                ExportEntitiesAsPatternsDatabase(saveFileDialog1.FileName);
            }
        }

        private void ExportEntitiesAsPatternsDatabase (string filename)
        {
            //
            // Locate propagation origin
            //

            PointF origin = new PointF ( LeftmostEntity() , TopmostEntity() );
            PointF stop = new PointF(RightmostEntity(), BotmostEntity());

            if (origin.X == float.PositiveInfinity || origin.Y == float.PositiveInfinity)
            {
                Console.WriteLine("Cannot find wave propagation origin.");
                return;
            }

            if (stop.X == float.NegativeInfinity || stop.Y == float.NegativeInfinity)
            {
                Console.WriteLine("Cannot find wave propagation stop.");
                return;
            }

            origin.X -= ScanWindowWidth;
            origin.Y -= ScanWindowWidth;
            stop.X += ScanWindowWidth;
            stop.Y += ScanWindowWidth;

            //
            // Flip screen coordinate space
            //

            origin.X *= -1;
            stop.X *= -1;

            //
            // Recursive wave propagation
            //

            Console.WriteLine("--- Start wave scan ---");

            GenSingleCell(origin, origin, stop, 0, -1);

            //
            // Generate patterns_db Text
            //

            GenPatternsTxt(filename);
        }

        private void GenSingleCell(PointF origin, PointF keyPoint, PointF stop, int Tier, int Kpn)
        {
            PointF[] kp = new PointF[3];
            Cell cell = new Cell();

            Console.WriteLine("Start from keypoint["+Tier.ToString()+","+Kpn.ToString()+"]: " +
                                keyPoint.ToString());

            //
            // Scan for labeled and unlabeled ViasFloating
            //

            bool LabeledViasFound = false;
            bool UnlabeledViasFound = false;
            Entity labeled = null;
            Entity unlabeled = null;

            PointF keypointPolar = PlanarToPolar(keyPoint);
            PointF originPolar = PlanarToPolar(origin);
            PointF stopPolar = PlanarToPolar(stop);

            PointF vertDelta = new PointF(keyPoint.X, keyPoint.Y);
            PointF horzDelta = new PointF(keyPoint.X, keyPoint.Y);

            Random rand = new Random();
            byte[] color = new byte[3];
            rand.NextBytes(color);

            for (float r = keypointPolar.X; r < stopPolar.X; r += ScanWindowWidth)
            {
                if (LabeledViasFound && UnlabeledViasFound)
                    break;

                PointF vertPolarDelta = PlanarToPolar(vertDelta);
                PointF horzPolarDelta = PlanarToPolar(horzDelta);

                for (float phi = horzPolarDelta.Y; phi < vertPolarDelta.Y; phi += 0.017F)
                {
                    PointF planar = PolarToPlanar(new PointF(r, phi));

                    planar.X *= -1;

                    // DEBUG
#if false
                    Point screen = entityBox1.LambdaToScreen ( planar.X, planar.Y );
                    Entity debugPoint = entityBox1.AddVias(EntityType.ViasConnect, screen.X, screen.Y,
                                Color.FromArgb(255, color[0], color[1], color[2]));
#endif

                    RectangleF rect = new RectangleF( planar.X, planar.Y,
                                                      ScanWindowWidth * 2, ScanWindowWidth * 2);

                    foreach (Entity entity in entityBox1._entities)
                    {
                        if (entity.Type == EntityType.ViasFloating && entity.Label.Length > 0 &&
                             rect.Contains(new PointF(entity.LambdaX, entity.LambdaY)) &&
                            !LabeledViasFound  &&
                            !CellExists( new PointF(entity.LambdaX, entity.LambdaY)) )
                        {
                            labeled = entity;
                            LabeledViasFound = true;
                        }

                        if (entity.Type == EntityType.ViasFloating && entity.Label.Length == 0 &&
                            rect.Contains(new PointF(entity.LambdaX, entity.LambdaY)) &&
                            !UnlabeledViasFound )
                        {
                            unlabeled = entity;
                            UnlabeledViasFound = true;
                        }
                    }

                    if (LabeledViasFound && UnlabeledViasFound)
                        break;
                }

                vertDelta.X -= ScanWindowWidth;
                horzDelta.Y += ScanWindowWidth;
            }

            if (!LabeledViasFound || !UnlabeledViasFound)
            {
                Console.WriteLine("Stop from keypoint["+Tier.ToString()+","+Kpn.ToString()+"]: " +
                                    keyPoint.ToString());
                return;
            }

            Console.WriteLine("labeled: X=" + labeled.LambdaX.ToString() + 
                               ", Y=" + labeled.LambdaY.ToString());
            Console.WriteLine("UN-labeled: X=" + unlabeled.LambdaX.ToString() + 
                               ", Y=" + unlabeled.LambdaY.ToString());
            Console.WriteLine(" --- ");

            //
            // Grab in / out viases within area
            //

            cell.vias = new List<Vias>();

            RectangleF cellArea = new RectangleF(labeled.LambdaX, labeled.LambdaY,
                                                   unlabeled.LambdaX - labeled.LambdaX,
                                                   unlabeled.LambdaY - labeled.LambdaY);

            foreach ( Entity vias in entityBox1._entities )
            {
                if ( vias.Type == EntityType.ViasInput || 
                     vias.Type == EntityType.ViasOutput || 
                     vias.Type == EntityType.ViasInout )
                {
                    Vias myVias = new Vias();

                    myVias.Label = vias.Label;
                    myVias.posOffset = new PointF(vias.LambdaX - labeled.LambdaX,
                                                    vias.LambdaY - labeled.LambdaY);
                    myVias.Type = vias.Type;

                    cell.vias.Add(myVias);
                }
            }

            //
            // Insert in list
            //

            // Now I have synaptic depression here ...
            cell.labeledPos = new PointF(labeled.LambdaX, labeled.LambdaY);

            cell.Label = labeled.Label;
            cell.Type = CellTypeByLabel(cell.Label);

            cells.Add(cell);

            //
            // Propagate keypoints
            //

            kp[0].X = keyPoint.X;
            kp[0].Y = unlabeled.LambdaY + ScanWindowWidth;
            kp[1].X = -(unlabeled.LambdaX + ScanWindowWidth);
            kp[1].Y = keyPoint.Y;
            kp[2].X = -(unlabeled.LambdaX + ScanWindowWidth);
            kp[2].Y = unlabeled.LambdaY + ScanWindowWidth;

            GenSingleCell(origin, kp[0], stop, Tier + 1, 0);
            GenSingleCell(origin, kp[1], stop, Tier + 1, 1);
            GenSingleCell(origin, kp[2], stop, Tier + 1, 2);
        }

        private float LeftmostEntity ()
        {
            float min = float.PositiveInfinity;

            foreach (Entity entity in entityBox1._entities)
            {
                if (!entityBox1.IsEntityVias(entity))
                    continue;

                if (entity.LambdaX < min)
                    min = entity.LambdaX;
            }

            return min;
        }

        private float RightmostEntity()
        {
            float max = float.NegativeInfinity;

            foreach (Entity entity in entityBox1._entities)
            {
                if (!entityBox1.IsEntityVias(entity))
                    continue;

                if (entity.LambdaX > max)
                    max = entity.LambdaX;
            }

            return max;
        }

        private float TopmostEntity ()
        {
            float min = float.PositiveInfinity;

            foreach (Entity entity in entityBox1._entities)
            {
                if (!entityBox1.IsEntityVias(entity))
                    continue;

                if (entity.LambdaY < min)
                    min = entity.LambdaY;
            }

            return min;
        }

        private float BotmostEntity()
        {
            float max = float.NegativeInfinity;

            foreach (Entity entity in entityBox1._entities)
            {
                if (!entityBox1.IsEntityVias(entity))
                    continue;

                if (entity.LambdaY > max)
                    max = entity.LambdaY;
            }

            return max;
        }

        private EntityType CellTypeByLabel (string Label)
        {
            return EntityType.CellOther;
        }

        private bool CellExists ( PointF pos)
        {
            foreach ( Cell cell in cells )
            {
                if (cell.labeledPos.X == pos.X && cell.labeledPos.Y == pos.Y)
                    return true;
            }
            return false;
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            MessageBox.Show("PatternsDbGen, version 1.0\n(c) 2016, psxdev.ru", "About PatternsDbGen");
        }

        //
        // Polar space conversion
        // 

        private PointF PlanarToPolar (PointF coord)
        {
            PointF polar = new PointF();
            polar.X = (float)Math.Sqrt(coord.X * coord.X + coord.Y * coord.Y);
            polar.Y = (float)Math.Atan2((float)coord.Y, (float)coord.X);
            return polar;
        }

        private PointF PolarToPlanar (PointF coord)
        {
            PointF planar = new PointF();
            planar.X = coord.X * (float)Math.Cos(coord.Y);
            planar.Y = coord.X * (float)Math.Sin(coord.Y);
            return planar;
        }

        //
        // Output cells in specified format
        //

        private void GenPatternsTxt (string filename)
        {
            string textOut = "";

            textOut +=

            "# Patterns database. Generated automatically by PatternsDbGen tool\r\n" +
            "\r\n" +
            "# Pattern Syntax:\r\n" +
            "# pattern name, type, source_lamda, pcount, ncount, image_path\r\n" +
            "\r\n" +
            "# Pattern types: CellNot, CellBuffer, CellMux, CellLogic, CellAdder, CellBusSupp, CellFlipFlop, CellLatch, CellOther\r\n" +
            "\r\n";

            //
            // Cells
            //

            foreach ( Cell cell in cells )
            {
                textOut += TextOutCell(cell);
            }

            textOut +=

            "\r\n" +
            "pattern UNK_SMALL, CellOther, 5.0, 0, 0, \"patterns_db/UNK_SMALL.jpg\"\r\n" +
            "pattern UNK_MED, CellOther, 5.0, 0, 0, \"patterns_db/UNK_MED.jpg\"\r\n" +
            "pattern UNK_BIG, CellOther, 5.0, 0, 0, \"patterns_db/UNK_BIG.jpg\"\r\n" +
            "pattern GARBAGE, CellOther, 5.0, 0, 0, \"patterns_db/GARBAGE.jpg\"\r\n";

            //
            // Viases
            //

            textOut +=

            "\r\n" +
            "# -----------------------------------------------------------------------------------\r\n" +
            "\r\n" +
            "# Vias Syntax:\r\n" +
            "# vias pattern_name, vias_name, offset_x, offset_y, type, reserved\r\n" +
            "\r\n" +
            "# Vias types: ViasInput, ViasOutput, ViasInout\r\n";

            textOut +=

            "\r\n# EOF";

            File.WriteAllText(filename, textOut);
        }

        private string TextOutCell (Cell cell)
        {
            return "pattern " + cell.Label + 
                   ", " + cell.Type.ToString() + 
                   ", 5.0, pc, nc, \"patterns_db/" + cell.Label + ".jpg\"\r\n";
        }
    }
}
