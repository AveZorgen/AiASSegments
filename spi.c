#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static double t;


#include "spi.h"


void swap(key_type *x, key_type *y) {
    key_type r = *x;
    *x = *y;
    *y = r;
}

void sort2(key_type *arr) {
    if (L(arr[1], arr[0]))
        swap(arr, arr + 1);
}



bool find(pNode _node, key_type k) { 
    int size = _node->size;
    for (int i = 0; i < size; ++i)
        if (EQ(_node->key[i], k))
            return true;
    return false;
}

void sort(pNode _node) {
    if (_node->size == 1)
        return;
    if (_node->size == 2)
        sort2(_node->key);
    else if (_node->size == 3) {
        sort2(_node->key);
        sort2(_node->key + 1);
        sort2(_node->key);
    }
}

void insert_to_node(pNode _node, key_type k) {
    _node->key[_node->size] = k;
    _node->size++;
    sort(_node);
}

void remove_from_node(pNode _node, key_type k) {
    if (_node->size >= 1 && EQ(_node->key[0], k)) {
        _node->key[0] = _node->key[1];
        _node->key[1] = _node->key[2];
        _node->size--;
    } else if (_node->size == 2 && EQ(_node->key[1], k)) {
        _node->key[1] = _node->key[2];
        _node->size--;
    }
}

void become_node2(pNode _node, key_type k, pNode first_, pNode second_) {
    _node->key[0] = k;
    _node->first = first_;
    _node->second = second_;
    _node->third = NULL;
    _node->fourth = NULL;
    _node->parent = NULL;
    _node->size = 1;
}

bool is_leaf(pNode _node) {
    return !(_node->first || _node->second || _node->third);
}

void init_node_by_key(pNode _node, key_type k) {
    _node->size = 1;
    _node->key[0] = k;
    _node->key[1] = _node->key[2] = default_key();
    _node->first = _node->second = _node->third = _node->fourth = _node->parent = NULL;
}

void init_node(pNode _node, key_type k, pNode first, pNode second, pNode third, pNode fourth, pNode parent) {
    _node->size = 1;
    _node->key[0] = k;
    _node->key[1] = _node->key[2] = default_key();
    _node->first = first;
    _node->second = second;
    _node->third = third;
    _node->fourth = fourth;
    _node->parent = parent;
}

pNode insert(pNode subroot, key_type k) { // вставка ключа k в дерево с корнем subroot; всегда возвращаем корень дерева, т.к. он может меняться
    if (!subroot) {
        pNode tmp = malloc(sizeof *tmp);
        init_node_by_key(tmp, k);
        return tmp; // если дерево пусто, то создаем первую 2-3-вершину (корень)
    }
    #ifdef SEG_TEST
    t = k->t_as_param;
    #endif

    if (is_leaf(subroot))
        insert_to_node(subroot, k);
    else if (LE(k, subroot->key[0]))
        insert(subroot->first, k);
    else if ((subroot->size == 1) || ((subroot->size == 2) && LE(k, subroot->key[1])))
        insert(subroot->second, k);
    else
        insert(subroot->third, k);

    if (subroot->size == 3)
        return split(subroot);
    return subroot;
}

pNode split(pNode subroot) {
    pNode x = malloc(sizeof(Node));
    pNode y = malloc(sizeof(Node));
    init_node(x, subroot->key[0], subroot->first, subroot->second, NULL, NULL, subroot->parent);
    init_node(y, subroot->key[2], subroot->third, subroot->fourth, NULL, NULL, subroot->parent);
    if (x->first)
        x->first->parent = x; // Правильно устанавливаем "родителя" "сыновей".
    if (x->second)
        x->second->parent = x; // После разделения, "родителем" "сыновей" является "дедушка",
    if (y->first)
        y->first->parent = y; // Поэтому нужно правильно установить указатели.
    if (y->second)
        y->second->parent = y;

    if (subroot->parent) {
        pNode parent = subroot->parent;
        insert_to_node(parent, subroot->key[1]);

        if (parent->first == subroot)
            parent->first = NULL;
        else if (parent->second == subroot)
            parent->second = NULL;
        else if (parent->third == subroot)
            parent->third = NULL;

        // Дальше происходит своеобразная сортировка ключей при разделении.
        if (parent->first == NULL) {
            parent->fourth = parent->third;
            parent->third = parent->second;
            parent->second = y;
            parent->first = x;
        } else if (parent->second == NULL) {
            parent->fourth = parent->third;
            parent->third = y;
            parent->second = x;
        } else {
            parent->fourth = y;
            parent->third = x;
        }

        free(subroot);
        return parent;
    } else {
        x->parent = subroot; // Так как в эту ветку попадает только корень,
        y->parent = subroot; // то мы "родителем" новых вершин делаем разделяющийся элемент.
        become_node2(subroot, subroot->key[1], x, y);
        return subroot;
    }
}

// may return null
pNode search(pNode subroot, key_type k) { // Поиск ключа k в 2-3 дереве с корнем subroot.
    if (!subroot)
        return NULL;

    if (find(subroot, k))
        return subroot;
    else if (L(k, subroot->key[0]))
        return search(subroot->first, k);
    else if ((subroot->size == 2) && L(k, subroot->key[1]) || (subroot->size == 1))
        return search(subroot->second, k);
    else if (subroot->size == 2)
        return search(subroot->third, k);
    return NULL;
}

pNode search_min(pNode subroot) { // Поиск узла с минимальным элементов в 2-3-дереве с корнем subroot.
    if (!subroot || !(subroot->first))
        return subroot;
    return search_min(subroot->first);
}

pNode search_max(pNode subroot) {
    if (!subroot)
        return subroot;
    if (subroot->third)
        return search_max(subroot->third);
    if (subroot->second)
        return search_max(subroot->second);
    return subroot;
}

key_type successor_seg(pNode subroot, key_type k) {
    #ifdef SEG_TEST
    t = k->t_as_param;
    #endif
    pNode item = search(subroot, k);
    if (!item)
        return NULL;
    if (is_leaf(item)) {
        if (item->size == 2 && EQ(item->key[0], k)) {
            return item->key[1];
        } else if ((item->size == 2 && EQ(item->key[1], k)) || item->size == 1) {
            while (item->parent &&
                   (item->parent->size == 2 && item->parent->third == item ||
                    item->parent->size == 1 && item->parent->second == item))
                item = item->parent;
            if (item->parent) {
                if (item->parent->first == item)
                    return item->parent->key[0];
                else if (item->parent->second == item)
                    return item->parent->key[1];
            }
        }
    } else {
        if (EQ(item->key[0], k))
            return search_min(item->second)->key[0];
        else if (EQ(item->key[1], k))
            return search_min(item->third)->key[0];
    }
    return NULL;
}

key_type predecessor_seg(pNode subroot, key_type k) {
    #ifdef SEG_TEST
    t = k->t_as_param;
    #endif
    pNode item = search(subroot, k);
    if (!item)
        return NULL;
    if (is_leaf(item)) {
        if (item->size == 2 || item->size == 1) {
            if (item->size == 2 && EQ(item->key[1], k)) {
                return item->key[0];
            } else if (EQ(item->key[0], k)) {
                while (item->parent && item->parent->first == item) item = item->parent;
                if (item->parent) {
                    if (item->parent->second == item)
                        return item->parent->key[0];
                    else if (item->parent->third == item)
                        return item->parent->key[1];
                }
            }
        }
    } else {
        if (EQ(item->key[0], k))
            return search_max(item->first)->key[0];
        else if (EQ(item->key[1], k)) 
            return search_max(item->second)->key[0];
    }
    return NULL;
}

pNode del(pNode subroot, key_type k) { // Удаление ключа k в 2-3-дереве с корнем subroot.
    #ifdef SEG_TEST
    t = k->t_as_param;
    #endif
    pNode item = search(subroot, k);   // Ищем узел, где находится ключ k

    if (!item)
        return subroot;

    pNode min = NULL;
    if (EQ(item->key[0], k)) {
        min = search_min(item->second); // Ищем эквивалентный ключ
        if (min) {                      // Меняем ключи местами
            swap(&item->key[0], &min->key[0]);
            item = min; // Перемещаем указатель на лист, т.к. min - всегда лист
        }
    } else {
        min = search_min(item->third);
        if (min) { // Меняем ключи местами
            swap(&item->key[1], &min->key[0]);
            item = min; // Перемещаем указатель на лист, т.к. min - всегда лист
        }
    }

    remove_from_node(item, k); // И удаляем требуемый ключ из листа
    return fix(item);          // Вызываем функцию для восстановления свойств дерева.
}

// may return null if tree is empty
pNode fix(pNode leaf) {
    if (leaf->size == 0 && leaf->parent == NULL) { // Случай 0, когда удаляем единственный ключ в дереве
        free(leaf);
        return NULL;
    }
    if (leaf->size != 0) { // Случай 1, когда вершина, в которой удалили ключ, имела два ключа
        if (leaf->parent)
            return fix(leaf->parent);
        else
            return leaf;
    }

    pNode parent = leaf->parent;
    if (parent->first->size == 2 || parent->second->size == 2 || parent->size == 2)
        leaf = redistribute(leaf); // Случай 2, когда достаточно перераспределить ключи в дереве
     else if (parent->size == 2 && parent->third->size == 2)
         leaf = redistribute(leaf); // Случай 2, когда достаточно перераспределить ключи в дереве
    else
        leaf = merge(leaf); // Случай 3, когда нужно произвести склеивание и пройтись вверх по дереву как минимум на еще одну вершину

    return fix(leaf);
}

pNode redistribute(pNode leaf) {
    pNode parent = leaf->parent;
    pNode first = parent->first;
    pNode second = parent->second;
    pNode third = parent->third;

    if ((parent->size == 2) && (first->size < 2) && (second->size < 2) && (third->size < 2)) {
        if (first == leaf) {
            parent->first = parent->second;
            parent->second = parent->third;
            parent->third = NULL;
            insert_to_node(parent->first, parent->key[0]);
            parent->first->third = parent->first->second;
            parent->first->second = parent->first->first;

            if (leaf->first != NULL)
                parent->first->first = leaf->first;
            else if (leaf->second != NULL)
                parent->first->first = leaf->second;

            if (parent->first->first != NULL)
                parent->first->first->parent = parent->first;

            remove_from_node(parent, parent->key[0]);
            free(first);
        } else if (second == leaf) {
            insert_to_node(first, parent->key[0]);
            remove_from_node(parent, parent->key[0]);
            if (leaf->first != NULL)
                first->third = leaf->first;
            else if (leaf->second != NULL)
                first->third = leaf->second;

            if (first->third != NULL)
                first->third->parent = first;

            parent->second = parent->third;
            parent->third = NULL;

            free(second);
        } else if (third == leaf) {
            insert_to_node(second, parent->key[1]);
            parent->third = NULL;
            remove_from_node(parent, parent->key[1]);
            if (leaf->first != NULL)
                second->third = leaf->first;
            else if (leaf->second != NULL)
                second->third = leaf->second;

            if (second->third != NULL)
                second->third->parent = second;

            free(third);
        }
    } else if ((parent->size == 2) && ((first->size == 2) || (second->size == 2) || (third->size == 2))) {
        if (third == leaf) {
            if (leaf->first != NULL) {
                leaf->second = leaf->first;
                leaf->first = NULL;
            }

            insert_to_node(leaf, parent->key[1]);
            if (second->size == 2) {
                parent->key[1] = second->key[1];
                remove_from_node(second, second->key[1]);
                leaf->first = second->third;
                second->third = NULL;
                if (leaf->first != NULL)
                    leaf->first->parent = leaf;
            } else if (first->size == 2) {
                parent->key[1] = second->key[0];
                leaf->first = second->second;
                second->second = second->first;
                if (leaf->first != NULL)
                    leaf->first->parent = leaf;

                second->key[0] = parent->key[0];
                parent->key[0] = first->key[1];
                remove_from_node(first, first->key[1]);
                second->first = first->third;
                if (second->first != NULL)
                    second->first->parent = second;
                first->third = NULL;
            }
        } else if (second == leaf) {
            if (third->size == 2) {
                if (leaf->first == NULL) {
                    leaf->first = leaf->second;
                    leaf->second = NULL;
                }
                insert_to_node(second, parent->key[1]);
                parent->key[1] = third->key[0];
                remove_from_node(third, third->key[0]);
                second->second = third->first;
                if (second->second != NULL)
                    second->second->parent = second;
                third->first = third->second;
                third->second = third->third;
                third->third = NULL;
            } else if (first->size == 2) {
                if (leaf->second == NULL) {
                    leaf->second = leaf->first;
                    leaf->first = NULL;
                }
                insert_to_node(second, parent->key[0]);
                parent->key[0] = first->key[1];
                remove_from_node(first, first->key[1]);
                second->first = first->third;
                if (second->first != NULL)
                    second->first->parent = second;
                first->third = NULL;
            }
        } else if (first == leaf) {
            if (leaf->first == NULL) {
                leaf->first = leaf->second;
                leaf->second = NULL;
            }
            insert_to_node(first, parent->key[0]);
            if (second->size == 2) {
                parent->key[0] = second->key[0];
                remove_from_node(second, second->key[0]);
                first->second = second->first;
                if (first->second != NULL)
                    first->second->parent = first;
                second->first = second->second;
                second->second = second->third;
                second->third = NULL;
            } else if (third->size == 2) {
                parent->key[0] = second->key[0];
                second->key[0] = parent->key[1];
                parent->key[1] = third->key[0];
                remove_from_node(third, third->key[0]);
                first->second = second->first;
                if (first->second != NULL)
                    first->second->parent = first;
                second->first = second->second;
                second->second = third->first;
                if (second->second != NULL)
                    second->second->parent = second;
                third->first = third->second;
                third->second = third->third;
                third->third = NULL;
            }
        }
    } else if (parent->size == 1) {
        insert_to_node(leaf, parent->key[0]);

        if (first == leaf && second->size == 2) {
            parent->key[0] = second->key[0];
            remove_from_node(second, second->key[0]);

            if (leaf->first == NULL)
                leaf->first = leaf->second;

            leaf->second = second->first;
            second->first = second->second;
            second->second = second->third;
            second->third = NULL;
            if (leaf->second != NULL)
                leaf->second->parent = leaf;
        } else if (second == leaf && first->size == 2) {
            parent->key[0] = first->key[1];
            remove_from_node(first, first->key[1]);

            if (leaf->second == NULL)
                leaf->second = leaf->first;

            leaf->first = first->third;
            first->third = NULL;
            if (leaf->first != NULL)
                leaf->first->parent = leaf;
        }
    }
    return parent;
}

pNode merge(pNode leaf) {
    pNode parent = leaf->parent;

    if (parent->first == leaf) {
        insert_to_node(parent->second, parent->key[0]);
        parent->second->third = parent->second->second;
        parent->second->second = parent->second->first;

        if (leaf->first != NULL)
            parent->second->first = leaf->first;
        else if (leaf->second != NULL)
            parent->second->first = leaf->second;

        if (parent->second->first != NULL)
            parent->second->first->parent = parent->second;

        remove_from_node(parent, parent->key[0]);
        free(parent->first);
        parent->first = NULL;
    } else if (parent->second == leaf) {
        insert_to_node(parent->first, parent->key[0]);

        if (leaf->first != NULL)
            parent->first->third = leaf->first;
        else if (leaf->second != NULL)
            parent->first->third = leaf->second;

        if (parent->first->third != NULL)
            parent->first->third->parent = parent->first;

        remove_from_node(parent, parent->key[0]);
        free(parent->second);
        parent->second = NULL;
    }

    if (parent->parent == NULL) {
        pNode tmp = NULL;
        if (parent->first != NULL)
            tmp = parent->first;
        else
            tmp = parent->second;
        tmp->parent = NULL;
        free(parent);
        return tmp;
    }
    return parent;
}

#define INDENT_INC 4
#define __indent(current) ((current) + INDENT_INC)

void print(pNode nd, int indent) {
    if (!nd)
        return;

    for (int i = 0; i < indent; i++) {
        if ((i + 1) % INDENT_INC) {
            putchar(' ');
        } else {
            putchar('|');
        }
    }
    putchar('-');
    putchar('[');

    int k = 0;
    for (; k < nd->size - 1; k++) {
        print_key(nd->key[k]); 
        putchar(' ');
    }
    print_key(nd->key[k]);
    putchar('\n');

    if (!nd->first)
        return;

    print(nd->first, __indent(indent));

    if (nd->third) {
        print(nd->second, __indent(indent));
        print(nd->third, __indent(indent));
    } else {
        print(nd->second, __indent(indent));
    }
}

#define CLR_RED     "\x1b[31m"
#define CLR_GREEN   "\x1b[32m"
#define CLR_YELLOW  "\x1b[33m"
#define CLR_BLUE    "\x1b[34m"
#define CLR_MAGENTA "\x1b[35m"
#define CLR_CYAN    "\x1b[36m"
#define CLR_RESET   "\x1b[0m"

void pint_with_colored_key(pNode nd, int indent, key_type key) {
    if (!nd)
        return;

    for (int i = 0; i < indent; i++) {
        if ((i + 1) % INDENT_INC) {
            putchar(' ');
        }
        else {
            putchar('|');
        }
    }
    putchar('-');
    putchar('[');

    int k = 0;
    key_type key_to_print;
    bool e;
    for (; k < nd->size - 1; k++) {
        key_to_print = nd->key[k];
        if (EQ(key_to_print, key)) {
            printf(CLR_GREEN);
            print_key(key_to_print);
            printf(CLR_RESET);
        }
        else {
            //printf("%d: [(%lf %lf) (%lf %lf)]", key_to_print->u.seg_idx, key_to_print->u.x, key_to_print->u.y, key_to_print->v.x, key_to_print->v.y);
            print_key(key_to_print);
        }
        putchar(' ');
    }

    key_to_print = nd->key[k];
    if (EQ(key_to_print, key)) {
        printf(CLR_GREEN);
        print_key(key_to_print);
        printf(CLR_RESET);
    }
    else {
        print_key(key_to_print);
    }
    putchar('\n');

    if (!nd->first)
        return;

    pint_with_colored_key(nd->first, __indent(indent), key);

    if (nd->third) {
        pint_with_colored_key(nd->second, __indent(indent), key);
        pint_with_colored_key(nd->third, __indent(indent), key);
    }
    else {
        pint_with_colored_key(nd->second, __indent(indent), key);
    }
}