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
            public int PCount;
            public int NCount;
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
#if DEBUG
            entityBox1.AssociateSelectionPropertyGrid(propertyGrid1);

            entityBox1.ViasBaseSize = 1;
#else
            propertyGrid1.Hide();
            splitContainer1.SplitterDistance = Width;
#endif
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
#if DEBUG
                Console.Clear();
#endif

                entityBox1.DeleteAllEntites();

                entityBox1.Unserialize(openFileDialog1.FileName, true);
            }
        }

        private void exportCellsAsPatternsdbTXTToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DialogResult result = saveFileDialog1.ShowDialog();

            if (result == DialogResult.OK)
            {
                if (!backgroundWorker1.IsBusy)
                {
                    backgroundWorker1.RunWorkerAsync();
                }
                else
                    MessageBox.Show("Patterns database is still generating...", "Busy");
            }
        }

        private void backgroundWorker1_DoWork(object sender, DoWorkEventArgs e)
        {
            ExportEntitiesAsPatternsDatabase(saveFileDialog1.FileName);
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

            cells.Clear();

            GenSingleCell(origin, origin, stop, 0, -1);

            Console.WriteLine("--- Wave scan stop ---");

            //
            // Generate patterns_db Text
            //

            GenPatternsTxt(filename);

            MessageBox.Show("Export complete. " + cells.Count.ToString() + " cells found.");
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

            if (color[0] < 15)
                color[0] = 15;
            if (color[1] < 15)
                color[1] = 15;
            if (color[2] < 15)
                color[2] = 15;

            float r = 0, phi = 0;

            for (r = keypointPolar.X; r < stopPolar.X; r += ScanWindowWidth)
            {
                if (LabeledViasFound && UnlabeledViasFound)
                    break;

                PointF vertPolarDelta = PlanarToPolar(vertDelta);
                PointF horzPolarDelta = PlanarToPolar(horzDelta);

                for (phi = horzPolarDelta.Y; phi < vertPolarDelta.Y; phi += 0.017F)
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

                    foreach (Entity entity in entityBox1.GetEntities())
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
                            !UnlabeledViasFound && LabeledViasFound )
                        {
                            if (entity.LambdaY > labeled.LambdaY && entity.LambdaX > labeled.LambdaX )
                            {
                                unlabeled = entity;
                                UnlabeledViasFound = true;
                            }
                        }
                    }

                    if (LabeledViasFound && UnlabeledViasFound)
                        break;
                }

                vertDelta.X -= ScanWindowWidth;
                horzDelta.Y += ScanWindowWidth;
            }

#if DEBUG
            Point startPoint = entityBox1.LambdaToScreen(-keyPoint.X, keyPoint.Y);
            PointF planarEnd = PolarToPlanar( new PointF(r, phi));
            Point endPoint = entityBox1.LambdaToScreen(-planarEnd.X, planarEnd.Y);
            Entity wire = entityBox1.AddWire(EntityType.WireInterconnect, startPoint.X, startPoint.Y, endPoint.X, endPoint.Y);
            wire.Label = Tier.ToString() + "," + Kpn.ToString();
#endif

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

            foreach ( Entity vias in entityBox1.GetEntities())
            {
                PointF point = new PointF(vias.LambdaX, vias.LambdaY);

                if ( (vias.Type == EntityType.ViasInput || 
                     vias.Type == EntityType.ViasOutput || 
                     vias.Type == EntityType.ViasInout) && 
                     cellArea.Contains(point) )
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

            cell.labeledPos = new PointF(labeled.LambdaX, labeled.LambdaY);

            cell.Label = labeled.Label;
            cell.Type = CellTypeByLabel(cell.Label);

            // Count transistors
            TransCount(cellArea, cell);

            cells.Add(cell);

            Console.WriteLine ( cell.Label + " vias collection:" );

            foreach ( Vias vias in cell.vias )
            {
                Console.WriteLine( vias.Type.ToString() + 
                                   " x: " + vias.posOffset.X.ToString() +
                                   ", y: " + vias.posOffset.Y.ToString());
            }

            //
            // Highlight cells without transistors
            //

            if ( cell.PCount == 0 || cell.NCount == 0)
            {
                foreach ( Entity entity in entityBox1.GetEntities())
                {
                    PointF point = new PointF(entity.LambdaX, entity.LambdaY);

                    if (cellArea.Contains(point))
                    {
                        entity.Selected = true;
                        entityBox1.Invalidate();
                    }
                }
            }

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

        private void TransCount ( RectangleF cellRect, Cell cell )
        {
            List<Entity> wires = new List<Entity>();
            List<Entity> preg = new List<Entity> ();
            List<Entity> nreg = new List<Entity> ();
            RectangleF prect = new RectangleF ( cellRect.X, cellRect.Y, 
                                                cellRect.Width / 2, cellRect.Height );
            RectangleF nrect = new RectangleF ( cellRect.X + cellRect.Width/2, cellRect.Y, 
                                                cellRect.Width / 2, cellRect.Height );

            Console.WriteLine("Begin trans count for " + cell.Label);

            //
            // Get cell wires (not m1)
            //

            foreach ( Entity entity in entityBox1.GetEntities())
            {
                PointF pointStart = new PointF(entity.LambdaX, entity.LambdaY);
                PointF pointEnd = new PointF(entity.LambdaEndX, entity.LambdaEndY);

                if ( entity.Type == EntityType.WireInterconnect && 
                     entity.ColorOverride != Color.Black && 
                    (cellRect.Contains(pointStart) || cellRect.Contains(pointEnd) ) )
                {
                    wires.Add (entity);
                }
            }

            //
            // Get p regions
            //

            foreach ( Entity entity in entityBox1.GetEntities())
            {
                if (entity.Type != EntityType.Region)
                    continue;

                PointF point = new PointF(entity.PathPoints[0].X, entity.PathPoints[0].Y);

                if (prect.Contains(point) && entity.ColorOverride == Color.Yellow)
                {
                    preg.Add (entity);
                }
            }

            //
            // Get n regions
            //

            foreach ( Entity entity in entityBox1.GetEntities())
            {
                if (entity.Type != EntityType.Region)
                    continue;

                PointF point = new PointF(entity.PathPoints[0].X, entity.PathPoints[0].Y);

                if ( nrect.Contains(point) && entity.ColorOverride == Color.Yellow )
                {
                    nreg.Add (entity);
                }
            }

            //
            // Count transistors
            //

            Console.WriteLine( "wires: " + wires.Count.ToString() + 
                               ", p-regions: " + preg.Count.ToString() +
                               ", n-regions: " + nreg.Count.ToString() );

            Console.WriteLine("------- p-count ----------");
            cell.PCount = CountTransInSection ( preg, wires );
            Console.WriteLine("------- n-count ---------");
            cell.NCount = CountTransInSection ( nreg, wires );

            Console.WriteLine(" ");
        }

        private int CountTransInSection ( List<Entity> regions, List<Entity> wires)
        {
            List<Entity> wiresInside = new List<Entity>();
            Entity[] wiresArray;

            //
            // Get all wires inside / crossing regions
            //

            foreach ( Entity region in regions )
            {
                foreach (Entity wire in wires)
                {
                    PointF pointStart = new PointF(wire.LambdaX, wire.LambdaY);
                    PointF pointEnd = new PointF(wire.LambdaEndX, wire.LambdaEndY);

                    if (PointInPoly(region.PathPoints.ToArray(), pointStart) ||
                        PointInPoly(region.PathPoints.ToArray(), pointEnd) ||
                        LineIntersectsPoly(region.PathPoints.ToArray(), pointStart, pointEnd) )
                    {
                        Console.WriteLine("wire: x=" + wire.LambdaX.ToString() + ", y=" + wire.LambdaY.ToString());

                        wire.UserData = 0;

                        wiresInside.Add(wire);
                    }
                }
            }

            wiresArray = wiresInside.ToArray();

            //
            // Eliminate attached wires (UserData contains flag)
            //

            int count = 0;

            for (int i = 0; i < wiresArray.Length; i++ )
            {
                if (wiresArray[i].UserData == 1)
                    continue;

                PointF pointStart = new PointF(wiresArray[i].LambdaX, wiresArray[i].LambdaY);
                PointF pointEnd = new PointF(wiresArray[i].LambdaEndX, wiresArray[i].LambdaEndY);

                // Mark
                wiresArray[i].UserData = 1;

                MarkWiresRecursive(pointStart, pointEnd, wiresArray);

                count++;
            }

            Console.WriteLine("Count=" + count.ToString());

            return count;
        }

        private void MarkWiresRecursive(PointF pointStart, PointF pointEnd, Entity[] wiresArray)
        {
            for (int j = 0; j < wiresArray.Length; j++)
            {
                if (wiresArray[j].UserData == 1)
                    continue;

                RectangleF rectStart = new RectangleF(wiresArray[j].LambdaX - 1, wiresArray[j].LambdaY - 1,
                                                        2, 2);
                RectangleF rectEnd = new RectangleF(wiresArray[j].LambdaEndX - 1, wiresArray[j].LambdaEndY - 1,
                                                        2, 2);

                //
                // Start attached - mark attached wire
                //

                if (rectStart.Contains(pointStart) || rectEnd.Contains(pointStart))
                {
                    wiresArray[j].UserData = 1;

                    PointF start = new PointF(wiresArray[j].LambdaX, wiresArray[j].LambdaY);
                    PointF end = new PointF(wiresArray[j].LambdaEndX, wiresArray[j].LambdaEndY);

                    MarkWiresRecursive(start, end, wiresArray);
                }

                //
                // End attached - mark attached wire
                //

                if (rectStart.Contains(pointEnd) || rectEnd.Contains(pointEnd))
                {
                    wiresArray[j].UserData = 1;

                    PointF start = new PointF(wiresArray[j].LambdaX, wiresArray[j].LambdaY);
                    PointF end = new PointF(wiresArray[j].LambdaEndX, wiresArray[j].LambdaEndY);

                    MarkWiresRecursive(start, end, wiresArray);
                }
            }
        }

        private float LeftmostEntity ()
        {
            float min = float.PositiveInfinity;

            foreach (Entity entity in entityBox1.GetEntities())
            {
                if (entity.Type != EntityType.ViasFloating)
                    continue;

                if (entity.LambdaX < min)
                    min = entity.LambdaX;
            }

            return min;
        }

        private float RightmostEntity()
        {
            float max = float.NegativeInfinity;

            foreach (Entity entity in entityBox1.GetEntities())
            {
                if (entity.Type != EntityType.ViasFloating)
                    continue;

                if (entity.LambdaX > max)
                    max = entity.LambdaX;
            }

            return max;
        }

        private float TopmostEntity ()
        {
            float min = float.PositiveInfinity;

            foreach (Entity entity in entityBox1.GetEntities())
            {
                if (entity.Type != EntityType.ViasFloating)
                    continue;

                if (entity.LambdaY < min)
                    min = entity.LambdaY;
            }

            return min;
        }

        private float BotmostEntity()
        {
            float max = float.NegativeInfinity;

            foreach (Entity entity in entityBox1.GetEntities())
            {
                if (entity.Type != EntityType.ViasFloating)
                    continue;

                if (entity.LambdaY > max)
                    max = entity.LambdaY;
            }

            return max;
        }

        private EntityType CellTypeByLabel (string Label)
        {
            if (Label.Contains("TRI", StringComparison.OrdinalIgnoreCase))
                return EntityType.CellBusSupp;
            if (Label.Contains("KEEPER", StringComparison.OrdinalIgnoreCase))
                return EntityType.CellBusSupp;
            if (Label.Contains("BUS", StringComparison.OrdinalIgnoreCase))
                return EntityType.CellBusSupp;

            if (Label.Contains("DFF", StringComparison.OrdinalIgnoreCase))
                return EntityType.CellFlipFlop;
            if (Label.Contains("LATCH", StringComparison.OrdinalIgnoreCase))
                return EntityType.CellLatch;

            if (Label.Contains("XNOR", StringComparison.OrdinalIgnoreCase))
                return EntityType.CellLogic;
            if (Label.Contains("XOR", StringComparison.OrdinalIgnoreCase))
                return EntityType.CellLogic;
            if (Label.Contains("AOI", StringComparison.OrdinalIgnoreCase))
                return EntityType.CellLogic;
            if (Label.Contains("OAI", StringComparison.OrdinalIgnoreCase))
                return EntityType.CellLogic;
            if (Label.Contains("NAND", StringComparison.OrdinalIgnoreCase))
                return EntityType.CellLogic;
            if (Label.Contains("AND", StringComparison.OrdinalIgnoreCase))
                return EntityType.CellLogic;
            if (Label.Contains("NOR", StringComparison.OrdinalIgnoreCase))
                return EntityType.CellLogic;
            if (Label.Contains("OR", StringComparison.OrdinalIgnoreCase))
                return EntityType.CellLogic;

            if (Label.Contains("BUF", StringComparison.OrdinalIgnoreCase))
                return EntityType.CellBuffer;
            if (Label.Contains("NOT", StringComparison.OrdinalIgnoreCase))
                return EntityType.CellNot;

            if (Label.Contains("HA", StringComparison.OrdinalIgnoreCase))
                return EntityType.CellAdder;
            if (Label.Contains("FA", StringComparison.OrdinalIgnoreCase))
                return EntityType.CellAdder;
            if (Label.Contains("CA", StringComparison.OrdinalIgnoreCase))
                return EntityType.CellAdder;
            if (Label.Contains("WS", StringComparison.OrdinalIgnoreCase))
                return EntityType.CellAdder;
            if (Label.Contains("ARRAY", StringComparison.OrdinalIgnoreCase))
                return EntityType.CellAdder;

            if (Label.Contains("IMUX", StringComparison.OrdinalIgnoreCase))
                return EntityType.CellMux;
            if (Label.Contains("MUX", StringComparison.OrdinalIgnoreCase))
                return EntityType.CellMux;
            if (Label.Contains("DEMUX", StringComparison.OrdinalIgnoreCase))
                return EntityType.CellMux;

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
        // Math
        //

        private bool PointInPoly(PointF[] poly, PointF point)
        {
            int max_point = poly.Length - 1;
            float total_angle = GetAngle(
                poly[max_point].X, poly[max_point].Y,
                point.X, point.Y,
                poly[0].X, poly[0].Y);

            for (int i = 0; i < max_point; i++)
            {
                total_angle += GetAngle(
                    poly[i].X, poly[i].Y,
                    point.X, point.Y,
                    poly[i + 1].X, poly[i + 1].Y);
            }

            return (Math.Abs(total_angle) > 0.000001);
        }

        private float GetAngle(float Ax, float Ay,
            float Bx, float By, float Cx, float Cy)
        {
            float dot_product = DotProduct(Ax, Ay, Bx, By, Cx, Cy);

            float cross_product = CrossProductLength(Ax, Ay, Bx, By, Cx, Cy);

            return (float)Math.Atan2(cross_product, dot_product);
        }

        private float DotProduct(float Ax, float Ay, float Bx, float By, float Cx, float Cy)
        {
            float BAx = Ax - Bx;
            float BAy = Ay - By;
            float BCx = Cx - Bx;
            float BCy = Cy - By;

            return (BAx * BCx + BAy * BCy);
        }

        private float CrossProductLength(float Ax, float Ay, float Bx, float By, float Cx, float Cy)
        {
            float BAx = Ax - Bx;
            float BAy = Ay - By;
            float BCx = Cx - Bx;
            float BCy = Cy - By;

            return (BAx * BCy - BAy * BCx);
        }

        private static bool LineIntersectsLine(PointF l1p1, PointF l1p2, PointF l2p1, PointF l2p2)
        {
            float q = (l1p1.Y - l2p1.Y) * (l2p2.X - l2p1.X) - (l1p1.X - l2p1.X) * (l2p2.Y - l2p1.Y);
            float d = (l1p2.X - l1p1.X) * (l2p2.Y - l2p1.Y) - (l1p2.Y - l1p1.Y) * (l2p2.X - l2p1.X);

            if (d == 0)
            {
                return false;
            }

            float r = q / d;

            q = (l1p1.Y - l2p1.Y) * (l1p2.X - l1p1.X) - (l1p1.X - l2p1.X) * (l1p2.Y - l1p1.Y);
            float s = q / d;

            if (r < 0 || r > 1 || s < 0 || s > 1)
            {
                return false;
            }

            return true;
        }

        private bool LineIntersectsPoly ( PointF [] poly, PointF lp1, PointF lp2 )
        {
            for (int i = 1; i < poly.Length; i++)
            {
                PointF poly1 = poly[i - 1];
                PointF poly2 = poly[i];

                if (LineIntersectsLine(lp1, lp2, poly1, poly2))
                    return true;
            }

            return false;
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
            "# Vias types: ViasInput, ViasOutput, ViasInout\r\n\r\n";

            foreach ( Cell cell in cells )
            {
                foreach ( Vias vias in cell.vias )
                {
                    textOut += TextOutVias(cell, vias);
                }
            }

            textOut +=

            "\r\n# EOF";

            File.WriteAllText(filename, textOut);
        }

        private string TextOutCell (Cell cell)
        {
            return "pattern " + cell.Label + 
                   ", " + cell.Type.ToString() + 
                   ", 5.0, " + cell.PCount.ToString()+
                   ", " + cell.NCount.ToString() +
                   ", \"patterns_db/" + cell.Label + ".jpg\"\r\n";
        }

        private string TextOutVias (Cell cell, Vias vias)
        {
            string text = "";

            text += "vias " + cell.Label +", ";

            if (vias.Label.Length == 0)
            {
                if (vias.Type == EntityType.ViasInput) text += "in";
                else if (vias.Type == EntityType.ViasOutput) text += "out";
                else if (vias.Type == EntityType.ViasInout) text += "inout";
                else return "";         // Skip
            }
            else text += vias.Label;
            
            text += ", " + vias.posOffset.X.ToString().Replace(',','.') +
                    ", " + vias.posOffset.Y.ToString().Replace(',', '.') + ", ";

            text += vias.Type.ToString();
            
            text += ", 0\r\n";

            return text;
        }
    }


    public static class StringExtensions
    {
        public static bool Contains(this string source, string toCheck, StringComparison comp)
        {
            return source.IndexOf(toCheck, comp) >= 0;
        }
    }
}
