static void Main(string[] args)
        {
            int[,] array = new int[5, 5];
            InitializeArray(array);
            Console.WriteLine();
            Console.WriteLine("Min Element in array: {0}", MinElement(array));
            Console.ReadLine();
        }