using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Serialization;
using System.Drawing.Imaging;
using System.IO;

namespace NeuralNetwork
{
    public class EntityNetwork
    {
        private const int imageSize = 16;

        private NeuralNetwork nn;
        public State _state;

        public class Feature
        {
            public string name;
            public string description;
            [XmlIgnore] public Image image;
            public string entities;

            [XmlElement("image")]
            [Browsable(false)]
            public byte[] _image
            {
                get
                {
                    if (image != null)
                    {
                        MemoryStream ms = new MemoryStream();
                        image.Save(ms, ImageFormat.Png);
                        return ms.ToArray();
                    }
                    else return null;
                }
                set
                {
                    if ( value == null)
                    {
                        image = null;
                    }
                    else
                    {
                        if (value == null)
                        {
                            image = null;
                        }
                        else
                        {
                            image = Image.FromStream(new MemoryStream(value));
                        }
                    }

                }
            }

            public Feature() {}

            public Feature(string name, string descr, Image image, string entities)
            {
                this.image = image;
                this.name = name;
                this.description = descr;
                this.entities = entities;
            }
        }

        public class State
        {
            public string name = "Click to rename";
            public double _learningRate;
            public double[][] _weightHiddenOutput;
            public double[][] _weightInputHidden;
            public List<Feature> features = new List<Feature>();
        }

        public EntityNetwork(List<Feature> features, int hiddenSize, float rate)
        {
            nn = new NeuralNetwork(imageSize * imageSize, hiddenSize, features.Count, rate);

            _state = new State();

            _state._learningRate = rate;
            _state._weightHiddenOutput = nn._weightHiddenOutput._matrix;
            _state._weightInputHidden = nn._weightInputHidden._matrix;
            _state.features = features;
        }

        public EntityNetwork(State state)
        {
            nn = new NeuralNetwork(state._weightHiddenOutput, state._weightInputHidden, state._learningRate);

            _state = state;
        }

        private double[] ConvertImageToVector(Image image)
        {
            Bitmap bitmap = new Bitmap(image);
            double[] inputs = new double[imageSize * imageSize];
            int i = 0;

            //
            // Create input vector
            //

            for (int y = 0; y < imageSize; y++)
            {
                for (int x = 0; x < imageSize; x++)
                {
                    Color color = bitmap.GetPixel(x, y);

                    double blue = color.B;
                    double green = color.G;
                    double red = color.R;

                    byte Y = (byte)((0.299 * red) + (0.587 * green) + (0.114 * blue));
                    inputs[i] = (double)Y / 256F;

                    i++;
                }
            }

            return inputs;
        }

        public static void ShowVector(double[] vector, int decimals,
              int lineLen, bool newLine)
        {
            for (int i = 0; i < vector.Length; ++i)
            {
                if (i > 0 && i % lineLen == 0) Console.WriteLine("");
                if (vector[i] >= 0) Console.Write(" ");

                if (vector[i] >= 0.9F)
                {
                    Console.Write("     ");
                }
                else
                {
                    Console.Write(vector[i].ToString("F" + decimals) + " ");
                }
            }
            if (newLine == true)
                Console.WriteLine("");
        }

        public void Train(Image image, int feature)
        {
            double[] inputs = ConvertImageToVector(image);

            ShowVector(inputs, 3, 16, true);

            //
            // Create outputs
            //

            double[] outputs = new double[_state.features.Count];

            for (int i = 0; i < _state.features.Count; i++)
            {
                if (i == feature)
                {
                    outputs[i] = 0.99;
                }
                else
                {
                    outputs[i] = 0.01;
                }
            }

            //
            // Train
            //

            nn.Train(inputs, outputs);

            //
            // Save state
            //

            _state._weightHiddenOutput = nn._weightHiddenOutput._matrix;
            _state._weightInputHidden = nn._weightInputHidden._matrix;
        }

        public int Guess(Image image, bool debug = false)
        {
            double[] inputs = ConvertImageToVector(image);

            if (debug)
            {
                ShowVector(inputs, 3, 16, true);
            }

            double[] outputs = nn.Query(inputs);

            if (debug)
            {
                int i = 0;

                foreach (double n in outputs)
                {
                    Console.Write(GetFeatureName(i) + ": ");
                    Console.Write(n.ToString() + ", ");
                    i++;
                }
                Console.Write("\n");
            }

            //
            // Choose max and make sure the network solution is correct
            //

            double maxValue = outputs.Max();
            int maxIndex = outputs.ToList().IndexOf(maxValue);

            if (outputs[maxIndex] > 0.95)
            {
                return maxIndex;
            }
            else
            {
                return -1;
            }
        }

        public Feature GetFeature (int feature)
        {
            if (feature < 0)
                return null;

            return _state.features[feature];
        }

        public string GetFeatureName (int feature)
        {
            if (feature < 0)
                return "Unknown";

            return _state.features[feature].name;
        }

        public int GetWindowSize ()
        {
            return imageSize;
        }

    }

}
