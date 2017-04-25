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

    // strbuf_read(&sb, 0, 0);
    sb.addstr("abc\ndefg\nhij\ndefg\n");

    if (sb.buf[sb.len - 1] == '\n') {
        sb.setLen(sb.len - 1);
    }
    list.splitInPlace(sb.buf, '\n', -1);
    list.sort();
    assert(std::strcmp(list[1].string, "defg") == 0);
    assert(std::strcmp(list[2].string, "defg") == 0);
}

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
