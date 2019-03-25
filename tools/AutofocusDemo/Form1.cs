// Демка для обкатки алгоритмов автофокуса для привода микроскопа

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

// Базируется на:
// Dynamic evaluation of autofocusing for automated microscopic analysis of blood smear and pap smear
// http://amnl.mie.utoronto.ca/data/J25.pdf

namespace Autofocus
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void loadImageToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if ( openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                pictureBox1.Image = Image.FromFile(openFileDialog1.FileName);
            }
        }

        private float ThresoldedAbsoluteGradient(Bitmap bitmap, int x, int y)
        {
            float intensity1 = greyIntesity(bitmap.GetPixel(x + 1, y));
            float intensity2 = greyIntesity(bitmap.GetPixel(x, y));

            float diff = Math.Abs(intensity1 - intensity2);

            return diff;
        }

        private float greyIntesity (Color col)
        {
            return 0.2126F * (float)col.R + 0.7152F * (float)col.G + 0.0722F * (float)col.B;
        }

        /// <summary>
        /// Усредненная интенсивность серого для всего изображения
        /// </summary>
        /// <param name="bitmap"></param>
        /// <returns></returns>
        private float meanIntencity (Bitmap bitmap)
        {
            float sum = 0.0F;

            for (int y = 0; y < bitmap.Height; y++)
            {
                for (int x = 0; x < bitmap.Width; x++)
                {
                    sum += greyIntesity(bitmap.GetPixel(x, y));
                }
            }

            return sum / (float)(bitmap.Width * bitmap.Height);
        }

        private float Algo_ThresoldedAbsoluteGradient(Bitmap bitmap)
        {
            float sum = 0.0F;

            for (int y = 0; y < bitmap.Height; y++)
            {
                for (int x = 0; x < bitmap.Width - 1; x++)
                {
                    sum += ThresoldedAbsoluteGradient(bitmap, x, y);
                }
            }

            return sum;
        }

        private float Variance (Bitmap bitmap, int x, int y, float meanIntensity)
        {
            float intensity1 = greyIntesity(bitmap.GetPixel(x, y));

            float diff = (float)Math.Pow(intensity1 - meanIntensity, 2.0);

            return diff;
        }

        private float Algo_Variance (Bitmap bitmap)
        {
            float mean = meanIntencity(bitmap);

            float sum = 0.0F;

            for (int y = 0; y < bitmap.Height; y++)
            {
                for (int x = 0; x < bitmap.Width - 1; x++)
                {
                    sum += Variance(bitmap, x, y, mean);
                }
            }

            sum = sum * (1.0F / (float)(bitmap.Width * bitmap.Height));

            return sum;
        }

        /// <summary>
        /// Вычислить функцию
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button1_Click(object sender, EventArgs e)
        {
            if (pictureBox1.Image == null)
            {
                label1.Text = "Загрузите изображение";
                return;
            }

            Bitmap bitmap = new Bitmap(pictureBox1.Image);

            //float res = Algo_ThresoldedAbsoluteGradient(bitmap);

            // По тестам на крови выходит что это самый лучший алгоритм, а значит для микросхем тоже пойдет

            float res = Algo_Variance(bitmap);

            label1.Text = res.ToString();
        }
    }
}
