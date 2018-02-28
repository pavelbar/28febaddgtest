#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "../TList.h"

using namespace testing;

TEST(TList, can_create_list)
{
    ASSERT_NO_THROW(TList<int> list);
}

TEST(TList,check_the_size_after_insert_first )
{
    TList<int> list;
    list.InSertFirst(1);
    list.InSertFirst(2);

    EXPECT_EQ(list.GetSize(), 2);
}

TEST(TList, check_the_size_after_insert_last)
{
    TList<int> list;
    list.InSertLast(1);
    list.InSertLast(2);

    EXPECT_EQ(list.GetSize(), 2);
}

TEST(TList, check_the_size_after_random_insert)
{
    TList<int> list;
    list.InSertFirst(1);
    list.InSertLast(2);
    list.InSertInPos(1, 3);

    EXPECT_EQ(list.GetSize(), 3);
}

TEST(TList, check_the_size_after_random_insert_and_del_first)
{
    TList<int> list;
    list.InSertFirst(1);
    list.InSertLast(3);
    list.InSertInPos(2, 2);
    list.DeleteFirst();

    EXPECT_EQ(list.GetSize(), 2);
}

TEST(TList, check_the_size_after_random_insert_and_del_last)
{
    TList<int> list;
    list.InSertFirst(1);
    list.InSertLast(3);
    list.InSertInPos(2, 2);
    list.DeleteLast();

    EXPECT_EQ(list.GetSize(), 2);
}
TEST(TList, check_the_size_after_random_insert_and_del_pos)
{
    TList<int> list;
    list.InSertFirst(1);
    list.InSertLast(3);
    list.InSertInPos(2, 2);
    list.Delete(2);

    EXPECT_EQ(list.GetSize(), 2);
}

TEST(TList,check_the_pos_after_insert_first_1)
{
    TList<int> list;
    list.InSertFirst(1);
    list.InSertFirst(2);
    list.InSertFirst(3);

    EXPECT_EQ(list.GetElem(0), 3);
}
TEST(TList, check_the_pos_after_insert_first_2)
{
    TList<int> list;
    list.InSertFirst(1);
    list.InSertFirst(2);
    list.InSertFirst(3);

    EXPECT_EQ(list.GetElem(1), 2);
}
TEST(TList, check_the_pos_after_insert_first_3)
{
    TList<int> list;
    list.InSertFirst(1);
    list.InSertFirst(2);
    list.InSertFirst(3);

    EXPECT_EQ(list.GetElem(2), 1);
}

TEST(TList, check_the_pos_after_insert_last_1)
{
    TList<int> list;
    list.InSertLast(1);
    list.InSertLast(2);
    list.InSertLast(3);

    EXPECT_EQ(list.GetElem(0), 1);
}
TEST(TList, check_the_pos_after_insert_last_2)
{
    TList<int> list;
    list.InSertLast(1);
    list.InSertLast(2);
    list.InSertLast(3);

    EXPECT_EQ(list.GetElem(1), 2);
}
TEST(TList, check_the_pos_after_insert_last_3)
{
    TList<int> list;
    list.InSertLast(1);
    list.InSertLast(2);
    list.InSertLast(3);

    EXPECT_EQ(list.GetElem(2), 3);
}

TEST(TList, check_the_pos_after_insert_random_1)
{
    TList<int> list;
    list.InSertFirst(1);
    list.InSertLast(3);
    list.InSertInPos(2, 2);

    EXPECT_EQ(list.GetElem(0), 1);
}
TEST(TList, check_the_pos_after_insert_random_2)
{
    TList<int> list;
    list.InSertFirst(1);
    list.InSertLast(3);
    list.InSertInPos(2, 2);

    EXPECT_EQ(list.GetElem(1), 3);
}
TEST(TList, check_the_pos_after_insert_random_3)
{
    TList<int> list;
    list.InSertFirst(1);
    list.InSertLast(3);
    list.InSertInPos(2, 2);

    EXPECT_EQ(list.GetElem(2), 2);
}

TEST(TList, check_the_pos_after_del_insert_1)
{
    TList<int> list;
    list.InSertFirst(1);
    list.InSertLast(3);
    list.InSertInPos(2, 2);
    list.DeleteFirst();

    EXPECT_EQ(list.GetElem(0), 3);
}

TEST(TList, check_the_pos_after_del_insert_2)
{
    TList<int> list;
    list.InSertFirst(1);
    list.InSertLast(3);
    list.InSertInPos(2, 2);
    list.DeleteFirst();
    list.DeleteFirst();

    EXPECT_EQ(list.GetElem(0), 2);
}

TEST(TList, check_the_pos_after_del_last_1)
{
    TList<int> list;
    list.InSertFirst(1);
    list.InSertLast(3);
    list.InSertInPos(2, 2);
    list.DeleteLast();

    EXPECT_EQ(list.GetElem(1), 3);
}
TEST(TList, check_the_pos_after_del_last_2)
{
    TList<int> list;
    list.InSertFirst(1);
    list.InSertLast(3);
    list.InSertInPos(2, 2);
    list.DeleteLast();
    list.DeleteLast();

    EXPECT_EQ(list.GetElem(0), 1);
}
TEST(TList, check_the_pos_after_del_in_pos_1)
{
    TList<int> list;
    list.InSertFirst(1);
    list.InSertLast(3);
    list.InSertInPos(2, 2);
    list.Delete(0);

    EXPECT_EQ(list.GetElem(0), 3);
}
TEST(TList, check_the_pos_after_del_in_pos_2)
{
    TList<int> list;
    list.InSertFirst(1);
    list.InSertLast(3);
    list.InSertInPos(2, 2);
    list.Delete(1);

    EXPECT_EQ(list.GetElem(0), 1);
}
TEST(TList, check_the_pos_after_del_in_pos_3)
{
    TList<int> list;
    list.InSertFirst(1);
    list.InSertLast(3);
    list.InSertInPos(2, 2);
    list.Delete(2);

    EXPECT_EQ(list.GetElem(1), 3);
}
TEST(TList, check_size_after_del_all)
{
    TList<int> list;

    list.InSertFirst(1);
    list.InSertLast(3);
    list.InSertInPos(2, 2);
    list.InSertFirst(21);
    list.DeleteAll();

    EXPECT_EQ(0, list.GetSize());
}
TEST(TList, can_assign)
{
    TList<int> list_one;
    TList<int> list_two;

    list_two.InSertFirst(1);
    list_two.InSertFirst(2);

    list_one = list_two;

    EXPECT_EQ(list_one, list_two);
}

TEST(TList, can_assing_two_list)
{
    TList<int> list_one;
    TList<int> list_two;

    list_two.InSertFirst(1);
    list_two.InSertFirst(2);

    list_one.InSertFirst(1);
    list_one.InSertFirst(2);

    EXPECT_EQ(list_one, list_two);
}

TEST(TList, can_not_assing_two_list)
{
    TList<int> list_one;
    TList<int> list_two;

    list_two.InSertFirst(1);
    list_two.InSertFirst(2);

    list_one.InSertFirst(1);
    list_one.InSertFirst(5);

    EXPECT_NE(list_one, list_two);
}
TEST(TList, can_set_elem_in_list)
{
    TList<int> list;
    list.InSertFirst(2);
    list.InSertFirst(3);

    list.SetElem(0, 12);

    EXPECT_EQ(12, list.GetElem(0));
}
TEST(TList, can_copy_list)
{
    TList<int> list;
    list.InSertFirst(2);
    list.InSertFirst(3);

    TList<int> tmp(list);

    EXPECT_EQ(tmp, list);
}
