// Deletion

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

        public void DeleteAllEntites()
        {
            foreach (Entity entity in GetEntities())
            {
                if (OnEntityRemove != null)
                    OnEntityRemove(this, entity, EventArgs.Empty);
            }

            root.Children.Clear();
            Invalidate();

            if (OnEntityCountChanged != null)
                OnEntityCountChanged(this, EventArgs.Empty);

            SetDestinationNode(root);
        }

        public void DeleteSelected()
        {
            bool UpdateRequired = false;
            List<Entity> pendingDelete = new List<Entity>();

            foreach (Entity entity in GetEntities())
            {
                if (entity.Selected == true)
                {
                    pendingDelete.Add(entity);
                    UpdateRequired = true;
                }
            }

            foreach (Entity entity in pendingDelete)
            {
                if (OnEntityRemove != null)
                    OnEntityRemove(this, entity, EventArgs.Empty);

                entity.parent.Children.Remove(entity);
            }

            if (UpdateRequired == true)
            {
                Invalidate();

                if (OnEntityCountChanged != null)
                    OnEntityCountChanged(this, EventArgs.Empty);
            }

            if (entityGrid != null)
                entityGrid.SelectedObject = null;

            SetDestinationNode(root);
        }


        private void DeleteGarbage()
        {
            //
            // Wipe small wires (< 1 lambda)
            //

            List<Entity> pendingDelete = new List<Entity>();

            foreach (Entity entity in GetEntities())
            {
                if (entity.IsWire())
                {
                    float len = (float)Math.Sqrt(Math.Pow(entity.LambdaEndX - entity.LambdaX, 2) +
                                                   Math.Pow(entity.LambdaEndY - entity.LambdaY, 2));

                    if (len < 1.0F)
                        pendingDelete.Add(entity);
                }
            }

            if (pendingDelete.Count > 0)
            {
                foreach (Entity entity in pendingDelete)
                {
                    entity.parent.Children.Remove(entity);
                }

                SetDestinationNode(root);
            }
        }


    }
}
