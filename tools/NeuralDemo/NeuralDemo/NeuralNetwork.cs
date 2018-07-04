using System;
using System.Linq;

namespace NeuralDemo
{
    public class NeuralNetwork
    {
        public double _learningRate;
        public Matrix _weightHiddenOutput;
        public Matrix _weightInputHidden;

        public NeuralNetwork (double[][] weightHiddenOutput, double[][] weightInputHidden, double learningRate)
        {
            _learningRate = learningRate;

            _weightInputHidden = new Matrix(weightInputHidden);
            _weightHiddenOutput = new Matrix(weightHiddenOutput);
        }

        public NeuralNetwork(int numberOfInputNodes, int numberOfHiddenNodes, int numberOfOutputNodes, double learningRate)
        {
            _learningRate = learningRate;

            _weightInputHidden = Matrix.Create(numberOfHiddenNodes, numberOfInputNodes);
            _weightHiddenOutput = Matrix.Create(numberOfOutputNodes, numberOfHiddenNodes);

            RandomizeWeights();
        }

        private void RandomizeWeights()
        {
            var rnd = new Random();

            //distribute -0.5 to 0.5.
            _weightHiddenOutput.Initialize(() => rnd.NextDouble() - 0.5);
            _weightInputHidden.Initialize(() => rnd.NextDouble() - 0.5);
        }

        public void Train(double[] inputs, double[] targets)
        {
            var inputSignals = ConvertToMatrix(inputs);
            var targetSignals = ConvertToMatrix(targets);

            var hiddenOutputs = Sigmoid(_weightInputHidden * inputSignals);
            var finalOutputs = Sigmoid(_weightHiddenOutput * hiddenOutputs);

            var outputErrors = targetSignals - finalOutputs;
            
            var hiddenErrors = _weightHiddenOutput.Transpose() * outputErrors;

            _weightHiddenOutput += _learningRate * outputErrors * finalOutputs * (1.0 - finalOutputs) * hiddenOutputs.Transpose();
            _weightInputHidden += _learningRate * hiddenErrors * hiddenOutputs * (1.0 - hiddenOutputs) * inputSignals.Transpose();
        }
       
        public double[] Query(double[] inputs)
        {
            var inputSignals = ConvertToMatrix(inputs);

            var hiddenOutputs = Sigmoid(_weightInputHidden * inputSignals);
            var finalOutputs = Sigmoid(_weightHiddenOutput * hiddenOutputs);

            return finalOutputs.Value.SelectMany(x => x.Select(y => y)).ToArray();
        }

        private static Matrix ConvertToMatrix(double[] inputList)
        {
            var input = new double[inputList.Length][];

            for (var x = 0; x < input.Length; x++)
            {
                input[x] = new[] { inputList[x] };
            }

            return Matrix.Create(input);
        }

        private Matrix Sigmoid(Matrix matrix)
        {
            var newMatrix = Matrix.Create(matrix.Value.Length, matrix.Value[0].Length);

            for (var x = 0; x < matrix.Value.Length; x++)
            {
                for (var y = 0; y < matrix.Value[x].Length; y++)
                {
                    newMatrix.Value[x][y] = 1 / (1 + Math.Pow(Math.E, -matrix.Value[x][y]));
                }
            }

            return newMatrix;
        }
    }
}