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
                OnEntityRemove?.Invoke(this, entity, EventArgs.Empty);
            }

            root.Children.Clear();
            Invalidate();

            OnEntityCountChanged?.Invoke(this, EventArgs.Empty);

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
                OnEntityRemove?.Invoke(this, entity, EventArgs.Empty);

                bool contains;
                HierarchyContainsDestinationNodeRecursive(entity, out contains);

                if (contains)
                {
                    SetDestinationNode(entity.parent != null ? entity.parent : root);
                }

                entity.parent.Children.Remove(entity);
            }

            if (UpdateRequired == true)
            {
                Invalidate();

                OnEntityCountChanged?.Invoke(this, EventArgs.Empty);
            }

            if (entityGrid != null)
                entityGrid.SelectedObject = null;
        }

        private void HierarchyContainsDestinationNodeRecursive (Entity entity, out bool contains)
        {
            if (entity == insertionNode)
            {
                contains = true;
                return;
            }

            foreach (var child in entity.Children)
            {
                HierarchyContainsDestinationNodeRecursive(child, out contains);
                if (contains)
                {
                    return;
                }
            }

            contains = false;
        }

        public void RemoveSmallWires (float smallerThanLambda)
        {
            List<Entity> pendingDelete = new List<Entity>();

            foreach (Entity entity in GetEntities())
            {
                if (entity.IsWire())
                {
                    if (entity.WireLength() < smallerThanLambda)
                        pendingDelete.Add(entity);
                }
            }

            if (pendingDelete.Count > 0)
            {
                foreach (Entity entity in pendingDelete)
                {
                    RemoveEntity(entity);
                }

                SetDestinationNode(root);
                Invalidate();
            }
        }

        private void DeleteGarbage()
        {
            //
            // Wipe small wires (< 1 lambda)
            //

            RemoveSmallWires(1.0F);
        }

        public void RemoveNonOrthogonalWires ()
        {
            List<Entity> pendingDelete = new List<Entity>();

            foreach (Entity entity in GetEntities())
            {
                if (entity.IsWire())
                {
                    float tangent = Math.Abs(entity.Tangent());

                    if (tangent < 0.015F)
                        continue;
                    if (tangent >= 100)
                        continue;

                    pendingDelete.Add(entity);
                }
            }

            if (pendingDelete.Count > 0)
            {
                foreach (Entity entity in pendingDelete)
                {
                    RemoveEntity(entity);
                }

                SetDestinationNode(root);
                Invalidate();
            }
        }

        public void RemoveEntity(Entity entity)
        {
            entity.parent.Children.Remove(entity);

            OnEntityRemove?.Invoke(this, entity, EventArgs.Empty);

            OnEntityCountChanged?.Invoke(this, EventArgs.Empty);
        }

    }
}
