/**
 * @author ustc-lezg
 */
#include <stdio.h>
#include <stdlib.h>

#define INFINITY 0x7fffffff

// 获取x这个数的d位数上的数字
int getDigit(int x, int d) {
    int a[] = {
            1, 1, 10
    }; // 本实例中的最大数是十位数，所以只要到10就可以了
    return ((x / a[d]) % 10);
}

void radixSort(int *arr, int begin, int end, int digit) {
    int radix = 10; // 基数
    int i = 0, j = 0;
    int count[radix]; // 存放各个桶的数据统计个数
    int bucket[end - begin + 1];

    // 按照从低位到高位的顺序执行排序过程
    for (int d = 1; d <= digit; d++) {

        // 置空各个桶的数据统计
        for (i = 0; i < radix; i++) {
            count[i] = 0;
        }

        // 统计各个桶将要装入的数据个数
        for (i = begin; i <= end; i++) {
            j = getDigit(arr[i], d);
            count[j]++;
        }

        // count[i]表示第i个桶的右边界索引
        for (i = 1; i < radix; i++) {
            count[i] = count[i] + count[i - 1];
        }

        // 将数据依次装入桶中
        // 这里要从右向左扫描，保证排序稳定性
        for (i = end; i >= begin; i--) {
            j = getDigit(arr[i], d); // 求出关键码的第k位的数字， 例如：576的第3位是5
            bucket[count[j] - 1] = arr[i]; // 放入对应的桶中，count[j]-1是第j个桶的右边界索引
            count[j]--; // 对应桶的装入数据索引减一
        }

        // 将已分配好的桶中数据再倒出来，此时已是对应当前位数有序的表
        for (i = begin, j = 0; i <= end; i++, j++) {
            arr[i] = bucket[j];
        }
    }
}

//获得未排序数组中最大的一个元素值
int GetMaxVal(int *arr, int len) {

    int maxVal = arr[0]; //假设最大为arr[0]

    for (int i = 1; i < len; i++)  //遍历比较，找到大的就赋值给maxVal
    {
        if (arr[i] > maxVal)
            maxVal = arr[i];
    }

    return maxVal;  //返回最大值
}

//桶排序
void bucketSort(int *arr, int len) {
    int tmpArrLen = GetMaxVal(arr, len) + 1;
    int tmpArr[tmpArrLen];  //获得空桶大小
    int i, j;

    for (i = 0; i < tmpArrLen; i++) //空桶初始化
        tmpArr[i] = 0;

    for (i = 0; i < len; i++) //寻访序列，并且把数字一个一个放到对应的桶子去。
        tmpArr[arr[i]]++;

    for (i = 0, j = 0; i < tmpArrLen; i++) {
        while (tmpArr[i] != 0) //对每个不是空的桶子进行排序。
        {
            arr[j] = i;  //从不是空的桶子里把项目再放回原来的序列中。
            j++;
            tmpArr[i]--;
        }
    }
}

//array是待调整的堆数组，i是待调整的数组元素的位置，nlength是数组的长度
void HeapAdjust(int *array, int i, int nLength) {
    int nChild;
    int nTemp;
    for (; 2 * i + 1 < nLength; i = nChild) {
        //子结点的位置=2*（父结点位置）+1
        nChild = 2 * i + 1;
        //得到子结点中较大的结点
        if (nChild < nLength - 1 && array[nChild + 1] > array[nChild])
            ++nChild;
        //如果较大的子结点大于父结点那么把较大的子结点往上移动，替换它的父结点
        if (array[i] < array[nChild]) {
            nTemp = array[i];
            array[i] = array[nChild];
            array[nChild] = nTemp;
        }
        else break; //否则退出循环
    }
}

void buildMaxHeap(int *arr, int length) {
    int i;
    //调整序列的前半部分元素，调整完之后第一个元素是序列的最大的元素
    //length/2-1是最后一个非叶节点，此处"/"为整除
    for (i = length / 2 - 1; i >= 0; --i)
        HeapAdjust(arr, i, length);
}

//堆排序算法
void heapSort(int *arr, int length) {
    buildMaxHeap(arr, length);
    int i;
    //从最后一个元素开始对序列进行调整，不断的缩小调整的范围直到第一个元素
    for (i = length - 1; i > 0; --i) {
        //把第一个元素和当前的最后一个元素交换，
        //保证当前的最后一个位置的元素都是在现在的这个序列之中最大的
        arr[i] = arr[0] ^ arr[i];
        arr[0] = arr[0] ^ arr[i];
        arr[i] = arr[0] ^ arr[i];
        //不断缩小调整heap的范围，每一次调整完毕保证第一个元素是当前序列的最大值
        HeapAdjust(arr, 0, i);
    }
}

int partition(int *arr, int low, int high) {
    int left = low;
    int right = high;
    int temp = arr[low];
    while (left < right) {
        while (left < right && arr[right] >= temp) {
            --right;
        }
        if (left < right) {
            arr[left] = arr[right];
        }
        while (left < right && arr[left] <= temp) {
            ++left;
        }
        if (left < right) {
            arr[right] = arr[left];
        }
    }
    arr[left] = temp;
    return left;
}

//快速排序
void quickSort(int arr[], int low, int high) {
    int pivot;
    if (low < high) {
        pivot = partition(arr, low, high);
        quickSort(arr, low, pivot - 1);
        quickSort(arr, pivot + 1, high);
    }
}

void merge(int *arr, int p, int mid, int q) {
    int a = mid - p + 1;
    int b = q - mid;
    int *L = (int *) calloc(a + 1, sizeof(int));
    int *R = (int *) calloc(b + 1, sizeof(int));
    int i;
    for (i = 0; i < a; ++i) {
        L[i] = arr[p + i];
    }
    for (i = 0; i < b; ++i) {
        R[i] = arr[mid + i + 1];
    }
    L[a] = INFINITY;
    R[b] = INFINITY;
    int j = 0;
    int k = 0;
    i = 0;
    for (k = p; k <= q; ++k) {//归并
        if (L[i] <= R[j]) {
            arr[k] = L[i++];
        } else {
            arr[k] = R[j++];
        }
    }
    free(L);
    free(R);
}

//归并排序
void mergeSort(int *arr, int p, int q) {
    int mid;
    if (p < q) {
        mid = (p + q) / 2;
        mergeSort(arr, p, mid);
        mergeSort(arr, mid + 1, q);
        merge(arr, p, mid, q);
    }
}

//希尔排序
void shellSort(int *arr, int n) {
    if (arr == NULL || n == 0) {
        return;
    }
    int gap = n / 2;
    while (1 <= gap) {
        int i;
        for (i = gap; i < n; ++i) {
            int temp = arr[i];
            int j;
            for (j = i - gap; j >= 0 && temp < arr[j]; j -= gap) {
                arr[j + gap] = arr[j];
            }
            arr[j + gap] = temp;
        }
        gap /= 2;
    }
}

//直接选择排序
void selectionSort(int *arr, int n) {
    if (arr == NULL || n == 0) {
        return;
    }
    int i, j;
    int min;
    for (i = 0; i < n - 1; ++i) {
        min = i;
        for (j = i + 1; j < n; ++j) {
            if (arr[min] > arr[j]) {
                min = j;
            }
        }
        if (min == i) {
            continue;
        }
        arr[min] = arr[min] ^ arr[i];
        arr[i] = arr[min] ^ arr[i];
        arr[min] = arr[min] ^ arr[i];
    }
}

//冒泡排序
void bubbleSort(int *arr, int n) {
    int flag = 1;//标记一次遍历没有发生移位，排序完毕

    if (arr == NULL || n == 0) {
        return;
    }
    for (int i = n - 1; i >= 1 && flag; --i) {
        flag = 0;
        for (int j = 0; j < i; ++j) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                if (flag == 0) {
                    flag = 1;//排序完成
                }
            }
        }
    }
}

//直接插入排序
void insertSort(int *arr, int n) {   //第一个数肯定是有序的，从第二个数开始遍历
    for (int i = 1; i < n; ++i) {
        int temp = arr[i];
        int j;
        for (j = i - 1; j >= 0 && temp < arr[j]; --j) {
            arr[j + 1] = arr[j];
        }
        arr[j + 1] = temp;
    }
}

int main() {
    
    return 0;
}
