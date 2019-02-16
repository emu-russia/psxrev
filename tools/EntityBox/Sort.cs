// Priority stuff

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
        public List<Entity> SortEntities()
        {
            List<Entity> _entities = GetEntities();

            if (AutoPriority == true)
                _entities = _entities.OrderBy(o => o.Priority).ToList();

            return _entities;
        }

        public List<Entity> SortEntitiesReverse()
        {
            List<Entity> _entities = GetEntities();

            List<Entity> reversed = _entities.OrderByDescending(o => o.Priority).ToList();

            return reversed;
        }

    }
}
