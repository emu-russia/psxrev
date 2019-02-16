using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using NeuralNetwork;

namespace MachineLearningKit
{
    public partial class FormCollection : Form
    {
        public List<EntityNetwork.Feature> features;

        public FormCollection(List<EntityNetwork.Feature> features)
        {
            InitializeComponent();

            this.features = features;

            PopulateDataGrid(features);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            features = ExtractDataGrid();

            DialogResult = DialogResult.OK;
            Close();
        }

        private List<EntityNetwork.Feature> ExtractDataGrid ()
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

                items.Add(item);
            }

            return items;
        }

        private void PopulateDataGrid (List<EntityNetwork.Feature> features)
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

            foreach (EntityNetwork.Feature feature in features)
            {
                object[] row = { feature.name,
                        feature.description,
                        feature.image };

                int rowIndex = grid.Rows.Add(row);

                DataGridViewImageCell imageCell = (DataGridViewImageCell)
                    grid.Rows[rowIndex].Cells[2];

            }

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

            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            features = new List<EntityNetwork.Feature>();

            PopulateDataGrid(features);
        }
    }
}
