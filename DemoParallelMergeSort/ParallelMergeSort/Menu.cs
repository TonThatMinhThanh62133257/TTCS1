using SDL2;
using System.Runtime.InteropServices;

namespace ParallelMergeSort
{
    
    public partial class Menu : Form
    {
        private int nThreads, elementsPerProc, n, maxY, part, count = 0;
        private int[] array;
        private int[][] subsets;
        private IntPtr panelWindow, renderer;
        private Mutex mutRender = new Mutex(), mutSort = new Mutex();
        private class Const
        {
            public const int MIN = 1;
            public const int MAX = 500;
        }

        private void InitArray()
        {
            n = elementsPerProc * nThreads;
            array = new int[n];
            Random rand = new Random();
            for (int i = 0; i <= n - 1; i++)
                array[i] = rand.Next(Const.MIN, Const.MAX);
        }

        private void Swap(int i, int a, int b) 
        {
            mutRender.WaitOne();

            SDL.SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL.SDL_RenderDrawLine(renderer, i, maxY, i, maxY - a);
            SDL.SDL_RenderPresent(renderer);
            SDL.SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL.SDL_RenderDrawLine(renderer, i, maxY, i, maxY - a);
            SDL.SDL_RenderPresent(renderer);
            SDL.SDL_SetRenderDrawColor(renderer, 50, 205, 50, 255);
            SDL.SDL_RenderDrawLine(renderer, i, maxY, i, maxY - b);
            SDL.SDL_RenderPresent(renderer);

            SDL.SDL_Delay(1);
                
            mutRender.ReleaseMutex();
        }
        private void Sort(int[] array, int[] t1, int[] t2, int n1, int n2, int l)
        {
            int i = 0, j = 0, k = l;
            while (i < n1 && j < n2)
            {
                if (t1[i] < t2[j])
                {
                    Swap(k, array[k], t1[i]);
                    array[k] = t1[i];
                    i++;
                }
                else
                {
                    Swap(k, array[k], t2[j]);
                    array[k] = t2[j];
                    j++;
                }
                k++;
            }
            while (i < n1)
            {
                Swap(k, array[k], t1[i]);
                array[k] = t1[i];
                i++;
                k++;
            }
            while (j < n2)
            {
                Swap(k, array[k], t2[j]);
                array[k] = t2[j];
                j++;
                k++;
            }
        }
        private void Merge(int l, int m, int r)
        {
            int n1 = m - l + 1, n2 = r - m;
            int[] t1 = new int[n1], t2 = new int[n2];
            for (int i = 0; i <= n1 - 1; i++)
                t1[i] = array[l + i];
            for (int i = 0; i <= n2 - 1; i++)
                t2[i] = array[m + 1 + i];
            Sort(array, t1, t2, n1, n2, l);
        }
        private void MergeSort(int l, int r)
        {
            if (l < r)
            {
                int m = (l + r) / 2;
                MergeSort(l, m);
                MergeSort(m + 1, r);
                Merge(l, m, r);
            }
        }

        private void SortInit()
        {
            int threadPart = part++;
            int l = threadPart * elementsPerProc;
            int r = (threadPart + 1) * elementsPerProc - 1;
            int m = (l + r) / 2;
            if(l < r)
            {
                MergeSort(l, m);
                MergeSort(m + 1, r);
                Merge(l,m,r);
            }
        }

        private void DrawArray()
        {
            for(int i = 0; i <= n - 1;i++)
            {
                SDL.SDL_SetRenderDrawColor(renderer, 50, 205, 50, 255);
                SDL.SDL_RenderDrawLine(renderer, i, maxY, i, maxY - array[i]);
                SDL.SDL_RenderPresent(renderer);
            }   
        }

        private void MergeChunks(int[] array, int nThreads, int aggregation)
        {
            int l, m, r;
            for(int i = 0;i < nThreads;i += 2)
            {
                l = i*elementsPerProc*aggregation;
                r = (i + 2) * (elementsPerProc*aggregation) - 1;
                m = l + (elementsPerProc * aggregation) - 1;
                if(r >= n)
                    r = n - 1;
                Merge(l,m,r);
            }
            if(nThreads / 2 >= 1)
                MergeChunks(array, nThreads / 2, aggregation * 2);
        }

        private bool IsPowerOfTwo(int x)
        {
            return (x > 1) && ((x & (x - 1)) == 0);
        }
        private void button_Click(object sender, EventArgs e)
        {
            if(int.TryParse(inputBox.Text, out elementsPerProc) && int.TryParse(threadBox.Text,out nThreads))
            {
                int error = 0;
                if(elementsPerProc <= 0 || !(elementsPerProc >= 2 && elementsPerProc*nThreads <= 1024))
                {
                    MessageBox.Show("Số lượng phần tử <2 hoặc số lượng phần tử của từng tiến trình * số threads > 1024", "ERROR",MessageBoxButtons.OK, MessageBoxIcon.Error);
                    error = 1;
                }
                if(nThreads <= 0 || IsPowerOfTwo(nThreads) == false)
                {
                    MessageBox.Show("Số thread không phải là một dạng 2^n", "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    error = 1;
                }
                if (error == 1)
                    return;
                SDL.SDL_Init(SDL.SDL_INIT_VIDEO);
                InitArray();
                panelWindow = SDL.SDL_CreateWindowFrom(panel.Handle);
                renderer = SDL.SDL_CreateRenderer(panelWindow, -1, SDL.SDL_RendererFlags.SDL_RENDERER_SOFTWARE);
                maxY = panel.Size.Height;
                DrawArray();
                SDL.SDL_Delay(2000);
                Thread[] threads = new Thread[nThreads];
                part = 0;
                for (int i = 0; i < nThreads; i += 1)
                    threads[i] = new Thread(new ThreadStart(SortInit));
                for (int i = 0; i < nThreads; i++)
                    threads[i].Start();
                for (int i = 0; i < nThreads; i++)
                    threads[i].Join();
                MergeChunks(array, nThreads, 1);
                MessageBox.Show("Đã hoàn tất công việc sắp xếp!");
            }
        }
        
        public Menu()
        {
            InitializeComponent();
            button.Click += new EventHandler(button_Click);
        }
    }
}