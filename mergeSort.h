

void mergeSort(int* arrDg, int* arrNO, int tam){
    if (tam < 2) return;
    int mid = tam / 2;
    int* leftDg = new int[mid];
    int* rightDg = new int[tam - mid];
    int* leftNO = new int[mid];
    int* rightNO = new int[tam - mid];
    for (int i = 0; i < mid; i++) {
        leftDg[i] = arrDg[i];
        leftNO[i] = arrNO[i];
    }
    for (int i = mid; i < tam; i++) {
        rightDg[i - mid] = arrDg[i];
        rightNO[i - mid] = arrNO[i];
    }
    mergeSort(leftDg, leftNO, mid);
    mergeSort(rightDg, rightNO, tam - mid);
    merge(arrDg, leftDg, leftNO, mid, rightDg, rightNO, tam - mid);
    delete[] leftDg;
    delete[] rightDg;
    delete[] leftNO;
    delete[] rightNO;
    return;
}

void merge(int* arr, int* leftDg, int* leftNO, int leftSize, int* rightDg, int* rightNO, int rightSize) {
 int i = 0, j = 0, k = 0;
 while (i < leftSize && j < rightSize) {
    if (leftDg[i] == rightDg[j]) {
        if (leftNO[i] < rightNO[j]){
                arr[k] = leftNO[i];
                arr[k] = leftDg[i];
                k++;
                i++;
            }else{
                arr[k] = rightNO[j];
                arr[k] = rightDg[j];
                k++;
                j++;
            } 
    } else if(leftDg[i] > rightDg[j]) {
        arr[k] = rightNO[j];
        arr[k] = rightDg[j];
        k++;
        j++;
    }else{
        arr[k] = leftNO[i];
        arr[k] = leftDg[i];
        k++;
        i++;
    }
 }
 while (i < leftSize) {
    arr[k] = leftDg[i];
    arr[k] = leftNO[i];
    k++;
    i++;
 }
 while (j < rightSize) {
    arr[k] = rightDg[j];
    arr[k] = rightNO[j];
    k++;
    j++;
 }
}
