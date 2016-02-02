#ifndef SRC_SORT_HEAPSORT_H
#define SRC_SORT_HEAPSORT_H

#include <stdio.h>
#include <iostream>

namespace sort {

    /**
     * Пирамидальная сортировка массива
     */
    template<typename T>
    class HeapSort {
    public:

        HeapSort() {
        }


        /**
         * Пирамидальная сортировка массива
         */
        void sort(T *const arr, size_t arrSize) {
            //Этап 1: построение пирамиды из массива
            for (size_t i = (arrSize / 2); i > 0; --i) {
                siftDown(arr, i - 1, arrSize);
            }

            //Этап 2: сортировка с помощью пирамиды.
            while (arrSize > 1) //Пока в пирамиде больше одного элемента
            {
                --arrSize; //Отделяю последний элемент

                //Меняю местами корневой элемент и отделённый:
                T const firstElem = arr[0];
                arr[0] = arr[arrSize];
                arr[arrSize] = firstElem;

                //Просеиваю новый корневой элемент:
                siftDown(arr, 0, arrSize);
            }
        }


    private:

        /**
         * Просеивает элемент номер i вниз в пирамиде heap.
         * Идея в том, что вычисляется максимальный элемент из трёх:
         * 1) текущий элемент
         * 2) левый потомок
         * 3) правый потомок
         *
         * Если индекс текущего элемента не равен индексу максимального,
         * то меняю их местами, и перехожу к максимальному
         */
        void siftDown(T *const arr, size_t index, size_t const arrSize) {
            //Индекс максимального элемента в текущей тройке элементов:
            size_t nMax = index;
            //Значение текущего элемента (не меняется):
            T const value = arr[index];

            while (true) {
                //Рассматривается элемент i и его потомки i*2+1 и i*2+2
                //В начале каждой итерации nMax == i и value == heap[i]

                size_t childN = index * 2 + 1; //Индекс левого потомка
                //Если есть левый потомок и он больше текущего элемента,
                if ((childN < arrSize) && (arr[childN] > value)) {
                    nMax = childN; //  то он считается максимальным
                }

                ++childN; //Индекс правого потомка
                //Если есть правый потомок и он больше максимального,
                if ((childN < arrSize) && (arr[childN] > arr[nMax]))
                    nMax = childN; //  то он считается максимальным

                //Если текущий элемент является максимальным из трёх
                //  (т.е. если он больше своих детей), то конец:
                if (nMax == index) break;

                //Меняю местами текущий элемент с максимальным:
                arr[index] = arr[nMax];
                arr[nMax] = value;
                //  при этом значение value будет в ячейке nMax,
                //  поэтому в начале следующей итерации значение value
                //  правильно, т.к. мы переходим именно к этой ячейке

                //Переходим к изменившемуся потомку
                index = nMax;

            };
        }

    };
}


#endif
