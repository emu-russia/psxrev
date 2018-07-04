using System;
using System.Threading.Tasks;

namespace NeuralDemo
{
    public class Matrix
    {
        public readonly double[][] _matrix;

        public Matrix(int rows, int cols)
        {
            _matrix = new double[rows][];

            for (var i = 0; i < rows; i++)
            {
                _matrix[i] = new double[cols];
            }
        }

        public Matrix(double[][] array)
        {
            _matrix = array;
        }

        private static double[][] CreateJagged(int rows, int cols)
        {
            var jagged = new double[rows][];

            for (var i = 0; i < rows; i++)
            {
                jagged[i] = new double[cols];
            }

            return jagged;
        }

        public static Matrix Create(int rows, int cols)
        {
            return new Matrix(rows, cols);
        }

        public static Matrix Create(double[][] array)
        {
            return new Matrix(array);
        }

        public void Initialize(Func<double> elementInitializer)
        {
            for (var x = 0; x < _matrix.Length; x++)
            {
                for (var y = 0; y < _matrix[x].Length; y++)
                {
                    _matrix[x][y] = elementInitializer();
                }
            }
        }

        public double[][] Value => _matrix;

        public static Matrix operator -(Matrix a, Matrix b)
        {
            var newMatrix = CreateJagged(a.Value.Length, b.Value[0].Length);

            for (var x = 0; x < a.Value.Length; x++)
            {
                for (var y = 0; y < a.Value[x].Length; y++)
                {
                    newMatrix[x][y] = a.Value[x][y] - b.Value[x][y];
                }
            }

            return Create(newMatrix);
        }

        public static Matrix operator +(Matrix a, Matrix b)
        {
            var newMatrix = CreateJagged(a.Value.Length, b.Value[0].Length);

            for (var x = 0; x < a.Value.Length; x++)
            {
                for (var y = 0; y < a.Value[x].Length; y++)
                {
                    newMatrix[x][y] = a.Value[x][y] + b.Value[x][y];
                }
            }

            return Create(newMatrix);
        }

        public static Matrix operator +(Matrix a, double b)
        {
            for (var x = 0; x < a.Value.Length; x++)
            {
                for (var y = 0; y < a.Value[x].Length; y++)
                {
                    a.Value[x][y] = a.Value[x][y] + b;
                }
            }

            return a;
        }

        public static Matrix operator -(double a, Matrix m)
        {
            for (var x = 0; x < m.Value.Length; x++)
            {
                for (var y = 0; y < m.Value[x].Length; y++)
                {
                    m.Value[x][y] = a - m.Value[x][y];
                }
            }

            return m;
        }

        public static Matrix operator *(Matrix a, Matrix b)
        {
            if (a.Value.Length == b.Value.Length && a.Value[0].Length == b.Value[0].Length)
            {
                var m = CreateJagged(a.Value.Length, a.Value[0].Length);

                Parallel.For(0, a.Value.Length, i =>
                {
                    for (var j = 0; j < a.Value[i].Length; j++)
                    {
                        m[i][j] = a.Value[i][j] * b.Value[i][j];
                    }
                });

                return Create(m);
            }

            var newMatrix = CreateJagged(a.Value.Length, b.Value[0].Length);

            if (a.Value[0].Length == b.Value.Length)
            {
                var length = a.Value[0].Length;

                Parallel.For(0, a.Value.Length, i =>
                {
                    for (var j = 0; j < b.Value[0].Length; j++)
                    {
                        var temp = 0.0;

                        for (var k = 0; k < length; k++)
                        {
                            temp += a.Value[i][k] * b.Value[k][j];
                        }

                        newMatrix[i][j] = temp;
                    }
                });
            }

            return Create(newMatrix);
        }

        public static Matrix operator *(double scalar, Matrix b)
        {
            var newMatrix = CreateJagged(b.Value.Length, b.Value[0].Length);

            for (var x = 0; x < b.Value.Length; x++)
            {
                for (var y = 0; y < b.Value[x].Length; y++)
                {
                    newMatrix[x][y] = b.Value[x][y] * scalar;
                }
            }

            return Create(newMatrix);
        }

        public Matrix Transpose()
        {
            var rows = _matrix.Length;

            var newMatrix = CreateJagged(_matrix[0].Length, rows); //rows --> cols, cols --> rows

            for (var row = 0; row < rows; row++)
            {
                for (var col = 0; col < _matrix[row].Length; col++)
                {
                    newMatrix[col][row] = _matrix[row][col];
                }
            }

            return Create(newMatrix);
        }
    }
}