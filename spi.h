#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

extern double t;


//#include "int_t.h"
//#include "float_t.h"
#include "seg_t.h"

#ifdef FLOAT_TEST
#define L(a, b) l(a, b)
#define EQ(a, b) eq(a, b)
#define LE(a, b) le(a, b)
#endif
#ifdef INT_TEST
#define L(a, b) l(a, b)
#define EQ(a, b) eq(a, b)
#define LE(a, b) le(a, b)
#endif
#ifdef SEG_TEST
#define L(a, b) l(a, b, t)
#define EQ(a, b) eq(a, b, t)
#define LE(a, b) le(a, b, t)
#endif


typedef struct node {
    int size; // количество занятых ключей
    key_type key[3];
    struct node *first;  // *first <= key[0];
    struct node *second; // key[0] <= *second < key[1];
    struct node *third;  // key[1] <= *third < key[2];
    struct node *fourth; // kye[2] <= *fourth.
    struct node *parent; // Указатель на родителя нужен для того, потому что адрес корня может меняться при удалении
} Node, *pNode;

void swap(key_type *x, key_type *y);

void sort2(key_type *arr);

bool find(pNode _node, key_type k); // Этот метод возвращает true, если ключ k находится в вершине, иначе false.
void sort(pNode _node); // Ключи в вершинах должны быть отсортированы
void insert_to_node(pNode _node, key_type k); // Вставляем ключ k в вершину (не в дерево)
void remove_from_node(pNode _node, key_type k); // Удаляем ключ k из вершины (не из дерева)
void become_node2(pNode _node, key_type k, pNode first_, pNode second_); // Преобразовать в 2-вершину.
bool is_leaf(pNode _node); // Является ли узел листом; проверка используется при вставке и удалении.
void init_node_by_key(pNode _node, key_type k);
void init_node(pNode _node, key_type k, pNode first, pNode second, pNode third, pNode fourth, pNode parent);

pNode insert(pNode subroot, key_type k); // Вставка в дерево;
pNode split(pNode subroot);              // Метод для разделение вершины при переполнении;
pNode search(pNode subroot, key_type k); // Поиск в дереве;
pNode search_min(pNode subroot);         // Поиск минимального элемента в поддереве;
pNode del(pNode subroot, key_type k);    // Собственно, из названия понятно;
void print(pNode subroot, int indent);
void pint_with_colored_key(pNode subroot, int indent, key_type key);

pNode merge(pNode leaf);        // Слияние используется при удалении;
pNode redistribute(pNode leaf); // Перераспределение также используется при удалении;
pNode fix(pNode leaf);          // Используется после удаления для возвращения свойств дереву (использует merge или redistribute)

pNode search_max(pNode subroot);
key_type successor_seg(pNode subroot, key_type k);
key_type predecessor_seg(pNode subroot, key_type k);


#define CLR_RED     "\x1b[31m"
#define CLR_GREEN   "\x1b[32m"
#define CLR_YELLOW  "\x1b[33m"
#define CLR_BLUE    "\x1b[34m"
#define CLR_MAGENTA "\x1b[35m"
#define CLR_CYAN    "\x1b[36m"
#define CLR_RESET   "\x1b[0m"
