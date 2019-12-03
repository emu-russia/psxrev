/// Базовый класс, определяющий элемент изображения
/// Вся сцена строится из элементов

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Drawing;

namespace CanvasControl
{
    public enum ItemCategory
    {
        Unknown,                /// Не ассоциирован ни с чем
        Node,                   /// Ассоциирован с вершиной графа
        Edge,                   /// Ассоциирован с ребром графа
    }

    public class CanvasItem
    {
        /// <summary>
        /// Топология (все координаты являются мировыми координатами)
        /// </summary>

        public ItemCategory Category { get; set; } = ItemCategory.Unknown;
        public PointF Pos { get; set; } = new PointF();
        public PointF PosEnd { get; set; } = new PointF();
        public List<PointF> Points { get; set; } = new List<PointF>();

        /// <summary>
        /// Метрика
        /// </summary>

        public float Width { get; set; } = 0;
        public float Height { get; set; } = 0;

        /// <summary>
        /// Стиль
        /// </summary>

        public Color FrontColor { get; set; } = new Color();
        public Color BorderColor { get; set; } = Color.Black;
        public bool RoundedEdges { get; set; } = false;
        public int BorderWidth { get; set; } = 1;
        public bool Visible { get; set; } = true;
        public bool Selected { get; set; } = false;
        
        public object UserData = null;
        protected CanvasControl parentControl = null;

        public PointF SavedPos = new PointF();
        public PointF SavedPosEnd = new PointF();
        public List<PointF> SavedPoints = new List<PointF>();
        public virtual void Draw(Graphics gr)
        {
        }

        public void Select()
        {
            Selected = true;
        }

        public void Unselect()
        {
            Selected = false;
        }

        public void Show(bool show)
        {
            Visible = show;
        }

        public void SetParentControl(CanvasControl parent)
        {
            parentControl = parent;
        }

        public virtual bool HitTest (PointF point)
        {
            return false;
        }

        public virtual bool BoxTest(RectangleF rect)
        {
            return false;
        }

    }
}
