// Entity Selection-related

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

        public void RemoveSelection()
        {
            bool UpdateRequired = false;

            foreach (Entity entity in GetEntities())
            {
                if (entity.Selected == true)
                {
                    entity.Selected = false;
                    UpdateRequired = true;
                }
            }

            if (UpdateRequired == true)
                Invalidate();

            if (entityGrid != null)
                entityGrid.SelectedObject = null;
        }

        public void AssociateSelectionPropertyGrid(PropertyGrid propertyGrid)
        {
            entityGrid = propertyGrid;
        }

        public List<Entity> GetSelected()
        {
            List<Entity> _selected = new List<Entity>();
            List<Entity> _entities = GetEntities();

            foreach (Entity entity in _entities)
            {
                if (entity.Selected)
                {
                    _selected.Add(entity);
                }
            }

            return _selected;
        }

        public List<Entity> GetSelectedVias()
        {
            List<Entity> _selected = new List<Entity>();
            List<Entity> _entities = GetEntities();

            foreach (Entity entity in _entities)
            {
                if (entity.IsVias() && entity.Selected)
                {
                    _selected.Add(entity);
                }
            }

            return _selected;
        }

        public List<Entity> GetSelectedWires()
        {
            List<Entity> _selected = new List<Entity>();
            List<Entity> _entities = GetEntities();

            foreach (Entity entity in _entities)
            {
                if (entity.IsWire() && entity.Selected)
                {
                    _selected.Add(entity);
                }
            }

            return _selected;
        }

        public Entity GetLastSelected()
        {
            List<Entity> _selected = GetSelected();

            if (_selected.Count == 0)
            {
                return null;
            }

            Entity lastSelected =
                _selected.Where(m => m.SelectTimeStamp == _selected.Max(p => p.SelectTimeStamp)).FirstOrDefault();

            return lastSelected;
        }

        public void SelectEntity(Entity entity)
        {
            entity.Selected = true;

            if (OnEntitySelect != null)
                OnEntitySelect(this, entity, EventArgs.Empty);
        }

        //
        // Select All
        //

        public void SelectAll(EntitySelection kind = EntitySelection.All)
        {
            foreach (Entity entity in GetEntities())
            {
                switch (kind)
                {
                    case EntitySelection.Vias:
                        if (entity.IsVias())
                            SelectEntity(entity);
                        break;

                    case EntitySelection.Wire:
                        if (entity.IsWire())
                            SelectEntity(entity);
                        break;

                    case EntitySelection.Cell:
                        if (entity.IsCell())
                            SelectEntity(entity);
                        break;

                    default:
                    case EntitySelection.All:
                        SelectEntity(entity);
                        break;
                }
            }

            Invalidate();
        }

    }
}
