
// Program  :  NOI 2005 Sequence

#include <iostream>

using namespace std;

struct node
  {
    int value, size, sum, maxl, maxr, maxm;
    bool rev, same;
    node *ch[2], *pre;
  } tree[500003], *queue[500003], *stack[500003], *Null, *root;

int num[500001], top1 = 0, top2 = 0;

node *New_Node(int x)
  {
    node *p;
    if (top2) p = stack[top2 --]; else p = &tree[top1 ++];
    p->value = p->sum = p->maxl = p->maxr = p->maxm = x, p->size = 1;
    p->pre = p->ch[0] = p->ch[1] = Null, p->rev = p->same = false;
    return p;
  }

void Reverse(node *x)
  {
    if (x == Null) return;
    swap(x->ch[0], x->ch[1]), swap(x->maxl, x->maxr), x->rev = ! x->rev;
  }

void Make_Same(node *x, int c)
  {
    if (x == Null) return;
    x->same = true, x->value = c, x->sum = c * x->size;
    x->maxl = x->maxr = x->maxm = max(x->sum, x->value);
  }

void Push_Down(node *x)
  {
    if (x == Null) return;
    if (x->same)
      Make_Same(x->ch[0], x->value), Make_Same(x->ch[1], x->value), x->same = false;
    if (x->rev)
      Reverse(x->ch[0]), Reverse(x->ch[1]), x->rev = false;
  }

void Update(node *x)
  {
    if (x == Null) return;
    x->size = x->ch[0]->size + x->ch[1]->size + 1;
    x->sum = x->ch[0]->sum + x->ch[1]->sum + x->value;
    x->maxl = max(x->ch[0]->maxl, x->ch[0]->sum + x->value + max(0, x->ch[1]->maxl));
    x->maxr = max(x->ch[1]->maxr, x->ch[1]->sum + x->value + max(0, x->ch[0]->maxr));
    x->maxm = max(x->ch[0]->maxm, x->ch[1]->maxm);
    x->maxm = max(x->maxm, max(x->ch[0]->maxr, 0) + x->value + max(x->ch[1]->maxl, 0));
  }

void Rotate(node *x, int c)
  {
    node *y = x->pre;
    Push_Down(y), Push_Down(x);
    y->ch[! c] = x->ch[c], x->pre = y->pre;
    if (x->ch[c] != Null) x->ch[c]->pre = y;
    if (y->pre != Null) y->pre->ch[y->pre->ch[1] == y] = x;
    y->pre = x, x->ch[c] = y;
    if (y == root) root = x;
    Update(y);
  }

void Splay(node *x, node *f)
  {
    for (Push_Down(x); x->pre != f; )
      if (x->pre->pre == f)
        Rotate(x, x->pre->ch[0] == x);
      else
        {
          node *y = x->pre, *z = y->pre;
          if (z->ch[0] == y)
            if (y->ch[0] == x)
              Rotate(y, 1), Rotate(x, 1);
            else
              Rotate(x, 0), Rotate(x, 1);
          else
            if (y->ch[1] == x)
              Rotate(y, 0), Rotate(x, 0);
            else
              Rotate(x, 1), Rotate(x, 0);
        }
    Update(x);
  }

void Select(int k, node *f)
  {
    node *now;
    for (now = root; ; )
      {
        Push_Down(now);
        int tmp = now->ch[0]->size;
        if (tmp + 1 == k) break;
        if (k <= tmp)
          now = now->ch[0];
        else
          now = now->ch[1], k -= tmp + 1;
      }
    Splay(now, f);
  }

void Clear(node *x)
  {
    int head, tail;
    for (head = tail = 1, queue[1] = x; head <= tail; head ++)
      if (queue[head] != Null)
        {
          stack[++ top2] = queue[head];
          queue[++ tail] = queue[head]->ch[0], queue[++ tail] = queue[head]->ch[1];
        }
  }

node *Make_Tree(int l, int r, node *fa)
  {
    if (l > r) return Null;
    int mid = l + r >> 1;
    node *p = New_Node(num[mid]);
    p->ch[0] = Make_Tree(l, mid-1, p), p->ch[1] = Make_Tree(mid+1, r, p);
    p->pre = fa, Update(p);
    return p;
  }

void Ins_Num(int pos, int tot)
  {
    for (int i = 1; i <= tot; i ++) scanf("%d", &num[i]);
    Select(pos, Null), Select(pos+1, root);
    root->ch[1]->ch[0] = Make_Tree(1, tot, root->ch[1]);
    Splay(root->ch[1]->ch[0], Null);
  }

int main()
  {
    int n, m, i, pos, tot, c;
    char ctrl[20];
    
    // freopen("sequence.in", "r", stdin);
    // freopen("sequence.out", "w", stdout);
    
	int T;
	scanf("%d",&T);
	while(T--){
    Null = New_Node(-100000), Null->size = Null->sum = 0, root = New_Node(-100000);
    root->ch[1] = New_Node(-100000), root->ch[1]->pre = root, Update(root);
    scanf("%d%d", &n, &m), Ins_Num(1, n);
    
    for (i = 1; i <= m; i ++)
      {
        scanf("%s", ctrl);
        if (ctrl[2] != 'X') scanf("%d%d", &pos, &tot);
        if (ctrl[0] == 'I') Ins_Num(pos+1, tot);
        if (ctrl[0] == 'D')
          {
            Select(pos, Null), Select(pos+tot+1, root);
            Clear(root->ch[1]->ch[0]), root->ch[1]->ch[0] = Null, Splay(root->ch[1], Null);
          }
        if (ctrl[0] == 'R')
          {
            Select(pos, Null), Select(pos+tot+1, root);
            Reverse(root->ch[1]->ch[0]), Splay(root->ch[1], Null);
          }
        if (ctrl[0] == 'G')
          Select(pos, Null), Select(pos+tot+1, root), printf("%d\n", root->ch[1]->ch[0]->sum);
        if (ctrl[0] == 'M')
          if (ctrl[2] == 'K')
            {
              scanf("%d", &c), Select(pos, Null), Select(pos+tot+1, root);
              Make_Same(root->ch[1]->ch[0], c), Splay(root->ch[1], Null);
            }
          else
            printf("%d\n", root->maxm);
      }
    }
    fclose(stdin), fclose(stdout);
    
    return 0;
  }
