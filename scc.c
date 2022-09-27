#include<stdio.h>
#include<stdlib.h>
#define endl printf("\n")

int time;

char* color; // global pointer to array which will store colors of all vertices
int* d; // global pointer to array which stores discovery time
int* pi; // global pointer to array which stores previous vertex for all vertices
int* f; // global pointer to array which stores finishing time for all vertices
int* topo;

// same variables as above but for the DFS on the transpose graph
char* colort;
int* dt;
int* pit;
int* ft;
int l; // indicates number of strongly connected components

struct node // structure for each node of the linked list
{
    int conn;
    struct node* next;
    char edge;
};

struct list
{
    struct node* one; // points to the starting of the list
};

struct graph // structure for the graph
{
    int vert; // no. of vertices
    struct list* arr;
};

struct node* addlist (int x) // creates a new node with entry x
{
    struct node* first;
    first = (struct node*)malloc(sizeof(struct node));

    first->conn = x;
    first->next = NULL;

    return first;
}

struct graph* generate_graph (int x) // generates a graph of appropriate size with entries initialized to null
{
    struct graph* g;
    g = (struct graph*)malloc(sizeof(struct graph));

    g->vert = x;

    g->arr = (struct list*)malloc(sizeof(struct list) * x);

    for (int i = 0; i < x; i++)
    {
        g->arr[i].one = NULL;
    }

    return g;
}

int search_edge(struct graph* g, int pt1, int pt2) // function for searching whether an edge is present in the graph or not
{
    struct node* dup = g->arr[pt1].one;
    if (dup == NULL)
    {
        ;
    }
    else
    {
        while(dup!=NULL)
        {
            if (dup->conn == pt2)
            {
                return 1;
            }
            dup = dup->next;
        }
    }
    return 0;
}

void create_directed_edge (struct graph* g, int pt1, int pt2) // function for adding an edge to a directed graph
{
    int p;
    p = search_edge(g, pt1, pt2);

    if (p == 1)
    {
        return;
    }
    struct node* temp = addlist(pt2);

    temp->next = g->arr[pt1].one;
    g->arr[pt1].one = temp;
}

void display_graph (struct graph* g) // function for displaying the graph
{
    for (int i = 0; i < g->vert; i++)
    {
        printf("Vertex %d: ", i);

        struct node* dup = g->arr[i].one;

        if(dup == NULL)
        {
            ;
        }
        else
        {
            while(dup->next!= NULL)
            {
                printf("%d -> ", dup->conn);
                dup = dup->next;
            }
            printf("%d", dup->conn);
        }
        endl;
    }
}

void DFS_VISIT (struct graph* g, int u) // DFS visit function as per CLRS pseudocode
{
    time = time + 1;
    *(d + u) = time;
    *(color + u) = 'g';

    int v;

    struct node* dup = g->arr[u].one;

    if(dup == NULL)
    {
        ;
    }
    else
    {
        while(dup!= NULL)
        {
            v = dup->conn;

                if(*(color + v) == 'w') // w stands for white color
                {
                    dup->edge = 'T'; // T stands for tree edge
                }
                else if (*(color + v) == 'g') // g stands for gray vertex
                {
                    dup->edge = 'B'; // B stands for back edge
                }
                else
                {
                    if(*(d + u) > *(d + v))
                    {
                        dup->edge = 'C'; // C stands for cross edge
                    }
                    else if (*(d + u) < *(d + v))
                    {
                        dup->edge = 'F'; // F stands for forward edge
                    }
                }

            if (*(color + v) == 'w')
            {
                *(pi + v) = u;
                DFS_VISIT (g, v);
            }
            dup = dup->next;
        }
    }
    *(color + u) = 'b'; // b stands for black color
    time = time + 1;
    *(f + u) = time;
}

void DFS (struct graph* g) // DFS function as per the CLRS pseudocode
{
    int n = g->vert;

    color = malloc(sizeof(char)*n);
    d = malloc(sizeof(int)*n);
    pi = malloc(sizeof(int)*n);
    f = malloc(sizeof(int)*n);

    for (int i = 0; i < n; i++)
    {
        *(color + i) = 'w'; // w stands for white color
        *(pi + i) = -10000; // indicates Nil
    }

    time = 0;

    for (int i = 0; i < n; i++)
    {
        if(*(color + i) == 'w')
        {
            DFS_VISIT(g,i);
        }
    }
}

void print_times (struct graph* g) // function for printing the discovery and finishing times
{
    int n = g->vert;
    for (int i = 0; i < n; i++)
    {
        printf("Vertex %d: discovery time %d, finishing time %d", i, *(d + i), *(f + i));
        endl;
    }
}

int IS_DAG(struct graph* g) // Function checks whether the graph is DAG or not by checking is there is a back edge or not
 {
    int u, v;
    for (int i = 0; i < g->vert; i++)
    {
        struct node* dup = g->arr[i].one;

        if(dup == NULL)
        {
            ;
        }
        else
        {
            while(dup!= NULL)
            {
                if (dup->edge == 'B')
                {
                    printf("The inserted directed graph is not a DAG.");
                    endl;
                    return 0;
                }
                dup = dup->next;
            }
        }
    }
    printf("The inserted directed graph is a DAG.");
    endl;
    return 1;
 }

 void TOPOLOGICAL_SORT(struct graph* g) // Orders the vertices in decreasing order of their finishing times as per the DFS
 {
     int n = g->vert;

     int arr[n];
     for (int i = 0; i < n; i++)
     {
         arr[i] = *(f + i);
     }
     int m;
     for(int i = 0; i < n; i++)
     {
         for(int j = i + 1; j < n; j++)
         {
             if(arr[i] < arr[j])
             {
                m = arr[i];
                arr[i] = arr[j];
                arr[j] = m;
             }
         }
     }
     topo = malloc(sizeof(int)*n);

     int t = 0;
     for (int i = 0; i < n; i++)
     {
        for (int k = 0; k < n; k++)
        {
            if(arr[i] == *(f + k))
            {
                *(topo + t) = k;
                t++;
            }
        }
     }
 }

void graph_transpose(struct graph* g, struct graph* gt) // function for finding the transpose of a graph
 {
    for (int i = 0; i < g->vert; i++)
    {
        struct node* dup = g->arr[i].one;

        if(dup == NULL)
        {
            ;
        }
        else
        {
            while(dup!= NULL)
            {
                create_directed_edge(gt, dup->conn, i);
                dup = dup->next;
            }
        }
    }
 }

 void DFS_VISIT_GT (struct graph* gt, int u) // DFS visit function for the DFS on the transpose graph
{
    time = time + 1;
    *(dt + u) = time;
    *(colort + u) = 'g';
    printf("%d ",u);

    int v;

    struct node* dup = gt->arr[u].one;

    if(dup == NULL)
    {
        ;
    }
    else
    {
        while(dup!= NULL)
        {
            v = dup->conn;

                if(*(color + v) == 'w') // w stands for white color
                {
                    dup->edge = 'T'; // T stands for tree edge
                }
                else if (*(color + v) == 'g') // g stands for gray vertex
                {
                    dup->edge = 'B'; // B stands for back edge
                }
                else
                {
                    if(*(d + u) > *(d + v))
                    {
                        dup->edge = 'C'; // C stands for cross edge
                    }
                    else if (*(d + u) < *(d + v))
                    {
                        dup->edge = 'F'; // F stands for forward edge
                    }
                }

            if (*(colort + v) == 'w')
            {
                *(pit + v) = u;
                DFS_VISIT_GT (gt, v);
            }
            dup = dup->next;
        }
    }
    *(colort + u) = 'b'; // b stands for black color
    time = time + 1;
    *(ft + u) = time;
}

 void DFS_GT (struct graph* gt) // DFS function for the transpose graph (with the main loop calling vertices in decreasing order of their finishing times)
{
    int n = gt->vert;

    colort = malloc(sizeof(char)*n);
    dt = malloc(sizeof(int)*n);
    pit = malloc(sizeof(int)*n);
    ft = malloc(sizeof(int)*n);

    for (int i = 0; i < n; i++)
    {
        *(colort + *(topo + i)) = 'w'; // w stands for white color
        *(pit + *(topo + i)) = -10000; // indicates Nil
    }

    time = 0;
    l = 1;
    for (int i = 0; i < n; i++)
    {
        if(*(colort + *(topo + i)) == 'w')
        {
            printf("Component %d: vertices ",l);
            l++;
            DFS_VISIT_GT(gt,*(topo + i));
            endl;
        }
    }
}

void STRONGLY_CONNECTED_COMPONENTS(struct graph* g) // Function for finding the strongly connected components of a graph
{
    struct graph* gt = generate_graph(g->vert);
    graph_transpose(g,gt);
    TOPOLOGICAL_SORT(g);
    printf("The inserted directed graph is decomposed into components. The components have the following vertices:");
    endl;
    DFS_GT(gt);
}

int main()
{
    int N;
    printf("Enter the number of vertices: ");
    scanf("%d", &N);

    printf("Your vertices are numbered from ");
    for (int i = 0; i<N-1; i++)
    {
        printf("%d, ",i);
    }
    printf("%d",N-1);
    endl;

    struct graph* G = generate_graph(N);

    int p,q;
    int ch;

        printf("Enter edges (in format: a b), one by one of your directed graph. Once done, enter -1 -1 to quit:");
        endl;
        while(1)
        {
            scanf("%d %d", &p, &q);
            if(p == -1 || q == -1)
            {
                break;
            }
            else
            {
                create_directed_edge (G,p,q);
            }
        }

        printf("Your directed graph is as follows:");
        endl;
        display_graph(G);
        endl;
        DFS(G);
        printf("You want to check the DAG property or you want to decompose the graph into its strongly connected components.");
        endl;
        printf("In other word, you want to run part 1b or part 1c?");
        endl;

        printf("Press '2' for part 1b and '3' for part 1c: ");

        scanf("%d", &ch);
        int m;
        int n = G->vert;

        if(ch == 2)
        {
            m = IS_DAG(G);
            if (m == 1)
            {
               TOPOLOGICAL_SORT(G);

               printf("A possible topological ordering of G is:");
               endl;

               for (int i = 0; i < n; i++)
               {
                    printf("%d ", *(topo + i));
               }
               endl;
            }
        }
        if (ch == 3)
        {
            STRONGLY_CONNECTED_COMPONENTS(G);
        }

    return 0;
}
