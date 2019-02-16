 using System;
 using System.Collections;
 using System.Drawing;
 using System.ComponentModel;
 using System.ComponentModel.Design.Serialization;
 using System.Globalization;
 using System.Reflection;    
 using System.Windows.Forms;

 public class PointFConverter : TypeConverter {

     public override bool CanConvertFrom(ITypeDescriptorContext context, Type sourceType) {
         return ((sourceType == typeof(string)) || base.CanConvertFrom(context, sourceType));
     }

     public override bool CanConvertTo(ITypeDescriptorContext context, Type destinationType) {
         return ((destinationType == typeof(InstanceDescriptor)) || base.CanConvertTo(context, destinationType));
     }

     public override object ConvertFrom(ITypeDescriptorContext context, CultureInfo culture, object value)
     {
         string str = value as string;
         if (value == null) return base.ConvertFrom(context, culture, value);
         str = str.Trim();
         if (str.Length == 0) return null;
         if (culture == null) culture = CultureInfo.CurrentCulture;
         char ch = culture.TextInfo.ListSeparator[0];
         string[] strArray = str.Split(new char[] { ch });
         float[] numArray = new float[strArray.Length];
         TypeConverter converter = TypeDescriptor.GetConverter(typeof(float));
         for (int i = 0; i < numArray.Length; i++)
         {
             numArray[i] = (float)converter.ConvertFromString(context, culture, strArray[i]);
         }
         if (numArray.Length != 2) throw new ArgumentException("Invalid format");
         return new PointF(numArray[0], numArray[1]);
     }

     public override object ConvertTo(ITypeDescriptorContext context, CultureInfo culture, object value, Type destinationType)
     {
         if (destinationType == null) throw new ArgumentNullException("destinationType");

         if (value is PointF)
         {
             if (destinationType == typeof(string))
             {
                 PointF point = (PointF)value;
                 if (culture == null) culture = CultureInfo.CurrentCulture;
                 string separator = culture.TextInfo.ListSeparator + " ";
                 TypeConverter converter = TypeDescriptor.GetConverter(typeof(float));
                 string[] strArray = new string[2];
                 int num = 0;
                 strArray[num++] = converter.ConvertToString(context, culture, point.X);
                 strArray[num++] = converter.ConvertToString(context, culture, point.Y);

                 return string.Join(separator, strArray);
             }
             if (destinationType == typeof(InstanceDescriptor))
             {
                 PointF point2 = (PointF)value;
                 ConstructorInfo constructor = typeof(PointF).GetConstructor(new Type[] { typeof(float), typeof(float) });
                 if (constructor != null) return new InstanceDescriptor(constructor, new object[] { point2.X, point2.Y });
             }
         }

         return base.ConvertTo(context, culture, value, destinationType);
     }

     public override object CreateInstance(ITypeDescriptorContext context, IDictionary propertyValues) {
         if (propertyValues == null) throw new ArgumentNullException("propertyValues");
         object xvalue = propertyValues["X"];
         object yvalue = propertyValues["Y"];
         if (((xvalue == null) || (yvalue == null)) || (!(xvalue is float) || !(yvalue is float))) {
             throw new ArgumentException("Invalid property value entry");
         }
         return new PointF((float)xvalue, (float)yvalue);
     }

     public override bool GetCreateInstanceSupported(ITypeDescriptorContext context) {
         return true;
     }

     public override PropertyDescriptorCollection GetProperties(ITypeDescriptorContext context, object value, Attribute[] attributes) {
         return TypeDescriptor.GetProperties(typeof(PointF), attributes).Sort(new string[] { "X", "Y" });
     }

     public override bool GetPropertiesSupported(ITypeDescriptorContext context) {
         return true;
     }
 }
