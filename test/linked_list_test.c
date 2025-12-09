#include <core/list.h>
#include <core/utils.h>
#include <core/log.h>

struct test_data_struct {
    char *tag;
    usz id;
};

void test_data_struct_init(struct test_data_struct *data, char *tag, usz id)
{
    data->tag = tag;
    data->id = id;
}

struct test_data_struct *generate_tds_entry(struct m_arena *arena)
{
    struct test_data_struct *ret = m_arena_alloc(arena, sizeof(*ret));
    ASSERT_RT(ret, "failed to allocate structure!");

    return ret;
}

void print_list_in_order(struct doubly_linked_list list)
{
    LOG("printing list in order:");
    for (EACH_DOUBLY_LINKED_LIST(list, node)) {
        struct test_data_struct *entry = (struct test_data_struct *)node.data;
        LOG("\tnode: %p", node);
        LOG("\tnode.tag: %s", entry->tag);
        LOG("\tnode.id: %d", entry->id);
        LOG("\tnext: %p\n", node->next);
    }
}

void print_list_array(struct doubly_linked_list list)
{
    struct test_data_struct *array = list.data;
    LOG("printing list array:");
    for (usz i = 0; i < list.count; ++i) {
        LOG("\tarray[%d].tag : %s", array[i].tag);
        LOG("\tarray[%d].id : %d", array[i].id);
    }
}

#define MAX_ENTRIES 128
int main(int argc, char **argv)
{
    struct m_arena tds_arena = {0};
    struct m_arena_info tds_arena_info = {
        .mem_size   = MAX_ENTRIES * sizeof(struct test_data_struct),
        .buffer     = (struct test_data_struct[MAX_ENTRIES]) {0},
        .external   = TRUE,
    };
    m_arena_init(&tds_arena, tds_arena_info);

    struct doubly_linked_list list = {0};
    struct doubly_linked_list_info list_info = {
        .width = sizeof(struct test_data_struct),
        .count = MAX_ENTRIES,
    };
    doubly_linked_list_init(&list, list_info);

    for (usz i = 0; i < MAX_ENTRIES; ++i) {
        struct test_data_struct *tds_entry = generate_tds_entry(&tds_arena);
        test_data_struct_init(tds_entry);
        doubly_linked_list_add(&list, tds_entry);
    }

    print_list_array(list);
    print_list_in_order(list);

    return 0;
}
