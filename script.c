#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define dim_offset 4096

int k;
int f_counter;
char *max_string;
int first_time;
int array_to_use; // 0 means a, 1 means b; indicates the array where you have to take the data
int array_is_ordered; // 0 means no, 1 means yes

struct dynamic_array
{
  char **array;
  int full_len;
  int last_index;
};

struct albero
{
  struct nodo *root;
  struct nodo *nil;
};

struct list_node
{
  char *key;
  struct list_node *next;
};

struct list
{
  struct list_node *head;
  int len;
};

struct nodo
{
  struct nodo *p;
  struct nodo *left;
  struct nodo *right;
  char key[];
};

struct q_node
{
  struct q_node *next;
  struct nodo *el;
};

struct queue
{
  struct q_node *head;
  struct q_node *tail;
};

struct b
{
  char ** M;
  int lower_bound[128];
  int exact_number[128];
  int plus_counter[128];
  int counter;
  int *found_plus;
};

void free_dynamic_array(struct dynamic_array *a)
{
  free(a->array);
  free(a);
};
int mystrcmp(char *a, char *b)
{
  for(int i = 0; i < k; i ++)
  {
    if(a[i] != b[i])
      return 1;
  }

  return 0;
};
struct dynamic_array * inizialize_dynamic_array()
{
  struct dynamic_array *a = malloc(sizeof(struct dynamic_array));
  char ** array = malloc(sizeof(char * ) * dim_offset);

  a->array = array;
  a->full_len = dim_offset;
  a->last_index = -1;
  a->array[0] = NULL;

  return a;
};
void insert_dynamic_array(struct dynamic_array *a, char *string)
{
  a->last_index++;

  if(a->last_index >= a->full_len - 3)
  {
    a->full_len += dim_offset;
    a->array = realloc(a->array, a->full_len * sizeof(char *));
  }

  a->array[a->last_index] = string;
};
void myprint(char *string)
{
  for(int i = 0; i < k; i++)
  {
    putchar(string[i]);
  }

  putchar('\n');
};
int lex_minority(char *a, char *b) //if a < b return 0; if a == b return 1; if a > b return 2;
{
  for(int i = 0; i < k; i++)
  {
    if(a[i] < b[i])
    return 0;
    if(a[i] > b[i])
    return 2;
  }

  return 1;
};
void merge(char ** A, int p, int q, int r, int counter)
{
  int i, j;
  int n_1 = q - p + 1;
  int n_2 = r - q;

  char ** L = malloc(sizeof(char *) * (n_1 + 1));
  char ** R = malloc(sizeof(char *) * (n_2 + 1));

  for(i = 0; i < n_1; i++)
  {
    L[i] = A[p + i];
  }

  for(j = 0; j < n_2; j++)
  {
    R[j] = A[q + j + 1];
  }

  L[n_1] = max_string;

  R[n_2] = max_string;

  i = 0;
  j = 0;

  for(int h = p; h <= r; h++)
  {
    if(lex_minority(L[i], R[j]) == 0)
    {
      A[h] = L[i];
      i = i + 1;
    }

    else
    {
      A[h] = R[j];
      j = j + 1;
    }
  }

  free(L);
  free(R);
};
void merge_sort(char **A, int p, int r, int counter)
{
  int q;
  if(p < r)
  {
    q = (p + r) / 2;
    merge_sort(A, p, q, counter);
    merge_sort(A, q+1, r, counter);
    merge(A, p, q, r, counter);
  }
};
void exec_stampa_bounds(struct b * bounds)
{
  char temp;
  printf("M:\n");
  for(int i = 0; i < k; i++)
  {
    printf("%d\t", i);
    for(int j = 0; j < 128; j++)
    {
      temp = bounds->M[i][j];
      if( ((j>= 'A' && j <= 'Z') || (j>= 'a' && j <= 'z') ||  (j>= '0' && j <= '9') || j == '-' || j == '_') && temp == 1)
        printf("%c", j);
    }

    printf("\n");
  }

  printf("\n\n");

  printf("Lower bound: \n");
  for(int i = 0; i < 128; i++)
    if( (i>= 'A' && i <= 'Z') || (i>= 'a' && i <= 'z') ||  (i>= '0' && i <= '9') || i == '-' || i == '_')
    {
      printf("%c\t", i);
      printf("%d\n", bounds->lower_bound[i]);
    }

  printf("\n\n");

  printf("Exact number: \n");
  for(int i = 0; i < 128; i++)
    if( (i>= 'A' && i <= 'Z') || (i>= 'a' && i <= 'z') ||  (i>= '0' && i <= '9') || i == '-' || i == '_')
    {
      printf("%c\t", i);
      printf("%d\n", bounds->exact_number[i]);
    }

  printf("\n\n");

  printf("Plus counter: \n");
  for(int i = 0; i < 128; i++)
    if( (i>= 'A' && i <= 'Z') || (i>= 'a' && i <= 'z') ||  (i>= '0' && i <= '9') || i == '-' || i == '_')
    {
      printf("%c\t", i);
      printf("%d\n", bounds->plus_counter[i]);
    }

  printf("Counter: %d\n\n", bounds->counter);
};
int count_if_correct(char *string, struct b *bounds)
{


  int counter = bounds->counter;

  for(int i = 0; i < k; i++)
  {
    if(bounds->M[i][(int)string[i]] == 0)
    {

      for(int j = 0; j < i; j ++)
      {
        if(bounds->exact_number[(int)string[j]] != -2)
          bounds->exact_number[(int)string[j]]++;
        else
          bounds->lower_bound[(int)string[j]]++;
      }

      return 0;
    }

    if(bounds->exact_number[(int)string[i]] != -2)
    {
      bounds->exact_number[(int)string[i]]--;
      counter--;


      if(bounds->exact_number[(int)string[i]] < 0)
      {
        for(int j = 0; j <= i; j ++)
        {
          if(bounds->exact_number[(int)string[j]] != -2)
            bounds->exact_number[(int)string[j]]++;
          else
            bounds->lower_bound[(int)string[j]]++;
        }
        return 0;
      }
    }

    else
    {
      bounds->lower_bound[(int)string[i]]--;

      if(bounds->lower_bound[(int)string[i]] >= 0)
      {
        counter--;
      }
    }

  }

  for(int j = 0; j < k; j ++)
  {
    if(bounds->exact_number[(int)string[j]] != -2)
      bounds->exact_number[(int)string[j]]++;
    else
      bounds->lower_bound[(int)string[j]]++;
  }

  if(counter < 0)
    printf("OH DOWN COUNTER < 0\n");

  if(counter > 0)
  {
    return 0;
  }
  else
  {
    return 1;
  }

};
void declare_list(struct list *L)
{
  L->head = NULL;
  L->len = 0;
};
void head_insert_list(struct list *L, char *key)
{
  struct list_node *temp = malloc(sizeof(struct list_node));
  temp->key = key;
  temp->next = L->head;
  L->head = temp;
  L->len++;
};
void list_insert_temp(struct list *L, char **a, int n, struct b *bounds)
{
  int i = 0;
  struct list_node *curr = L->head;
  struct list_node *prec = NULL;

  while(i < n && count_if_correct(a[i], bounds) == 0 )
  {
    i++;
  }

  if(L->head == NULL && i < n) //lista vuota
  {
    struct list_node *temp = malloc(sizeof(struct list_node));
    temp->key = a[i];
    temp->next = NULL;
    L->head = temp;
    prec = temp;
    curr = NULL;
    L->len++;
    i++;
  }

  if(i < n)
    if(lex_minority(a[i], curr->key))
    {
      struct list_node *temp = malloc(sizeof(struct list_node));
      temp->key = a[i];
      temp->next = L->head;
      L->head = temp;
      prec = temp;
      L->len++;
      i++;
    }

  prec = curr;
  curr = curr->next;
  while(i < n)
  {

    while(curr != NULL && i < n && lex_minority(a[i], curr->key) == 0)
    {
      if(count_if_correct(a[i], bounds) == 1)
      {
        struct list_node *temp = malloc(sizeof(struct list_node));
        temp->key = a[i];
        temp->next = curr;
        prec->next = temp;
        prec = temp;
        L->len++;
        i++;
      }
      else
        i++;

      if(i == n)
        break;
    }

    while(curr == NULL && i < n)
    {
      if(count_if_correct(a[i], bounds) == 1)
      {
        struct list_node *temp = malloc(sizeof(struct list_node));
        temp->key = a[i];
        temp->next = curr;
        prec->next = temp;
        prec = temp;
        L->len++;
        i++;
      }
      else
        i++;
    }

    prec = curr;
    if(curr != NULL)
      curr = curr->next;
  }
};
void update_array(struct dynamic_array *a, struct dynamic_array *b, struct b *bounds, char *r)
{
  struct dynamic_array *source;
  struct dynamic_array *destination;

  if(array_to_use == 0)
  {
    source = a;
    destination = b;
  }
  else
  {
    source = b;
    destination = a;
  }

  destination->last_index = -1;

  for(int i = 0; i <= source->last_index; i++)
  {
    if(count_if_correct(source->array[i], bounds) == 1)
    {
      insert_dynamic_array(destination, source->array[i]);
    }
  }

  destination->array[destination->last_index + 1] = NULL;

  if(destination == a)
    array_to_use = 0;

  else
    array_to_use = 1;
};
void free_list(struct list *L)
{
  struct list_node *curr = L->head;
  struct list_node *temp;

  while(curr != NULL)
  {
    temp = curr;
    curr = curr->next;

    free(temp);
  }


  free(L);
};
void free_tree_nodes(struct albero *T, struct nodo * x)
{
  if(x != T->nil)
  {
    free_tree_nodes(T, x->left);
    free_tree_nodes(T, x->right);

    free(x);
  }
};
void free_tree(struct albero *T)
{
  free_tree_nodes(T, T->root);

  free(T->nil);
  free(T);
}
void free_bounds(struct b* bounds)
{
  for(int i = 0; i <k; i++)
    free(bounds->M[i]);

  free(bounds->M);
  free(bounds->found_plus);
  free(bounds);

};
void inizialize_tree(struct albero* T)
{
  struct nodo *nil = malloc(sizeof(struct nodo) + (sizeof(char) * (k + 1)));


  for(int i = 0; i < k; i++)
    nil->key[i] = '#';
  nil->key[k] = 'b';

  nil->p = nil;
  nil->left = nil;
  nil->right = nil;

  T->nil = nil;
  T->root = nil;


};
struct nodo *tree_minimum(struct nodo *x, struct albero *T)
{
  while(x->left != T->nil)
    x = x->left;
  return x;
};
struct nodo *tree_successor(struct nodo *x, struct albero *T)
{
  struct nodo *y;

  if(x->right != T->nil)
    return tree_minimum(x->right, T);

  y = x->p;

  while(y != T->nil && x == y->right)
  {
    x = y;
    y = y->p;
  }

  return y;
};
void left_rotate(struct nodo *x, struct albero *T)
{
  struct nodo *y;

  y = x->right;
  x->right = y->left;

  if (y->left != T->nil)
    y->left->p = x;

  y->p = x->p;
  if(x->p == T->nil)
    T->root = y;

  else if (x == x->p->left)
    x->p->left = y;

  else
    x->p->right = y;

  y->left = x;
  x->p = y;
};
void right_rotate(struct nodo *x, struct albero *T)
{
  struct nodo *y;

  y = x->left;
  x->left = y->right;

  if (y->right != T->nil)
    y->right->p = x;

  y->p = x->p;
  if(x->p == T->nil)
    T->root = y;

  else if (x == x->p->right)
    x->p->right = y;

  else
    x->p->left = y;

  y->right = x;
  x->p = y;
};
void rb_transplant(struct albero *T, struct nodo *u, struct nodo *v)
{
  if(u->p == T->nil)
    T->root = v;
  else if (u == u->p->left)
    u->p->left = v;
  else
    u->p->right = v;

  v->p = u->p;
};
void rb_delete_fixup(struct nodo *x, struct albero *T)
{
  struct nodo *w;
  while(x != T->root && x->key[k] == 'b')
  {
    if(x == x->p->left)
    {
      w = x->p->right;
      if(w->key[k] == 'r')
      {
        w->key[k] = 'b';
        x->p->key[k] = 'r';
        left_rotate(x->p, T);
        w = x->p->right;
      }

      if(w->left->key[k] == 'b' && w->right->key[k] == 'b')
      {
        w->key[k] = 'r';
        x = x->p;
      }

      else
      {
        if(w->right->key[k] == 'b')
        {
          w->left->key[k] = 'b';
          w->key[k] = 'r';
          right_rotate(w, T);
          w = x->p->right;
        }

        w->key[k] = x->p->key[k];
        x->p->key[k] = 'b';
        w->right->key[k] = 'b';
        left_rotate(x->p, T);
        x = T->root;
      }
    }

    else
    {
      w = x->p->left;
      if(w->key[k] == 'r')
      {
        w->key[k] = 'b';
        x->p->key[k] = 'r';
        right_rotate(x->p, T);
        w = x->p->left;
      }

      if(w->right->key[k] == 'b' && w->left->key[k] == 'b')
      {
        w->key[k] = 'r';
        x = x->p;
      }

      else
      {
        if(w->left->key[k] == 'b')
        {
          w->right->key[k] = 'b';
          w->key[k] = 'r';
          left_rotate(w, T);
          w = x->p->left;
        }

        w->key[k] = x->p->key[k];
        x->p->key[k] = 'b';
        w->left->key[k] = 'b';
        right_rotate(x->p, T);
        x = T->root;
      }
    }
  }

  x->key[k] = 'b';
};
void rb_delete(struct nodo *z, struct albero *T)
{
  struct nodo *y;
  struct nodo *x;
  
  char y_original_color;
  y = z;
  y_original_color = y->key[k];

  if(z->left == T->nil)
  {
    x = z->right;
    rb_transplant(T, z, z->right);
  }

  else if (z->right == T->nil)
  {
    x = z->left;
    rb_transplant(T, z, z->left);
  }

  else
  {
    y = tree_minimum(z->right, T);
    y_original_color = y->key[k];
    x = y->right;

    if(y->p == z)
      x->p = y;

    else
    {
      rb_transplant(T, y, y->right);
      y->right = z->right;
      y->right->p = y;
    }

    rb_transplant(T, z, y);
    y->left = z->left;
    y->left->p = y;
    y->key[k] = z->key[k];
  }

  if(y_original_color == 'b')
    rb_delete_fixup(x, T);

  free(z);
};
void rb_insert_fixup(struct nodo *z, struct albero *T)
{
  struct nodo *y;

  while(z->p->key[k] == 'r')
  {
    if(z->p == z->p->p->left)
    {
      y = z->p->p->right;
      if(y->key[k] == 'r')
      {
        z->p->key[k] = 'b';
        y->key[k] = 'b';
        z->p->p->key[k] = 'r';
        z = z->p->p;
      }
      else
      {
        if(z == z->p->right)
        {
          z = z->p;
          left_rotate(z, T);
        }

        z->p->key[k] = 'b';
        z->p->p->key[k] = 'r';
        right_rotate(z->p->p, T);
      }
    }

    else
    {
      y = z->p->p->left;
      if(y->key[k] == 'r')
      {
        z->p->key[k] = 'b';
        y->key[k] = 'b';
        z->p->p->key[k] = 'r';
        z = z->p->p;
      }
      else
      {
        if(z == z->p->left)
        {
          z = z->p;
          right_rotate(z, T);
        }

        z->p->key[k] = 'b';
        z->p->p->key[k] = 'r';
        left_rotate(z->p->p, T);
      }
    }
  }

  T->root->key[k] = 'b';
};
void rb_insert(struct nodo *z, struct albero *T)
{
  struct nodo *y;
  struct nodo *x;

  y = T->nil;
  x = T->root;

  while(x != T->nil)
  {
    y = x;
    if (lex_minority(z->key, x->key) == 0)
      x = x->left;
    else
      x = x->right;
  }

  z->p = y;

  if(y == T->nil)
    T->root = z;

  else if (lex_minority(z->key, y->key) == 0)
    y->left = z;

  else
    y->right = z;

  z->left = T->nil;
  z->right = T->nil;
  z->key[k] = 'r';
  rb_insert_fixup(z, T);
};
void exec_stampa_filtrate_first_time(struct albero *T, struct nodo *x)
{
  if(x != T->nil)
  {
    exec_stampa_filtrate_first_time(T, x->left);
    myprint(x->key);
    exec_stampa_filtrate_first_time(T, x->right);
  }
};
void exec_stampa_filtrate(struct dynamic_array *a, struct dynamic_array *b, struct albero *T)
{
  if(first_time == 1)
  {
    exec_stampa_filtrate_first_time(T, T->root);
  }

  struct dynamic_array *v;

  if(array_to_use == 0)
    v = a;

  else
    v = b;

  if(array_is_ordered == 0)
  {
    merge_sort(v->array, 0, v->last_index, v->last_index + 1);
    array_is_ordered = 1;
  }

  for(int i = 0; i <=v->last_index; i++)
    myprint(v->array[i]);
};
void copy_tree_in_array_if_bounds(struct albero * T, struct nodo *x, struct b *bounds, struct dynamic_array * a)
{
  if(x != T->nil)
  {
    copy_tree_in_array_if_bounds(T, x->left, bounds, a);

    if(count_if_correct(x->key, bounds) == 1)
    {
      insert_dynamic_array(a, x->key);
    }

    copy_tree_in_array_if_bounds(T, x->right, bounds, a);

  }
};
void update_array_first_time(struct albero * T, struct b *bounds, struct dynamic_array * a)
{

  copy_tree_in_array_if_bounds(T, T->root, bounds, a);

  a->array[a->last_index + 1] = NULL;
  array_to_use = 0;
  array_is_ordered = 1;
};
void copy_list_to_array(struct list *L, char **a)
{
  int i = 0;
  struct list_node *curr = L->head;

  while(curr != NULL)
  {
    a[i] = curr->key;
    i++;
    curr = curr->next;
  }
};
void update_bounds(char *res, char *p, struct b *bounds, struct dynamic_array *a, struct dynamic_array *b, char *r, struct albero *T)
{
  int temp_count[128] = {0};


  for(int i = 0; i < k; i++)
  {
    if(res[i] == '+')
    {
      temp_count[(int)p[i]]++;
      if(bounds->found_plus[i] == 0)
      {
        bounds->plus_counter[(int)p[i]]++;
        bounds->found_plus[i] = 1;
      }
      for(int j = 0; j < 128; j++)
        if(j != p[i])
          bounds->M[i][j] = 0;
    }

    if(res[i] == '|')
    {
      temp_count[(int)p[i]]++;
      bounds->M[i][(int)p[i]] = 0;
    }

  }

  for(int i = 0; i < k; i++)
  {
    if(res[i] == '/')
    {
      if(temp_count[(int)p[i]] == 0)
        for(int j = 0; j < k; j++)
        {
          bounds->M[j][(int)p[i]] = 0;
          bounds->exact_number[(int)p[i]] = 0;
        }

      else
      {
        if(bounds->exact_number[(int)p[i]] == -2)
        {
          bounds->counter = bounds->counter + temp_count[(int)p[i]] - bounds->lower_bound[(int)p[i]];
        }

        bounds->exact_number[(int)p[i]] = temp_count[(int)p[i]];
        bounds->M[i][(int)p[i]] = 0;
      }
    }
  }

  for(int i = 0; i < 128; i++)
  {
    if(temp_count[i] > bounds->plus_counter[i])
    {
      if(temp_count[i] > bounds->lower_bound[i] && bounds->exact_number[i] == -2)
      {
        bounds->counter = bounds->counter - bounds->lower_bound[i] + temp_count[i];
        bounds->lower_bound[i] = temp_count[i];
      }
    }
    else
    {
      if(bounds->plus_counter[i] > bounds->lower_bound[i] && bounds->exact_number[i] == -2)
      {
        bounds->counter = bounds->counter - bounds->lower_bound[i] + bounds->plus_counter[i];
        bounds->lower_bound[i] = bounds->plus_counter[i];
      }
    }
  }

  if(first_time == 1)
  {
    update_array_first_time(T, bounds, a);
    first_time = 0;
  }
  else
    update_array(a, b, bounds, r);
};
void inizialize_bounds(struct b *bounds)
{
  char **M;
  M = malloc(sizeof(int *) * k);

  for(int i = 0; i < k; i ++)
    M[i] = malloc(sizeof(int)  * 128);

  for(int i = 0; i < k; i++)
    for(int j = 0; j < 128; j ++)
      M[i][j] = 1;

  bounds->M = M;

  for(int i = 0; i < 128; i++)
    bounds->lower_bound[i] = 0;

  for(int i = 0; i < 128; i++)
    bounds->exact_number[i] = -2;

  for(int i = 0; i < 128; i++)
    bounds->plus_counter[i] = 0;

  bounds->found_plus = malloc(sizeof(int) * k);

  for(int i = 0; i < k; i++)
    bounds->found_plus[i] = 0;

  bounds->counter = 0;
};
int compare(char *p, char *r, char *res, struct b *bounds, struct dynamic_array *a, struct dynamic_array *b, struct albero *T)
{
  if(!strcmp(p , r))
  {
    return 1;
  }

  int counter[128];

  for(int i = 0; i < 128; i++)
    counter[i] = 0;


  for(int i = 0; i < k; i++)
  {
    if(p[i] == r[i])
      res[i] = '+';
    else
    {
      counter[(int)r[i]]++;
    }
  }


  for(int i = 0; i < k; i++)
  {
    if(res[i] != '+')
    {
      if(counter[(int)p[i]] == 0)
        res[i] = '/';
      else
      {
        res[i] = '|';
        counter[(int)p[i]]--;
      }
    }
  }

  update_bounds(res, p, bounds, a, b, r, T);

  return 0;
};
char *tree_search(struct albero *T, struct nodo *x, char *k)
{
  if(x == T->nil)
    return NULL;
  else if (!mystrcmp(k, x->key))
    return x->key;
  if (lex_minority(k, x->key) == 0)
    return tree_search(T, x->left, k);
  else
    return tree_search(T, x->right, k);
};
void get_string(char *command, char *word)
{
  int i = 0;
  char c;

  c = (char)getchar();

  if(c == (char)EOF)
  {
    strcpy(command, "+fine");
    return;
  }

  if(c == '+')
  {
    command[i] = c;
    i = 1;

    while(1)
    {
      c = getchar();

      if(c == '\n')
      {
        command[i] = '\0';
        return;
      }
      command[i] = c;
      i++;
    }
  }

  else
  {
    word[i] = c;
    i = 1;

    while(1)
    {
      c = getchar();
      if (c == '\n')
      {
        word[i] = '\0';

        return;
      }
      word[i] = c;
      i++;
    }
  }
};
void exec_inserisci_inizio(struct albero *T, struct dynamic_array *a, struct dynamic_array *b, struct b *bounds, char *r)
{
  char *word;
  char *command;
  int counter = 0;
  struct dynamic_array *v;

  if(array_to_use == 0)
    v = a;
  else
    v = b;



  word = malloc(sizeof(char) * (k + 1));
  command =malloc(sizeof(char) * 30);

  while(1)
  {
    word[0] = '\0';
    command[0] = '\0';
    get_string(command, word);

    if(word[0] != '\0')
    {
      counter++;
      struct nodo *new_node = malloc(sizeof(struct nodo) + (sizeof(char) * (k * 1)) );

      for(int i = 0; i < k + 1; i++)
      {
          new_node->key[i] = word[i];
      }

      rb_insert(new_node, T);

      if(first_time == 0)
      {
        if(count_if_correct(new_node->key, bounds) == 1)
        {
          insert_dynamic_array(v, new_node->key);
        }
      }

    }
    else
    {

      if(first_time == 0)
        array_is_ordered = 0;

      free(word);
      free(command);
      return;
    }
  }

}
void exec_inserisci_inizio_main(struct albero *T)
{
  char *word;
  char *command;
  int counter = 0;

  word = malloc(sizeof(char) * (k + 1));
  command =malloc(sizeof(char) * 30);

  while(1)
  {
    word[0] = '\0';
    command[0] = '\0';
    get_string(command, word);

    if(word[0] != '\0')
    {
      counter++;
      struct nodo *new_node = malloc(sizeof(struct nodo) + (sizeof(char) * (k * 1)) );

      for(int i = 0; i < k + 1; i++)
      {
          new_node->key[i] = word[i];
      }

      rb_insert(new_node, T);

    }
    else
    {
      free(word);
      free(command);
      return;
    }
  }

}
void new_game(struct albero *T)
{
  int n, out;
  char *r;
  char *p;
  char *res;
  char *command;
  int scanf_output;

  first_time = 1;

  struct dynamic_array *a = inizialize_dynamic_array();
  struct dynamic_array *b = inizialize_dynamic_array();

  struct b *bounds = malloc(sizeof(struct b));
  r = malloc(sizeof(char) * (k + 1));
  res = malloc(sizeof(char) * (k + 1));

  for(int i = 0; i < k; i++)
    res[i] = '#';

  res[k] = '\0';

  scanf_output = scanf("%s", r);
  if(scanf_output == 0)
    printf("errore nella lettura della lettera");

  scanf_output = scanf("%d%*c", &n);
  if(scanf_output == 0)
    printf("errore nella lettura della lettera");

  inizialize_bounds(bounds);

  p = malloc(sizeof(char) * (k + 1));
  command = malloc(sizeof(char) * 30);
  while(n != 0)
  {
    p[0] = '\0';
    command[0] = '\0';
    get_string(command, p);

    if(p[0] != '\0')
    {

      if(tree_search(T, T->root, p) == NULL)
      {
        printf("not_exists\n");
        continue;
      }
      else
      {
        out = compare(p , r, res, bounds, a, b, T);
        if(out == 1)
        {
          printf("ok\n");
          free_dynamic_array(a);
          free_dynamic_array(b);
          free_bounds(bounds);
          free(r);
          free(res);
          free(p);
          free(command);

          return;
        }

        else
        {
          printf("%s\n", res);
          for(int i = 0; i < k; i++)
            res[i] = '#';

          if(array_to_use == 0)
            printf("%d\n", a->last_index + 1);
          else
            printf("%d\n", b->last_index + 1);

          n--;
        }
      }
    }

    else
    {
      if(!strcmp(command, "+stampa_filtrate"))
        exec_stampa_filtrate(a, b, T);

      if (!strcmp(command, "+inserisci_inizio"))
        exec_inserisci_inizio(T, a, b, bounds, r);

      if (!strcmp(command, "+stampa_bounds"))
        exec_stampa_bounds(bounds);
      if (!strcmp(command, "+fine"))
      {
        free_bounds(bounds);
        free_dynamic_array(a);
        free_dynamic_array(b);
        free(r);
        free(res);
        free(p);
        free(command);
        return;
      }
    }
  }


  free_bounds(bounds);
  free_dynamic_array(a);
  free_dynamic_array(b);
  free(r);
  free(res);
  free(p);
  free(command);
  printf("ko\n");
  return;
}


int main()
{
  int scanf_output;
  char * command;
  char *word;

  scanf_output = scanf("%d%*c", &k);
  if(scanf_output == 0)
  printf("errore nella lettura della lettera");

  struct albero *T = malloc(sizeof(struct albero));
  inizialize_tree(T);



  max_string = malloc(sizeof(char) * (k + 1));
  for(int i = 0; i < k; i ++)
    max_string[i] = 'z';

  max_string[k] = '\0';
  word = malloc(sizeof(char) * (k + 2));
  command = malloc(sizeof(char) * 30);
  while(1)
  {
    word[0] = '\0';
    command[0] = '\0';

    get_string(command, word);

    if(word[0] != '\0')
    {
      struct nodo *new_node = malloc(sizeof(struct nodo) + (sizeof(char) * (k + 1)) );

      for(int i = 0; i < k + 1; i++)
      {
          new_node->key[i] = word[i];
      }

      rb_insert(new_node, T);
    }

    else if (!strcmp(command, "+nuova_partita"))
    {
      new_game(T);
    }

    else if(!strcmp(command, "+inserisci_inizio"))
    {
      exec_inserisci_inizio_main(T);
    }

    else if (!strcmp(command, "+fine"))
    {
      free_tree(T);
      free(word);
      free(command);
      free(max_string);
      return 0;
    }
  }

}
