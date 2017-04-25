#include "cache/StringListTest.h"

namespace test {
namespace cache_string_list {

bool prefix_cb(cache::StringList<bool>::ListItem* item, void* cb_data) {
    const char* prefix = (const char*)cb_data;
    return cache::str::starts_with(item->string, prefix);
}

void test_split() {
    cache::StringList<bool> list(nullptr, true);
    list.split("abc,defg,hij", ',', 3);
    // list.print();
    assert(list.size() == 3);
    assert(std::strcmp(list[0].string, "abc") == 0);
    assert(std::strcmp(list[2].string, "hij") == 0);
    list.clear(false);
}

void test_split_in_place() {
    cache::StringList<bool> list(nullptr, false);
    char string[] = "abc,defg,hij";
    list.splitInPlace(string, ',', 1);
    assert(list.size() == 2);
    assert(std::strcmp(list[0].string, "abc") == 0);
    assert(std::strcmp(list[1].string, "defg,hij") == 0);
    list.clear(false);
}

void test_filter() {
    cache::StringList<bool> list(nullptr, true);
    list.split("abc:defg:hij", ':', -1);
    // list.print();

    assert(list.size() == 3);
    const char* prefix = "def";
    list.filter(false, prefix_cb, (void*)prefix);
    assert(list.size() == 1);
    // list.print();

    assert(std::strcmp(list[0].string, "defg") == 0);
    list.clear(false);
}

void test_remove_duplicates() {
    cache::StringList<bool> list(nullptr, true);
    list.split("abc:defg:hij:defg", ':', -1);

    assert(list.size() == 4);
    // sort list before call remove duplicates
    list.sort();
    // list.print();
    list.removeDuplicates(false);
    // list.print();
    assert(list.size() == 3);

    assert(std::strcmp(list[1].string, "defg") == 0);
    list.clear(false);
}

void test_sort_list() {
    cache::StringList<bool> list(nullptr, false);
    cache::str::StringBuffer sb;
    cache::StringList<bool>::ListItem* item;

    // strbuf_read(&sb, 0, 0);
    sb.addstr("abc\ndefg\nhij\ndefg\n");

    // list.print();
    //std::printf("sb %s\n", sb.buf);
    if (sb.buf[sb.len - 1] == '\n') {
        sb.setLen(sb.len - 1);
	}
    //std::printf("sb %s\n", sb.buf);
    list.splitInPlace(sb.buf, '\n', -1);
	list.sort();
    assert(std::strcmp(list[1].string, "defg") == 0);
    assert(std::strcmp(list[2].string, "defg") == 0);
    // char string[] = "abc,defg,hij";
    // list.splitInPlace(string, ',', 1);
    // assert(list.size() == 2);
    // assert(std::strcmp(list[0].string, "abc") == 0);
    // assert(std::strcmp(list[1].string, "defg,hij") == 0);
    // list.clear(false);
}

// int cmd_main(int argc, const char **argv)
//{

// if (argc == 2 && !strcmp(argv[1], "sort")) {
// struct string_list list = STRING_LIST_INIT_NODUP;
// struct strbuf sb = STRBUF_INIT;
// struct string_list_item *item;

// strbuf_read(&sb, 0, 0);

/*
 * Split by newline, but don't create a string_list item
 * for the empty string after the last separator.
 */
// if (sb.buf[sb.len - 1] == '\n')
// strbuf_setlen(&sb, sb.len - 1);
// string_list_split_in_place(&list, sb.buf, '\n', -1);

// string_list_sort(&list);

// for_each_string_list_item(item, &list)
// puts(item->string);

// string_list_clear(&list, 0);
// strbuf_release(&sb);
// return 0;
//}

// fprintf(stderr, "%s: unknown function name: %s\n", argv[0],
// argv[1] ? argv[1] : "(there was none)");
// return 1;
//}

void stringList_test() {
    suite("StringList");
    mytest(split);
    mytest(split_in_place);
    mytest(filter);
    mytest(remove_duplicates);
    mytest(sort_list);
}
}
}
