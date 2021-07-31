using NeuralNetwork;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using static NeuralNetwork.EntityNetwork;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;

namespace DerouteSharp
{
    public partial class FormCreateMLModel : Form
    {
        public NeuralNetwork.EntityNetwork nn = null;
        bool ShowMode = false;

        public FormCreateMLModel(bool showExisting, NeuralNetwork.EntityNetwork nnToShow)
        {
            InitializeComponent();
            ShowMode = showExisting;
            if (ShowMode)
            {
                nn = nnToShow;
                button1.Visible = false;
                textBox1.ReadOnly = true;
                textBox2.ReadOnly = true;
                textBox3.ReadOnly = true;
                Text = "Current ML Model";
            }
            textBox3.Focus();
            InitDataGrid();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            ComposeModel();
            Close();
        }

        private void ComposeModel()
        {
            // Limit between 10 ... 100000

            int hiddenLayer = Math.Min(100000, Math.Max(100, Convert.ToInt32(textBox1.Text)));

            // Learning rate between 0.1 ... 10.0

            float learningRate;
            float.TryParse(textBox2.Text, NumberStyles.Any, CultureInfo.InvariantCulture, out learningRate);
            learningRate = Math.Min(10.0F, Math.Max(0.1F, learningRate));

            nn = new EntityNetwork(ExtractDataGrid(), hiddenLayer, learningRate);

            nn._state.name = textBox3.Text;
        }

        private void InitDataGrid()
        {
            DataGridView grid = dataGridView1;

            grid.Columns.Clear();

            grid.AutoSizeColumnsMode =
                DataGridViewAutoSizeColumnsMode.AllCells;
            grid.AutoSizeRowsMode =
                DataGridViewAutoSizeRowsMode.AllCells;

            // Add fields

            DataGridViewTextBoxColumn textColumn1 = new DataGridViewTextBoxColumn();
            textColumn1.Name = "Name";
            textColumn1.SortMode = DataGridViewColumnSortMode.NotSortable;
            grid.Columns.Add(textColumn1);

            DataGridViewTextBoxColumn textColumn2 = new DataGridViewTextBoxColumn();
            textColumn2.Name = "Description";
            textColumn2.SortMode = DataGridViewColumnSortMode.NotSortable;
            grid.Columns.Add(textColumn2);

            DataGridViewImageColumn imageColumn = new DataGridViewImageColumn();
            imageColumn.Name = "Image";
            imageColumn.SortMode = DataGridViewColumnSortMode.NotSortable;
            DataGridViewCellStyle style = new DataGridViewCellStyle();
            imageColumn.DefaultCellStyle = style;
            grid.Columns.Add(imageColumn);

            DataGridViewTextBoxColumn textColumn3 = new DataGridViewTextBoxColumn();
            textColumn3.Name = "Entities";
            textColumn3.SortMode = DataGridViewColumnSortMode.NotSortable;
            grid.Columns.Add(textColumn3);

            if (ShowMode)
            {
                foreach (EntityNetwork.Feature feature in nn._state.features)
                {
                    object[] row = { feature.name,
                        feature.description,
                        feature.image };

                    int rowIndex = grid.Rows.Add(row);

                    DataGridViewImageCell imageCell = (DataGridViewImageCell)
                        grid.Rows[rowIndex].Cells[2];

                }
            }
        }

        private List<EntityNetwork.Feature> ExtractDataGrid()
        {
            List<EntityNetwork.Feature> items = new List<EntityNetwork.Feature>();

            DataGridViewTextBoxCell textCell;
            DataGridViewImageCell imageCell;

            DataGridView grid = dataGridView1;

            int rows = grid.Rows.Count;

            for (int i = 0; i < rows; i++)
            {
                textCell = (DataGridViewTextBoxCell)grid.Rows[i].Cells[0];          // name
                string name = (string)textCell.Value;

                if (name == null)
                    continue;

                if (name.Trim() == "")
                    continue;

                EntityNetwork.Feature item = new EntityNetwork.Feature();

                textCell = (DataGridViewTextBoxCell)grid.Rows[i].Cells[1];          // description
                if (textCell.Value != null)
                    item.description = ((string)textCell.Value).Trim();

                item.name = name.Trim();

                imageCell = (DataGridViewImageCell)grid.Rows[i].Cells[2];           // image
                item.image = (Image)imageCell.Value;

                // TODO: Entities

                items.Add(item);
            }

            return items;
        }

        private void dataGridView1_CellClick(object sender, DataGridViewCellEventArgs e)
        {
            int column = e.ColumnIndex;
            int row = e.RowIndex;

            switch (e.ColumnIndex)
            {
                // Load image

                case 2:
                    DataGridViewImageCell imageCell = (DataGridViewImageCell)
                        dataGridView1.Rows[row].Cells[column];

                    if (openFileDialog1.ShowDialog() == DialogResult.OK)
                    {
                        string path = openFileDialog1.FileName;
                        imageCell.Value = Image.FromFile(path);
                    }
                    break;

                // Edit feature entities

                case 3:
                    break;

            }
        }
    }
}
