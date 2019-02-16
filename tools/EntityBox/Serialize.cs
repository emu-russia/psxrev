// Serialization

using System;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Drawing.Imaging;
using System.IO;
using System.Xml.Serialization;
using System.Linq;

namespace System.Windows.Forms
{
    public partial class EntityBox : Control
    {


        public void Serialize(string FileName)
        {
            XmlSerializer ser = new XmlSerializer(typeof(List<Entity>));

            DeleteGarbage();

            using (FileStream fs = new FileStream(FileName, FileMode.Create))
            {
                ser.Serialize(fs, root.Children);
            }
        }

        public void Unserialize(string FileName, bool Append)
        {
            XmlSerializer ser = new XmlSerializer(typeof(List<Entity>));

            using (FileStream fs = new FileStream(FileName, FileMode.Open))
            {
                if (Append == false)
                    insertionNode.Children.Clear();

                insertionNode.Children = (List<Entity>)ser.Deserialize(fs);

                FixupParentLinksAndSelectionRecursive(insertionNode);

                DeleteGarbage();
                SortEntities();
                Invalidate();

                if (OnEntityCountChanged != null)
                    OnEntityCountChanged(this, EventArgs.Empty);

                UnserializeLastStamp = DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond;
            }
        }

        private void FixupParentLinksAndSelectionRecursive (Entity parent)
        {
            foreach (var entity in parent.Children)
            {
                entity.parent = parent;
                entity.Selected = SelectEntitiesAfterAdd;

                FixupParentLinksAndSelectionRecursive(entity);
            }
        }

    }
}
